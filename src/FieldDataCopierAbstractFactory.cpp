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
#include "FieldDataCopierAbstractFactory.h"
#include "FieldDataCopierInterface.h"
#include "FieldDataSharer.h"
#include "FieldDataBlockCopier.h"
#include "FieldDataPointCopier.h"
#include "FieldDataAllocator.h"

#if defined FACTORY_USE_AUTOPTR
FieldDataCopierPtr_t FieldDataCopierAbstractFactory::createFieldDataCopier(ZoneList_t const& sourceZones,
                                                                           EntIndex_t destZone,
                                                                           EntIndex_t var) const
#else
FieldDataCopierInterface* FieldDataCopierAbstractFactory::createFieldDataCopier(ZoneList_t const& sourceZones,
                                                                                EntIndex_t destZone,
                                                                                EntIndex_t var) const
#endif
{
    bool singleSourceZone = sourceZones.size() == 1;

    if (singleSourceZone &&
        TecUtilDataValueIsSharingOk(sourceZones.front(), destZone, var))
    {
        return FieldDataCopierPtr_t(new FieldDataSharer);
    }
    else
    {
        return buildFieldDataCopierChain(sourceZones,
                                         destZone,
                                         var)
#if !defined FACTORY_USE_AUTOPTR
               .release()
#endif
               ;
    }
}

FieldDataCopierPtr_t FieldDataCopierAbstractFactory::buildFieldDataCopierChain(ZoneList_t const& sourceZones,
                                                                               EntIndex_t destZone,
                                                                               EntIndex_t var) const
{
    std::auto_ptr<FieldDataCopierAbstract> headCopier;

    for (ZoneList_t::const_reverse_iterator sourceZone = sourceZones.rbegin(); sourceZone != sourceZones.rend(); ++sourceZone)
    {
        if (zonesAreCompatibleForBlockCopy(*sourceZone, destZone, var))
            headCopier.reset(new FieldDataBlockCopier(headCopier, *sourceZone));
        else
            headCopier.reset(new FieldDataPointCopier(headCopier, *sourceZone));
    }

    // The first item in the chain must be the one who allocates room in the destZone
    headCopier.reset(new FieldDataAllocator(headCopier));

    return FieldDataCopierPtr_t(headCopier);
}
