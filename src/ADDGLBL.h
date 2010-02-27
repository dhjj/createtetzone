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
#ifndef ADDGLBL_H_ /* Only include once */
#define ADDGLBL_H_

#include <list>
#include <vector>
#include "ADDONVER.h"
#include "TECADDON.h" // for definition of AddOn_pa

extern AddOn_pa AddOnID;

typedef std::list<EntIndex_t> ZoneList_t;
typedef std::vector<EntIndex_t> VariableList_t;

#if 0
#ifdef _MSC_VER
#pragma warning (push)
// Copy these to source files as needed
#pragma warning (disable : 4267) // conversion from 'size_t' to 'LgIndex_t', possible loss of data
#pragma warning (disable : 4244) // conversion from 'SetIndex_t' to 'EntIndex_t', possible loss of data
/* These disable warnings caused by boost::lexical cast */
#pragma warning (disable : 4127) // conditional expression is constant
#pragma warning (disable : 4701) // potentially uninitialized local variable 'result' used
/* This disables warnings caused by boost/algorithm/string.hpp */
#pragma warning (disable : 4512) // assignment operator could not be generated
#endif

#ifdef _MSC_VER
#pragma warning (pop)
#endif

#ifdef _MSC_VER
#endif
#endif

#endif /* ADDGLBL_H_ */
