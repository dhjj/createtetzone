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
#ifndef ListViewInterface_h_ /* Only include once */
#define ListViewInterface_h_

#include <memory>
#include "TECADDON.h"
#include "ListModelViewFwd.h"

class ListViewInterface
{
public:
    enum SelectionModel_e
    {
        SingleSelect,
        MultiSelect
    };

    typedef std::tr1::shared_ptr<ListModelGenericInterface> ListModelPtr_t;

    /**
     * Set a new model for the view. The view object DOES NOT take ownership of the model object.
     */
    virtual void setModel(ListModelPtr_t model) = 0;

    /**
     * Set a new model for the view. The view object takes ownership of the model object, and
     * it will be deleted when the view object is destructed or a new model is provided.
     */
    //virtual void setModel(ListModelGenericInterface* model) = 0;

    /**
     * Set a new model for the view. The view object DOES NOT take ownership of the model object.
     */
    //virtual void setModel(ListModelGenericInterface& model) = 0;

    /**
     * Get a reference to the lists current model.
     */
    virtual ListModelGenericInterface& model() = 0;

    /**
     * Get a reference to the current selection model.
     */
    virtual ListSelectionModelInterface& selectionModel() = 0;

    /**
     * Get/set the GUI id of the list control to which the view object will maintain.
     * The given ID must be a valid list control ID.
     */
    virtual LgIndex_t guiId() const = 0;
    virtual void setGuiID(LgIndex_t listId) = 0;

    /**
     * Tell the view object to suspend updates to the GUI. Should be called, for example,
     * when the dialog containing the GUI list is dropped. Updates will not be resumed
     * until resumeUpdates is called, even if setListId or setModel is called in between.
     */
    virtual void suspendUpdates() = 0;

    /**
     * Tell the view objects to resume updates to the GUI.
     */
    virtual void resumeUpdates() = 0;

    /**
     */
    virtual ~ListViewInterface() {}
};

#endif
