/*
 * Copyright (c) Tecplot, Inc.
 *
 * All rights reserved.  Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice, this list of
 *     conditions and the following disclaimer. 
 *   - Redistributions in binary form must reproduce the above copyright notice, this list
 *     of conditions and the following disclaimer in the documentation and/or other
 *     materials provided with the distribution.
 *   - Neither the name of the Tecplot, Inc., nor the names of its contributors may be
 *     used to endorse or promote products derived from this software without specific
 *     prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef ListSelectionModelAbstract_h_ /* Only include once */
#define ListSelectionModelAbstract_h_

#include <set>
#include <functional>
#include <algorithm>
#include "ListModelViewFwd.h"
#include "ListSelectionModelInterface.h"
#include "ListViewInterface.h"

class ListSelectionModelAbstract : public virtual ListSelectionModelInterface
{
public:
    /**
     * Constructor takes a reference to the view that is the parent of
     * this object.
     */
    ListSelectionModelAbstract(ListViewInterface& parentView);
    virtual ~ListSelectionModelAbstract() {}

    /**
     * Partial implementation of ListSelectionModelInterface.
     */
    virtual IndexList_t getSelectedItems() const;
    virtual LgIndex_t getSelectedItem() const;
    virtual void setSelectedItem(LgIndex_t selectedItem) const;
    virtual void addObserver(ListSelectionModelObserverInterface& observer);
    virtual void removeObserver(ListSelectionModelObserverInterface& observer);
    virtual void selectionChanged();

protected:
    ListViewInterface& parentView() const;

    /**
     * Derived classes can call this to notify all observers of a change.
     */
    void notifyOfChange() const;

    // Erase items that are not valid selections from the selection list
    template<typename T>
    void eraseInvalidItems(std::vector<T>& selectedItems) const;

private:
    typedef std::set<ListSelectionModelObserverInterface*> ObserverSet_t;
    ObserverSet_t      m_observers;
    ListViewInterface& m_parentView;

    ListSelectionModelAbstract(const ListSelectionModelAbstract& other);
    ListSelectionModelAbstract& operator=(const ListSelectionModelAbstract& other);
};


/**
 */
inline bool ListIndexIsValid(LgIndex_t listId,
                             LgIndex_t index) // 0-based index
{
    LgIndex_t listSize = TecGUIListGetItemCount(listId);
    return index >= 0 &&
           index < listSize;
}

/**
 */
template<typename T>
class ListIndexIsValidObj : public std::unary_function<T, bool>
{
public:
    ListIndexIsValidObj(LgIndex_t listId) :
            m_listId(listId)
    {}

    bool operator()(T index) const  // 0-based index
    {
        return ListIndexIsValid(m_listId,
                                static_cast<LgIndex_t>(index));
    }

private:
    LgIndex_t m_listId;
};

/**
 */
template<typename T>
void ListSelectionModelAbstract::eraseInvalidItems(std::vector<T>& selectedItems) const
{
    // Erase items that are not valid indices
    selectedItems.erase(std::remove_if(selectedItems.begin(),
                                       selectedItems.end(),
                                       std::not1(ListIndexIsValidObj<T>(parentView().guiId()))),
                        selectedItems.end());
}

#endif // ListSelectionModelAbstract_h_
