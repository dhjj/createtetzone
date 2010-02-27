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
#ifndef FieldDataBlockCopier_h__
#define FieldDataBlockCopier_h__

#include "FieldDataCopierAbstract.h"

/**
 * Class responsible for copying field data for the given variable from the source zone
 * specified in the constructor to the destination zone as a block (i.e. by calling
 * TecUtilDataValueArrayGet and SetByRef). The source zone and the destination zone must
 * be compatible, and the convenience function, zonesAreCompatibleForBlockCopy, is
 * provided to do this check.
 */
class FieldDataBlockCopier
    : public FieldDataCopierAbstract
{
public:
    FieldDataBlockCopier(std::auto_ptr<FieldDataCopierAbstract> nextCopier,
                         EntIndex_t sourceZone);
private:
    virtual LgIndex_t copyFieldData(LgIndex_t offset,
                                    EntIndex_t destZone,
                                    EntIndex_t var);
    size_t getSizeOfType(FieldDataType_e dataType) const;
    EntIndex_t m_sourceZone;
};

/**
 * Returns true if the given source and destination zones are compatible for a block copy
 * of data for the given variable.
 */
bool zonesAreCompatibleForBlockCopy(EntIndex_t sourceZone,
                                    EntIndex_t destZone,
                                    EntIndex_t var);

#endif // FieldDataBlockCopier_h__
