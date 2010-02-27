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
#include "ZoneListModel.h"
#include <sstream>
#include <functional>
#include "StateChangeListener.h"

using std::tr1::bind;
using std::tr1::mem_fn;

#if defined(_MSC_VER)
# pragma warning (push)
# pragma warning (disable: 4355) // 'this' : used in base member initializer list
#endif

ZoneListModel::ZoneListModel(TecUtilDataSetInterfacePtr_t tecUtilDataSet,
                             StateChangeNotifierPtr_t stateChangeNotifier)
    : m_tecUtilDataSet(tecUtilDataSet)
    , m_stateChangeNotifier(stateChangeNotifier)
    , m_stateChangeListener(new StateChangeListener(bind(&ZoneListModel::stateChangeCallback, this, _1, _2),
                                                    *stateChangeNotifier))
    , m_enabledZones()
{
    rebuildModel();
}

ZoneListModel::~ZoneListModel()
{
}

Index_t ZoneListModel::size() const
{
    return m_tecUtilDataSet->numZones();
}

std::string ZoneListModel::displayString(Index_t index) const
{
    EntIndex_t zoneNumber = data(index);
    std::string zoneName = m_tecUtilDataSet->zoneName(zoneNumber);

    return buildDisplayString(zoneNumber, zoneName);
}

EntIndex_t ZoneListModel::data(Index_t index) const
{
    REQUIRE(index <= m_enabledZones.size());

    return m_enabledZones.at(index);
}

void ZoneListModel::rebuildModel()
{
    m_enabledZones = m_tecUtilDataSet->enabledZones();
}

std::string ZoneListModel::buildDisplayString(EntIndex_t zoneNumber,
                                              std::string const& zoneName) const
{
    std::ostringstream buffer;
    buffer << zoneNumber << " : " << zoneName;
    return buffer.str();
}

void ZoneListModel::stateChangeCallback(const tecplot::toolbox::StateChangeEventInterface& /*stateChangeEvent*/,
                                        tecplot::toolbox::StateChangeNotifierInterface& /*source*/)
{
    rebuildModel();
    ListModelGenericInterface::ChangeInfo_s changeInfo(ModelReset);
    notifyOfChange(changeInfo);
}

#if defined(_MSC_VER)
# pragma warning(pop)
#endif
