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
#ifndef ListModelAbstract_h_ /* Only include once */
#define ListModelAbstract_h_

#include <set>
#include <vector>
#include "TECADDON.h"
#include "ListModelInterface.h"
#include "ListModelObserverInterface.h"

/**
 * This abstract class handles the basics of a ListModel, including management of
 * observers (adding, removing), default empty model message, and disabling (via
 * assertions) the editable features of the model (i.e. insert/remove).
 */
template<typename T>
class ListModelAbstract : public ListModelInterface<T>
{
public:
    ListModelAbstract();
    virtual ~ListModelAbstract();

    /**
     * Partial implementation of ListModelInterface. If the subclass accepts all of the
     * partial implementation, it only needs to implement:
     *  - virtual Index_t size() const; 
     *  - virtual std::string displayString(Index_t index) const;
     *  - virtual const T& data(Index_t index) const;
     *  - virtual void rebuildModel();
     *
     * Default versions of the insert/remove and setData methods will assert, so
     * non-editable models need not implement these. Default indexForData() performs a
     * linear search for the item; override this if the model can provide a faster search
     * or operator== is not an appropriate comparison operator.
     */
    virtual void addObserver(ListModelObserverInterface& observer);
    virtual void removeObserver(ListModelObserverInterface& observer);
    virtual Index_t end() const;
    virtual std::string emptyModelMessage() const
    {
        return "No items available.";
    }
    virtual void insertItem(Index_t,
                            const T&)
    {
        CHECK(false);
    }
    virtual void insertItems(Index_t,
                             const std::vector<T>&)
    {
        CHECK(false);
    }
    virtual void removeItem(Index_t)
    {
        CHECK(false);
    }
    virtual void removeItems(Index_t,
                             Index_t)
    {
        CHECK(false);
    }
    virtual void setData(Index_t,
                         const T&)
    {
        CHECK(false);
    }
    virtual Index_t indexForData(const T& data) const;

protected:
    /**
     */
    void notifyOfChange(const ListModelGenericInterface::ChangeInfo_s& changeInfo);

private:
    /*
     * The set of observers watching.
     */
    typedef std::set<ListModelObserverInterface*> ObserverSet_t;
    ObserverSet_t m_observers;
};

/**
 */
template<typename T>
ListModelAbstract<T>::ListModelAbstract()
{
}

/**
 */
template<typename T>
ListModelAbstract<T>::~ListModelAbstract()
{
}

/**
 */
template<typename T>
void ListModelAbstract<T>::addObserver(ListModelObserverInterface& observer)
{
    m_observers.insert(&observer);
}

/**
 */
template<typename T>
void ListModelAbstract<T>::removeObserver(ListModelObserverInterface& observer)
{
    m_observers.erase(&observer);
}

/**
 */
template<typename T>
Index_t ListModelAbstract<T>::end() const
{
    return this->size() + 1;
}

/**
 */
template<typename T>
void ListModelAbstract<T>::notifyOfChange(ListModelGenericInterface::ChangeInfo_s const& changeInfo)
{
    ObserverSet_t::iterator observerIter = m_observers.begin();
    while (observerIter != m_observers.end())
    {
        ListModelObserverInterface *observer = *observerIter;
        observer->modelChanged(changeInfo);
        ++observerIter;
    }
}

/**
 */
template<typename T>
Index_t ListModelAbstract<T>::indexForData(const T& data) const
{
    Index_t index = 0;
    while (index < this->size())
    {
        if (data == this->data(index))
            return index;
    }
    return end();
}

#endif
