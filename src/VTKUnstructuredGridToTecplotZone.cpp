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
#include "VTKUnstructuredGridToTecplotZone.h"
#include <new> // for std::bad_alloc
#include "vtkCellArray.h"
#include "vtkUnstructuredGrid.h"
#include "toolbox/ArgList.h"
#include "toolbox/Set.h"
#include "toolbox/OutOfMemoryException.h"
#include "Lock.h"
#include "Error.h"
#include "ZoneToVTKPolyDataSource.h"
#include "FieldDataCopierInterface.h"
#include "FieldDataCopierAbstract.h"
#include "FieldDataSharer.h"
#include "FieldDataCopierAbstractFactory.h"

namespace tbx = tecplot::toolbox;

tbx::Set VTKUnstructuredGridToTecplotZone::getEnabledVars() const
{
    Set_pa enabledVars = NULL;
    if (TecUtilVarGetEnabled(&enabledVars) == FALSE)
        throw Error("Ran out of memory during tetrahedral zone creation.");

    try
    {
        tbx::Set enabledVarSet(enabledVars);
        TecUtilSetDealloc(&enabledVars);
        return enabledVarSet;
    }
    catch (tbx::OutOfMemoryException const&)
    {
        throw std::bad_alloc();
    }
}

void VTKUnstructuredGridToTecplotZone::sendConnectivityToTecplot(EntIndex_t destZone,
                                                                 vtkCellArray& cellArray) const
{
    Lock lockObject;
    REQUIRE(TecUtilZoneIsEnabled(destZone));
    REQUIRE("cellArray.GetNumberOfCells() is the same as destZone.jMax()");

    NodeMap_pa nodeMap = TecUtilDataNodeGetWritableRef(destZone);
    vtkIdType numPointsInCell = 0;
    vtkIdType* pointList = NULL;
    LgIndex_t curElement = 0;

    cellArray.InitTraversal();
    while (cellArray.GetNextCell(numPointsInCell, pointList))
    {
        const LgIndex_t numPointsPerTetra = 4;
        CHECK(numPointsInCell == numPointsPerTetra); // Should ALWAYS have tetrahedrons

        ++curElement;
        for (LgIndex_t cornerIndex = 0; cornerIndex < numPointsPerTetra; ++cornerIndex)
        {
            TecUtilDataNodeSetByRef(nodeMap,
                                    curElement,
                                    cornerIndex + 1,
                                    pointList[cornerIndex] + 1);
        }
    }
}

void VTKUnstructuredGridToTecplotZone::sendZoneAddedStateChange(EntIndex_t destZone) const
{
    Lock lockObject;
    tbx::ArgList stateChangedArgList;
    tbx::Set destZoneSet(destZone);
    stateChangedArgList.appendInt(SV_STATECHANGE, StateChange_ZonesAdded);
    stateChangedArgList.appendSet(SV_ZONELIST,    destZoneSet);
    TecUtilStateChangedX(stateChangedArgList.getRef());
}

/**
 * If sourceZones is a single, compatible zone, sets up sharing of all nodal vars with the
 * source zone. Else, copies data from sourceZones to new zones.
 */
void VTKUnstructuredGridToTecplotZone::addFieldData(ZoneList_t const& sourceZones,
                                                    EntIndex_t destZone) const
{
    REQUIRE(!sourceZones.empty());
    REQUIRE(TecUtilZoneIsEnabled(destZone));

    FieldDataCopierAbstractFactory fieldDataCopierAbstractFactory;
    tbx::Set enabledVars = getEnabledVars();
    EntIndex_t var = 0;
    TecUtilSetForEachMember(var, enabledVars.getRef())
    {
        FieldDataCopierPtr_t fieldDataCopier(fieldDataCopierAbstractFactory.createFieldDataCopier(sourceZones,
                                             destZone,
                                             var));
        fieldDataCopier->copyFieldData(sourceZones, destZone, var);
    }
}

using std::make_pair;

