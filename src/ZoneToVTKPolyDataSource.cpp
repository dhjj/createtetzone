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
#include "ZoneToVTKPolyDataSource.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkCellArray.h"
#include "vtkObjectFactory.h"
#include "Lock.h"

LgIndex_t getNumZonePoints(EntIndex_t sourceZone)
{
    LgIndex_t iMax, jMax, kMax = 0;
    TecUtilZoneGetIJK(sourceZone, &iMax, &jMax, &kMax);

    if (TecUtilZoneGetType(sourceZone) == ZoneType_Ordered)
        return iMax * jMax * kMax;
    else
        return iMax;
}

LgIndex_t ZoneToVTKPolyDataSource::getNumZonePoints(ZoneList_t const& sourceZones) const
{
    LgIndex_t numPoints = 0;

    ZoneToVTKPolyDataSource::ZoneList_t::const_iterator zone = sourceZones.begin();
    while (zone != sourceZones.end())
    {
        using ::getNumZonePoints;
        numPoints += getNumZonePoints(*zone);
        ++zone;
    }
    return numPoints;
}

vtkStandardNewMacro(ZoneToVTKPolyDataSource);

ZoneToVTKPolyDataSource::ZoneToVTKPolyDataSource()
{
    // Since we're a source, we have no inputs
    SetNumberOfInputPorts(0);
}

ZoneToVTKPolyDataSource::~ZoneToVTKPolyDataSource()
{
}

int ZoneToVTKPolyDataSource::RequestData(vtkInformation* request,
                                         vtkInformationVector** inputVector,
                                         vtkInformationVector* outputVector)
{
    UNUSED(request);
    UNUSED(inputVector);

    Lock lockObject;

    // Create VTK data containers
    vtkSmartPointer<vtkPoints> newPoints = vtkSmartPointer<vtkPoints>::New();
    newPoints->Allocate(getNumZonePoints(m_sourceZones));
    vtkSmartPointer<vtkCellArray> newVerts = vtkSmartPointer<vtkCellArray>::New();
    newVerts->Allocate(newVerts->EstimateSize(1, getNumZonePoints(m_sourceZones)));

    ZoneToVTKPolyDataSource::ZoneList_t::iterator zone = m_sourceZones.begin();
    while (zone != m_sourceZones.end())
    {
        using ::getNumZonePoints;

        for (LgIndex_t node = 1; node <= getNumZonePoints(*zone); ++node)
        {
            double coords[3];
            coords[0] = TecUtilDataValueGetByZoneVar(*zone, TecUtilVarGetNumByAssignment('X'), node);
            coords[1] = TecUtilDataValueGetByZoneVar(*zone, TecUtilVarGetNumByAssignment('Y'), node);
            coords[2] = TecUtilDataValueGetByZoneVar(*zone, TecUtilVarGetNumByAssignment('Z'), node);
            newVerts->InsertCellPoint(newPoints->InsertNextPoint(coords));
        }
        ++zone;
    }

    // Update the output data set
    vtkInformation *outInfo = outputVector->GetInformationObject(0);
    vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
    output->SetPoints(newPoints);
    output->SetVerts(newVerts);

    return 1;
}

void ZoneToVTKPolyDataSource::setSourceZones(ZoneList_t sourceZones)
{
    m_sourceZones.swap(sourceZones);
}
