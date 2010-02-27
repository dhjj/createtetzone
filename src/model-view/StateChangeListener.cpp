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
#include "StateChangeListener.h"
#include "toolbox/StateChangeEventInterface.h"
#include "toolbox/StateChangeNotifierInterface.h"
#include "Lock.h"

StateChangeListener::StateChangeListener(StateChangeCallback stateChangeCallback,
                                         tecplot::toolbox::StateChangeNotifierInterface& notifier)
                                         : m_stateChangeCallback(stateChangeCallback)
                                         , m_notifier(notifier)
{
    startListening();
}

StateChangeListener::~StateChangeListener()
{
    stopListening();
}

void StateChangeListener::startListening()
{
    Lock lock;
    m_notifier.addListener(this);
}

void StateChangeListener::stopListening()
{
    Lock lock;
    m_notifier.removeListener(this);
}

void StateChangeListener::stateChanged(const tecplot::toolbox::StateChangeEventInterface& stateChangeEvent,
                                       tecplot::toolbox::StateChangeNotifierInterface&    source)
{
    if (stateChangeEvent.getStateChange() == StateChange_QuitTecplot)
        stopListening();

    m_stateChangeCallback(stateChangeEvent, source);
}
