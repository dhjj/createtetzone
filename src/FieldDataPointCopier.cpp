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
#include "FieldDataPointCopier.h"
#include "Lock.h"

FieldDataPointCopier::FieldDataPointCopier(std::auto_ptr<FieldDataCopierAbstract> nextCopier,
                                           EntIndex_t sourceZone)
    : FieldDataCopierAbstract(nextCopier)
    , m_sourceZone(sourceZone)
{
    REQUIRE(TecUtilZoneIsEnabled(sourceZone));
}

LgIndex_t FieldDataPointCopier::copyFieldData(LgIndex_t offset,
                                              EntIndex_t destZone,
                                              EntIndex_t var)
{
    INVARIANT(TecUtilZoneIsEnabled(m_sourceZone));
    REQUIRE(TecUtilZoneIsEnabled(destZone));
    REQUIRE(TecUtilVarIsEnabled(var));
    REQUIRE(offset >= 1 && offset < TecUtilDataValueGetCountByRef(TecUtilDataValueGetWritableNativeRef(destZone, var)));

    FieldData_pa destFieldData = TecUtilDataValueGetWritableNativeRef(destZone, var);
    FieldData_pa sourceFieldData = TecUtilDataValueGetReadableNLRef(m_sourceZone, var);
    LgIndex_t numSourceZonePoints = TecUtilDataValueGetCountByRef(sourceFieldData);
    for (LgIndex_t sourceZoneIndex = 1; sourceZoneIndex <= numSourceZonePoints; ++sourceZoneIndex, ++offset)
    {
        double value = TecUtilDataValueGetByRef(sourceFieldData, sourceZoneIndex);
        TecUtilDataValueSetByRef(destFieldData, offset, value);
    }
    return numSourceZonePoints;
}
