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
#ifndef ListModelGenericInterface_h_ /* Only include once */
#define ListModelGenericInterface_h_

#include <string>
#include "ListModelViewTypedefs.h"

class ListModelObserverInterface;

/*
 * This is a non-type-dependent interface for ListModel classes.
 * Its purpose is for use with objects that don't interact with the
 * actual data in the model, such as View classes (which only request
 * string representations of the data).
 */
class ListModelGenericInterface
{
public:
    enum Change_e
    {
        ItemsAdded,
        ItemsDeleted,
        ItemsChanged,
        ModelReset,
    };

    struct ChangeInfo_s
    {
        Change_e      change;
        IndexList_t   changedindices;

        explicit ChangeInfo_s(Change_e change)
            : change(change)
        {}

        bool operator==(ChangeInfo_s const& rhs) const
        {
            return change == rhs.change &&
                   changedindices == rhs.changedindices;
        }
    };

    /**
     * Add/remove observers to be notified when the model changes.
     */
    virtual void addObserver(ListModelObserverInterface& observer) = 0;
    virtual void removeObserver(ListModelObserverInterface& observer) = 0;

    /**
     * Get the number of items available in the model.
     */
    virtual Index_t size() const = 0;

    /**
     * Return an index one past the end of the model size. Used for searching
     * and insertion of data in the model.
     */
    virtual Index_t end() const = 0;

    /**
     * Get a string appropriate for display in a view for the given
     * index into the model. indices are 0-based.
     */
    virtual std::string displayString(Index_t index) const = 0;

    /**
     * Get a message appropriate for displaying in the view when there
     * are no items available in the model. For example, in a zone list
     * model, an appropriate message would be "No zones available."
     */
    virtual std::string emptyModelMessage() const = 0;

    virtual ~ListModelGenericInterface() {}
};

#endif
