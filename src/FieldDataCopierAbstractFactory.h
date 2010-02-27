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
#ifndef FieldDataCopierAbstractFactory_h__
#define FieldDataCopierAbstractFactory_h__

#include <memory>
#include "ADDGLBL.h"
#include "FieldDataCopierInterface.h"

typedef std::tr1::shared_ptr<FieldDataCopierInterface> FieldDataCopierPtr_t;
#define FACTORY_USE_AUTOPTR

/**
 * Class responsible for creating the appropriate field data copier for the given source
 * and destination zones. The copier may be a single entity that sets up sharing between
 * the zones, or it may be a chain of copiers (one for each source zone).
 */
class FieldDataCopierAbstractFactory
{
public:
    /**
     * MSVC8 apparently has issues returning auto_ptrs in this manner. So we have to turn
     * this off until it can be further investigated.
     */
#if defined FACTORY_USE_AUTOPTR
    FieldDataCopierPtr_t createFieldDataCopier(ZoneList_t const& sourceZones,
                                               EntIndex_t destZone,
                                               EntIndex_t var) const;
#else
    FieldDataCopierInterface* createFieldDataCopier(ZoneList_t const& sourceZones,
                                                    EntIndex_t destZone,
                                                    EntIndex_t var) const;

#endif

private:
    FieldDataCopierPtr_t buildFieldDataCopierChain(ZoneList_t const& sourceZones,
                                                   EntIndex_t destZone,
                                                   EntIndex_t var) const;
};

#endif // FieldDataCopierAbstractFactory_h__
