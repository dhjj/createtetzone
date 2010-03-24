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
#ifndef ZoneListModel_h__
#define ZoneListModel_h__

#include <memory>
#include <set>
#include "ListModelAbstract.h"
#include "toolbox/ToolboxFwd.h"

class StateChangeListener;

class TecUtilDataSetInterface
{
public:
    typedef std::vector<EntIndex_t> ZoneList_t;

    virtual EntIndex_t numZones() const = 0;
    virtual std::string zoneName(EntIndex_t zoneNumber) const = 0;
    virtual ZoneList_t enabledZones() const = 0;

    virtual ~TecUtilDataSetInterface() {}
};

class ZoneListModel
    : public ListModelAbstract<EntIndex_t>
{
public:
    typedef std::tr1::shared_ptr<TecUtilDataSetInterface> TecUtilDataSetInterfacePtr_t;
    typedef std::tr1::shared_ptr<tecplot::toolbox::StateChangeNotifierInterface> StateChangeNotifierPtr_t;

    ZoneListModel(TecUtilDataSetInterfacePtr_t tecUtilDataSet,
                  StateChangeNotifierPtr_t stateChangeNotifier);

    ~ZoneListModel();

    /**
     * Implementation of ListModel(Generic)Interface
     */
    virtual Index_t size() const;

    virtual std::string displayString(Index_t index) const;

    EntIndex_t data(Index_t index) const;

    void stateChangeCallback(const tecplot::toolbox::StateChangeEventInterface& stateChangeEvent,
        tecplot::toolbox::StateChangeNotifierInterface&    source);

protected:
    virtual void rebuildModel();
    std::string buildDisplayString(EntIndex_t zoneNumber, std::string const& zoneName) const;

private:
    TecUtilDataSetInterfacePtr_t m_tecUtilDataSet;
    StateChangeNotifierPtr_t m_stateChangeNotifier;

    typedef std::auto_ptr<StateChangeListener> StateChangeListenerPtr_t;
    StateChangeListenerPtr_t m_stateChangeListener;

    TecUtilDataSetInterface::ZoneList_t m_enabledZones;
};

#endif // ZoneListModel_h__
