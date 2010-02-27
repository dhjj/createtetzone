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
#ifndef ListSelectionModelInterface_h_ /* Only include once */
#define ListSelectionModelInterface_h_

#include "TECADDON.h"
#include "ListModelViewTypedefs.h"
#include "ListModelViewFwd.h"

class ListSelectionModelInterface
{
public:
    /**
     * Add/remove observers to be notified when the model changes.
     */
    virtual void addObserver(ListSelectionModelObserverInterface& observer) = 0;
    virtual void removeObserver(ListSelectionModelObserverInterface& observer) = 0;

    /**
     * Because we can't directly intercept TGB callbacks for selection changes,
     * this method allows the callback owner to forward the callback to us.
     */
    virtual void selectionChanged() = 0;

    /**
     * Get a list of all selected indices. Indices are zero-based.
     */
    virtual IndexList_t getSelectedItems() const = 0;

    /**
     * Set the list of all selected indices. indices are zero-based.
     */
    virtual void setSelectedItems(const IndexList_t& selectedItems) const = 0;

    /**
     * Returns the zero-based index of the selected item.
     * If no item OR multiple items are selected, return -1.
     */
    virtual LgIndex_t getSelectedItem() const = 0;

    /**
     * Set the zero-based index as the only selected item in the list.
     */
    virtual void setSelectedItem(LgIndex_t selectedItem) const = 0;

    /**
     */
    virtual ~ListSelectionModelInterface() {}
};

#endif // ListSelectionModelInterface_h_