VTKUnstructuredGridToTecplotZone::DataTypeToRankMap_t VTKUnstructuredGridToTecplotZone::createDataTypeToRankMap() const
{
    DataTypeToRankMap_t dataTypeToRankMap;
    dataTypeToRankMap.insert(make_pair(FieldDataType_Double, 1));
    dataTypeToRankMap.insert(make_pair(FieldDataType_Float,  2));
    dataTypeToRankMap.insert(make_pair(FieldDataType_Float,  3));
    dataTypeToRankMap.insert(make_pair(FieldDataType_Double, 4));
    dataTypeToRankMap.insert(make_pair(FieldDataType_Int32,  5));
    dataTypeToRankMap.insert(make_pair(FieldDataType_Int16,  6));
    dataTypeToRankMap.insert(make_pair(FieldDataType_Byte,   7));
    dataTypeToRankMap.insert(make_pair(FieldDataType_Bit,    8));
    return dataTypeToRankMap;
}

VTKUnstructuredGridToTecplotZone::FieldDataTypeList_t 
VTKUnstructuredGridToTecplotZone::determineFieldDataTypes(ZoneList_t const& sourceZones) const
{
    FieldDataTypeList_t fieldDataTypes;
    fieldDataTypes.reserve(TecUtilDataSetGetNumVars());

    DataTypeToRankMap_t dataTypeToRankMap = createDataTypeToRankMap();

    tbx::Set enabledVars = getEnabledVars();
    EntIndex_t var = 0;
    TecUtilSetForEachMember(var, enabledVars.getRef())
    {
        typedef std::map<char, FieldDataType_e> RankToDataTypeMap_t;
        RankToDataTypeMap_t rankToDataTypeMap;

        for (ZoneList_t::const_iterator sourceZone = sourceZones.begin(); sourceZone != sourceZones.end(); ++sourceZone)
        {
            DataTypeToRankMap_t::iterator it = dataTypeToRankMap.find(TecUtilDataValueGetType(*sourceZone, var));
            rankToDataTypeMap.insert(make_pair(it->second, it->first));
        }

        fieldDataTypes.push_back(rankToDataTypeMap.begin()->second);
    }
    return fieldDataTypes;
}

EntIndex_t VTKUnstructuredGridToTecplotZone::createTetrahedralZone(vtkUnstructuredGrid &unstructuredGrid,
                                                                   ZoneList_t const& sourceZones) const
{
    Lock lockObject;
    tbx::ArgList addZoneArgList;
    addZoneArgList.appendString(SV_NAME,     "Tetrahedral zone");
    addZoneArgList.appendInt(SV_ZONETYPE, ZoneType_FETetra);
    addZoneArgList.appendInt(SV_IMAX,     unstructuredGrid.GetNumberOfPoints());
    addZoneArgList.appendInt(SV_JMAX,     unstructuredGrid.GetNumberOfCells());
    addZoneArgList.appendInt(SV_DEFERVARCREATION, true);
    FieldDataTypeList_t fieldDataTypes = determineFieldDataTypes(sourceZones);
    addZoneArgList.appendArray(SV_VARDATATYPE, fieldDataTypes);
    std::vector<ValueLocation_e> valueLocations(TecUtilDataSetGetNumVars(), ValueLocation_Nodal);
    addZoneArgList.appendArray(SV_VALUELOCATION, valueLocations);
    if (TecUtilDataSetAddZoneX(addZoneArgList.getRef()) != TRUE)
        throw Error("Unable to create Tecplot zone.");

    return TecUtilDataSetGetNumZones();
}

void VTKUnstructuredGridToTecplotZone::sendVTKUnstructuredGridToTecplot(ZoneList_t const& sourceZones,
                                                                        vtkUnstructuredGrid& unstructuredGrid) const
{
    EntIndex_t destZone = createTetrahedralZone(unstructuredGrid, sourceZones);

    addFieldData(sourceZones, destZone);

    sendConnectivityToTecplot(destZone, *unstructuredGrid.GetCells());

    sendZoneAddedStateChange(destZone);

    // Activate the new zone
    Lock lockObject;
    TecUtilZoneSetActive(tbx::Set(destZone).getRef(), AssignOp_PlusEquals);
}
