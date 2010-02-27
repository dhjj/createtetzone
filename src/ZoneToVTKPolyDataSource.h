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
#ifndef ZoneToVTKPolyDataSource_h__
#define ZoneToVTKPolyDataSource_h__

#include <list>
#include "vtkPolyDataAlgorithm.h"
#include "TECADDON.h"

/*
 * Class that can be a source in a VTK data pipeline. Takes a Tecplot source zone and
 * outputs it as a vtkPolyDataSet of points. The following limitations apply:
 * - Only the spatial variable data is copied to the output data set
 */
class ZoneToVTKPolyDataSource
    : public vtkPolyDataAlgorithm
{
public:
    static ZoneToVTKPolyDataSource* New();

    typedef std::list<EntIndex_t> ZoneList_t;

    void setSourceZones(ZoneList_t sourceZones);

    vtkTypeMacro(ZoneToVTKPolyDataSource, vtkPolyDataAlgorithm);

protected:
    virtual int RequestData(vtkInformation*        request,
                            vtkInformationVector** inputVector,
                            vtkInformationVector*  outputVector);

    ZoneToVTKPolyDataSource();
    virtual ~ZoneToVTKPolyDataSource();

private:
    ZoneList_t m_sourceZones;

    ZoneToVTKPolyDataSource(ZoneToVTKPolyDataSource const&);
    ZoneToVTKPolyDataSource operator=(ZoneToVTKPolyDataSource const&);
    LgIndex_t getNumZonePoints(ZoneList_t const& sourceZones) const;
};

LgIndex_t getNumZonePoints(EntIndex_t sourceZone);

#endif // ZoneToVTKPolyDataSource_h__
