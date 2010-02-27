#include "ListSelectionModelAbstract.h"
#if 0
#include <boost/mem_fn.hpp>
#include <boost/bind.hpp>
using namespace boost;
#endif // 0
#include "ListSelectionModelObserverInterface.h"
#include "Lock.h"


/**
 */
ListSelectionModelAbstract::ListSelectionModelAbstract(ListViewInterface& parentView) :
        m_parentView(parentView)
{
}

/**
 */
ListViewInterface& ListSelectionModelAbstract::parentView() const
{
    return m_parentView;
}

/**
 */
IndexList_t ListSelectionModelAbstract::getSelectedItems() const
{
    INVARIANT(parentView().guiId() != BADGUIID);

    IndexList_t selectedItems;

    LgIndex_t  *selectedArray   = NULL;
    LgIndex_t   selectedCount   = 0;

    Lock lockStart;
    try
    {
        TecGUIListGetSelectedItems(parentView().guiId(),
                                   &selectedArray,
                                   &selectedCount);

        selectedItems.assign(selectedArray,
                             selectedArray + selectedCount);
        if (selectedArray != NULL)
            TecUtilArrayDealloc((void**)&selectedArray);

        // Translate from 1-based to 0-based
        std::transform(selectedItems.begin(), selectedItems.end(),
                       selectedItems.begin(),
                       std::bind2nd(std::minus<Index_t>(),
                                    Index_t(1)));
        return selectedItems;
    }
    catch (...)
    {
        if (selectedArray != NULL)
            TecUtilArrayDealloc((void**)&selectedArray);
        throw;
    }
}

/**
 * Return -1 if no items selected or more than a single item
 * is selected.
 */
LgIndex_t ListSelectionModelAbstract::getSelectedItem() const
{
    INVARIANT(parentView().guiId() != BADGUIID);

    IndexList_t selectedItems = getSelectedItems();
    if (selectedItems.empty() ||
        selectedItems.size() > 1)
        return -1;
    else
        return static_cast<LgIndex_t>(selectedItems.front());
}

/**
 */
void ListSelectionModelAbstract::setSelectedItem(LgIndex_t selectedItem) const
{
    INVARIANT(parentView().guiId() != BADGUIID);
    REQUIRE(selectedItem >= 0);

    IndexList_t selectedItems(1, selectedItem);

    if (ListIndexIsValid(parentView().guiId(),
                         selectedItem))
    {
        ++selectedItem; // 0-based to 1-based
        TecGUIListSetSelectedItem(parentView().guiId(),
                                  selectedItem);
    }
    else
    {
        TecGUIListDeselectAllItems(parentView().guiId());
    }

    notifyOfChange();
}

/**
 */
void ListSelectionModelAbstract::addObserver(ListSelectionModelObserverInterface& observer)
{
    m_observers.insert(&observer);
}

/**
 */
void ListSelectionModelAbstract::removeObserver(ListSelectionModelObserverInterface& observer)
{
    m_observers.erase(&observer);
}


/**
 */
void ListSelectionModelAbstract::notifyOfChange() const
{
    if (!m_observers.empty())
    {
        IndexList_t selectedItems = getSelectedItems();
#if 0
        std::for_each(m_observers.begin(),
                      m_observers.end(),
                      bind(mem_fn(&ListSelectionModelObserverInterface::selectionChanged), _1, selectedItems));
#endif // 0
        for (ObserverSet_t::const_iterator observer = m_observers.begin(); observer != m_observers.end(); ++observer)
            (*observer)->selectionChanged(selectedItems);
    }
}

/**
 */
void ListSelectionModelAbstract::selectionChanged()
{
    notifyOfChange();
}
