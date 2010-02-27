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
#ifndef StateChangeListener_h
#define StateChangeListener_h

#include <functional>
#include "toolbox/ToolboxFwd.h"
#include "toolbox/StateChangeListenerInterface.h"
#include "toolbox/Uncopyable.h"

/**
 * StateChangeListener is a convenience class to which you can delegate the
 * responsibilities involved in listening to state changes. This has the benefit of
 * keeping your code more cohesive and removing the typical burden of listening to state
 * changes (either implementing StateChangeListenerInterface yourself or as an inner
 * class).
 */
class StateChangeListener
    : public virtual tecplot::toolbox::StateChangeListenerInterface
{
public:
    typedef std::tr1::function< void (const tecplot::toolbox::StateChangeEventInterface& stateChangeEvent,
                                      tecplot::toolbox::StateChangeNotifierInterface&    source) > StateChangeCallback;

    /**
     * Create a StateChangeListener instance.
     *
     * @param stateChangeCallback
     *  A function to be called when a state change occurs.
     *
     * @param notifier
     *  A reference to the state change notification object with which the
     *  StateChangeListener will register itself. The StateChangeListener will immediately
     *  register itself with this notification object, and will unregistered itself upon
     *  destruction.
     */
    StateChangeListener(StateChangeCallback stateChangeCallback,
                        tecplot::toolbox::StateChangeNotifierInterface& notifier);

    /**
     */
    ~StateChangeListener();

    /**
     * Instruct the StateChangeListener to start listening to state changes (which will
     * then be forwarded to the parent class). Note that the StateChangeListener will
     * immediately begin listening to state changes upon construction, so there is no need
     * to call this method unless you have previously called stopListening.
     */
    void startListening();

    /**
     * Instruct the StateChangeListener to stop listening to state changes. Note that the
     * StateChangeListener will automatically unregister itself as a listener upon
     * destruction. The StateChangeListener will also automatically unregister itself if
     * it receives a state change of type StateChange_QuitTecplot, as further calls into
     * the TecUtil layer are unsafe after such an event.
     */
    void stopListening();

    /*
     * Implementation of StateChangeListenerInterface
     */
    virtual void stateChanged(const tecplot::toolbox::StateChangeEventInterface& stateChangeEvent,
                              tecplot::toolbox::StateChangeNotifierInterface&    source);

private:
    UNCOPYABLE_CLASS(StateChangeListener);

    StateChangeCallback m_stateChangeCallback;
    tecplot::toolbox::StateChangeNotifierInterface& m_notifier;
};

#endif //StateChangeListener_h
