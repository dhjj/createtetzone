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
#ifndef ListModelPreMapped_h_ /* Only include once */
#define ListModelPreMapped_h_

#include <string>
#include <vector>
#include <algorithm>
#include "ListModelAbstract.h"

template<typename T>
struct PreMappedValue
{
    std::string displayString;
    T           value;
};

template<typename T>
class ListModelPreMapped : public ListModelAbstract<T>
{
public:
    ListModelPreMapped(PreMappedValue<T> const* staticDataArray,
                       size_t                   arraySize);

    /**
     * Implementation of ListModelInterface
     */
    virtual Index_t size() const;
    virtual std::string displayString(Index_t index) const;
    virtual const T& data(Index_t index) const;
    virtual Index_t indexForData(const T& data) const;

    /**
     * Implementation of ListModelAbstract pure virtuals
     */
    virtual void rebuildModel()
    {
        CHECK(false);
    } // Should never be called

private:
    typedef std::vector<PreMappedValue<T> > ValueList_t;
    ValueList_t m_data;
};

/**
 */
template<typename T>
ListModelPreMapped<T>::ListModelPreMapped(PreMappedValue<T> const* staticDataArray,
                                          size_t                   arraySize) :
        m_data(staticDataArray,
               staticDataArray + arraySize)
{
}

/**
 */
template<typename T>
Index_t ListModelPreMapped<T>::size() const
{
    return m_data.size();
}

/**
 */
template<typename T>
std::string ListModelPreMapped<T>::displayString(Index_t index) const
{
    return m_data.at(index).displayString;
}

/**
 */
template<typename T>
const T& ListModelPreMapped<T>::data(Index_t index) const
{
    return m_data.at(index).value;
}

/**
 */
template<typename T>
class matches_value
{
public:
    matches_value(const T& data) :
            m_data(data)
    {}

    bool operator()(const PreMappedValue<T>& data) const
    {
        return m_data == data.value;
    }

private:
    const T& m_data;
};

/**
 */
template<typename T>
Index_t ListModelPreMapped<T>::indexForData(const T& data) const
{
    typename ValueList_t::const_iterator iter = std::find_if(m_data.begin(),
                                                             m_data.end(),
                                                             matches_value<T>(data));
    if (iter != m_data.end())
        return std::distance(m_data.begin(), iter);
    else
        return ListModelPreMapped<T>::end();
}

#endif
