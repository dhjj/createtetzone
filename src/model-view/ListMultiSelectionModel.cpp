#include "ListMultiSelectionModel.h"
#include <algorithm>
#include <functional>
#include "ListViewInterface.h"

/**
 */
ListMultiSelectionModel::ListMultiSelectionModel(ListViewInterface& parentView) :
        ListSelectionModelAbstract(parentView)
{
}

/**
 */
void ListMultiSelectionModel::setSelectedItems(const IndexList_t& selectedItems) const
{
    TecGUIListDeselectAllItems(parentView().guiId());

    // Make a copy of the items we can modify. Because Index_t and LgIndex_t are different
    // types, can't use vector copy constructor or vector.assign.
    std::vector<LgIndex_t> copyItems;
    copyItems.reserve(selectedItems.size());
    for (IndexList_t::const_iterator ii = selectedItems.begin(); ii != selectedItems.end(); ++ii)
        copyItems.push_back(static_cast<LgIndex_t>(*ii));

    eraseInvalidItems(copyItems);
    if (!copyItems.empty())
    {
        // Transform 0-based to 1-based
        std::transform(copyItems.begin(), copyItems.end(),
                       copyItems.begin(),
                       std::bind2nd(std::plus<LgIndex_t>(),
                                    LgIndex_t(1)));

        TecGUIListSetSelectedItems(parentView().guiId(),
                                   &copyItems[0],
                                   static_cast<LgIndex_t>(copyItems.size()));
    }

    notifyOfChange();
}
