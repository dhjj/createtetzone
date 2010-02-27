#include "ListSingleSelectionModel.h"
#include "ListViewInterface.h"

/**
 */
ListSingleSelectionModel::ListSingleSelectionModel(ListViewInterface& parentView) :
        ListSelectionModelAbstract(parentView)
{
}

/**
 */
void ListSingleSelectionModel::setSelectedItems(const IndexList_t& selectedItems) const
{
    TecGUIListDeselectAllItems(parentView().guiId());

    // TGB Single selection lists don't support TecGUIListSetSelectedItems, so we use
    // TecGUIListSetSelectedItem with just the first valid index

    // Make a copy we can modify, and remove invalid indices
    IndexList_t copyItems(selectedItems);
    eraseInvalidItems(copyItems);

    if (!copyItems.empty())
    {
        LgIndex_t selectedItem = static_cast<LgIndex_t>(copyItems.front());
        ++selectedItem; // 0-based to 1-based
        TecGUIListSetSelectedItem(parentView().guiId(),
                                  selectedItem);
    }

    notifyOfChange();
}
