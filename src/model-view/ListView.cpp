#include "ListView.h"
#include <algorithm>
#include <functional>
#include "ListSingleSelectionModel.h"
#include "ListMultiSelectionModel.h"
#include "Lock.h"

using std::bind2nd;
using std::less;
using std::find_if;

/**
 */
ListView::ListView(LgIndex_t                           guiListId,
                   ListViewInterface::SelectionModel_e selectionModel)
    : m_guiListId(guiListId)
    , m_ownsModel(false)
    , m_suspendUpdates(false)
{
    init(selectionModel);
}

/**
 */
ListView::ListView(ListViewInterface::SelectionModel_e selectionModel)
    : m_guiListId(BADGUIID)
    , m_ownsModel(false)
    , m_suspendUpdates(false)
{
    init(selectionModel);
}

/**
 */
ListView::~ListView()
{
    suspendUpdates();

    if (m_model.get())
        m_model->removeObserver(*this);
}

/**
 */
void ListView::init(ListViewInterface::SelectionModel_e selectionModel)
{
    // Create the correct selection model
    if (selectionModel == SingleSelect)
        m_selectionModel.reset(new ListSingleSelectionModel(*this));
    else
        m_selectionModel.reset(new ListMultiSelectionModel(*this));
}

void ListView::setModel(ListModelPtr_t model)
{
    if (m_model.get() != model.get())
        resetModel(model);
}

/**
 */
void ListView::resetModel(ListModelPtr_t model)
{
    if (m_model.get())
        m_model->removeObserver(*this);

    m_model = model;
    m_model->addObserver(*this);
    rebuildList();
}

/**
 */
ListModelGenericInterface& ListView::model()
{
    INVARIANT(VALID_REF(m_model.get()));
    return *m_model;
}

/**
 */
ListSelectionModelInterface& ListView::selectionModel()
{
    INVARIANT(VALID_REF(m_selectionModel.get()));
    return *m_selectionModel;
}

/**
 */
LgIndex_t ListView::guiId() const
{
    return m_guiListId;
}

/**
 */
void ListView::setGuiID(LgIndex_t listId)
{
    REQUIRE(listId != BADGUIID);

    if (m_guiListId != listId)
    {
        m_guiListId = listId;
        rebuildList();
    }
}

/**
 */
void ListView::modelChanged(const ListModelGenericInterface::ChangeInfo_s& info)
{
    REQUIRE("VALID_ENUM(info.change, ListModelGenericInterface::Change_e)");
    switch (info.change)
    {
        case ListModelGenericInterface::ModelReset:
        {
            rebuildList();
        } break;
        case ListModelGenericInterface::ItemsAdded:
        {
            addItems(info.changedindices);
        } break;
        case ListModelGenericInterface::ItemsDeleted:
        {
            deleteItems(info.changedindices);
        } break;
        case ListModelGenericInterface::ItemsChanged:
        {
            updateItems(info.changedindices);
        } break;
        default:
            CHECK(FALSE);
    }
}

/**
 */
void ListView::resumeUpdates()
{
    INVARIANT(VALID_REF(m_model.get()));

    if (m_suspendUpdates)
    {
        m_model->addObserver(*this);
        m_suspendUpdates = false;
        rebuildList();
    }
}

/**
 */
void ListView::suspendUpdates()
{
    INVARIANT(VALID_REF(m_model.get()));

    m_model->removeObserver(*this);
    m_suspendUpdates = true;
}

/**
 */
void ListView::rebuildList()
{
    if (m_guiListId != BADGUIID &&
        m_model.get() != NULL &&
        !m_suspendUpdates)
    {
        Lock lockStart;

        if (m_model->size() == 0)
        {
            displayEmptyModelMessage();
        }
        else
        {
            TecGUISetSensitivity(m_guiListId, TRUE);

            /* Get the previous selections so we can restore them later */
            IndexList_t previousSelections = m_selectionModel->getSelectedItems();

            TecGUIListDeleteAllItems(m_guiListId);

            for (Index_t curItem = 0; curItem < m_model->size(); ++curItem)
                TecGUIListAppendItem(m_guiListId, m_model->displayString(curItem).c_str());

            m_selectionModel->setSelectedItems(previousSelections);
        }
    }
}

/**
 */
void ListView::displayEmptyModelMessage()
{
    if (m_guiListId != BADGUIID)
    {
        TecGUIListDeleteAllItems(m_guiListId);
        TecGUIListAppendItem(m_guiListId, m_model->emptyModelMessage().c_str());
        TecGUISetSensitivity(m_guiListId, FALSE);
    }
}

/**
 */
void ListView::addItems(const IndexList_t &items)
{
    INVARIANT(VALID_REF(m_model.get()));
    INVARIANT(!m_suspendUpdates);

    if (m_guiListId != BADGUIID)
    {
        Lock lockStart;

        /*
         * These could be the first items added to an empty model. In that case,
         * the list will just have the "Empty model" string. rebuildList() will take
         * care of this situation.
         */
        if (items.size() == m_model->size()) // The new items are the only items in the model, meaning it was empty before
        {
            rebuildList();
        }
        else
        {
            /*
             * TecGUIListXXX only allows appending to the end of the list. So if the item was
             * added at the end, we can just append. Otherwise, we rebuild the whole list.
             */
            Index_t numCurItems = static_cast<Index_t>(TecGUIListGetItemCount(m_guiListId));
            if (find_if(items.begin(), items.end(),
                        bind2nd(less<Index_t>(), numCurItems)) == items.end())
            {
                IndexList_t::const_iterator itemIter = items.begin();
                while (itemIter != items.end())
                {
                    TecGUIListAppendItem(m_guiListId,
                                         m_model->displayString(*itemIter).c_str());
                    ++itemIter;
                }
            }
            else
            {
                rebuildList();
            }
        }
    }
}

/**
 */
void ListView::deleteItems(const IndexList_t &items)
{
    INVARIANT(VALID_REF(m_model.get()));
    INVARIANT(!m_suspendUpdates);

    if (m_guiListId != BADGUIID)
    {
        if (m_model->size() == 0)
        {
            displayEmptyModelMessage();
        }
        else
        {
            /* Get the previous selections so we can restore them later */
            IndexList_t previousSelections = m_selectionModel->getSelectedItems();

            /*
             * Delete items in reverse order so indices into TecGUI calls remain valid.
             */
            IndexList_t copyItems(items);
            std::sort(copyItems.begin(), copyItems.end());

            Lock lockStart;
            IndexList_t::reverse_iterator itemIter = copyItems.rbegin();
            while (itemIter != copyItems.rend())
            {
                TecGUIListDeleteItemAtPos(m_guiListId,
                                          static_cast<LgIndex_t>(*itemIter + 1));
                ++itemIter;
            }
            m_selectionModel->setSelectedItems(previousSelections);
        }
    }
}

/**
 */
void ListView::updateItems(const IndexList_t &items)
{
    INVARIANT(VALID_REF(m_model.get()));
    INVARIANT(!m_suspendUpdates);

    if (m_guiListId != BADGUIID)
    {
        Lock lockStart;

        /* Get the previous selections so we can restore them later */
        IndexList_t previousSelections = m_selectionModel->getSelectedItems();

        IndexList_t::const_iterator itemIter = items.begin();
        while (itemIter != items.end())
        {
            TecGUIListReplaceItem(m_guiListId,
                                  m_model->displayString(*itemIter).c_str(),
                                  static_cast<LgIndex_t>(*itemIter + 1));
            ++itemIter;
        }

        m_selectionModel->setSelectedItems(previousSelections);
    }
}
