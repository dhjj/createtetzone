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
#ifndef ListView_h_ /* Only include once */
#define ListView_h_

#include <memory>
#include "TECADDON.h"
#include "ListModelViewFwd.h"
#include "ListViewInterface.h"
#include "ListModelGenericInterface.h"
#include "ListModelObserverInterface.h"

class ListView : public virtual ListModelObserverInterface, public virtual ListViewInterface
{
public:
    ListView(LgIndex_t                           guiListId,
             ListViewInterface::SelectionModel_e selectionModel);

    ListView(ListViewInterface::SelectionModel_e selectionModel);

    virtual ~ListView();

    /*
     * Implementation of ListViewInterface.
     */
    virtual void setModel(ListModelPtr_t model);
    //virtual void setModel(ListModelGenericInterface* model);
    //virtual void setModel(ListModelGenericInterface& model);
    virtual ListModelGenericInterface& model();
    virtual ListSelectionModelInterface& selectionModel();
    virtual LgIndex_t guiId() const;
    virtual void setGuiID(LgIndex_t listId);
    virtual void resumeUpdates();
    virtual void suspendUpdates();

    /*
     * Implementation of ListModelObserverInterface.
     */
    virtual void modelChanged(const ListModelGenericInterface::ChangeInfo_s& info);

private: // Member functions
    void init(ListViewInterface::SelectionModel_e selectionModel);
    void rebuildList();
    void addItems(const IndexList_t& items);
    void deleteItems(const IndexList_t& items);
    void updateItems(const IndexList_t& items);
    void resetModel(ListModelPtr_t model);
    void displayEmptyModelMessage();

private: // Member variables
    LgIndex_t      m_guiListId;
    ListModelPtr_t m_model;
    bool           m_ownsModel;
    bool           m_suspendUpdates;
    std::auto_ptr<ListSelectionModelInterface> m_selectionModel;

    ListView(const ListView& other);
    ListView& operator=(const ListView& other);
};

#endif
