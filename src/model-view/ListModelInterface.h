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
#ifndef ListModelInterface_h_ /* Only include once */
#define ListModelInterface_h_

#include <string>
#include "TECADDON.h"
#include "ListModelViewFwd.h"
#include "ListModelGenericInterface.h"
#include "ListModelViewTypedefs.h"

/**
 * This is a model that viewers can observe if they need to maintain a view
 * of items that stays up-to-date as the model changes.
 */
template<typename T>
class ListModelInterface : public virtual ListModelGenericInterface
{
public:
    /**
     * Get the data item associated with the given index. For example, in a list
     * of zones, the model would provide a zone number (EntIndex_t).
     */
    //virtual const T& data(Index_t index) const = 0;
    virtual T data(Index_t index) const = 0;

    /**
     * Set the data item associated with the given index.
     */
    virtual void setData(Index_t  index,
                         const T& data) = 0;

    /**
     * Get the index into the model for the given data. Returns end()
     * if the data is not found in the model.
     */
    virtual Index_t indexForData(const T& data) const = 0;

    /**
     * Insert new items into the model. Item(s) will be inserted before the given index.
     * Specify end() to insert items at the end of the model.
     */
    virtual void insertItem(Index_t  index,
                            const T& data) = 0;
    virtual void insertItems(Index_t               index,
                             const std::vector<T>& data) = 0;

    /**
     * Remove a single item from the model.
     */
    virtual void removeItem(Index_t index) = 0;

    /**
     * Remove "count" items from the model starting at "index."
     */
    virtual void removeItems(Index_t index,
                             Index_t count) = 0;

    virtual ~ListModelInterface() {}
};

#endif
