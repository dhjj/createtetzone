cmake_minimum_required (VERSION 2.8)

set (CORE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set (CORE_HEADER_FILES
    ${CORE_SOURCE_DIR}/ADDGLBL.h
    ${CORE_SOURCE_DIR}/ADDONVER.h
    ${CORE_SOURCE_DIR}/FieldDataAllocator.h
    ${CORE_SOURCE_DIR}/FieldDataCopierAbstract.h
    ${CORE_SOURCE_DIR}/FieldDataCopierAbstractFactory.h
    ${CORE_SOURCE_DIR}/FieldDataCopierInterface.h
    ${CORE_SOURCE_DIR}/FieldDataSharer.h
    ${CORE_SOURCE_DIR}/Tetrahedralizer.h
    ${CORE_SOURCE_DIR}/FieldDataBlockCopier.h
    ${CORE_SOURCE_DIR}/FieldDataPointCopier.h
    ${CORE_SOURCE_DIR}/VTKUnstructuredGridToTecplotZone.h
    ${CORE_SOURCE_DIR}/ZoneToVTKPolyDataSource.h
)

set (CORE_SOURCE_FILES
    ${CORE_SOURCE_DIR}/FieldDataAllocator.cpp
    ${CORE_SOURCE_DIR}/FieldDataCopierAbstract.cpp
    ${CORE_SOURCE_DIR}/FieldDataCopierAbstractFactory.cpp
    ${CORE_SOURCE_DIR}/FieldDataSharer.cpp
    ${CORE_SOURCE_DIR}/Tetrahedralizer.cpp
    ${CORE_SOURCE_DIR}/FieldDataBlockCopier.cpp
    ${CORE_SOURCE_DIR}/FieldDataPointCopier.cpp
    ${CORE_SOURCE_DIR}/VTKUnstructuredGridToTecplotZone.cpp
    ${CORE_SOURCE_DIR}/ZoneToVTKPolyDataSource.cpp
    ${CORE_SOURCE_DIR}/main.cpp
)

list (APPEND HEADER_FILES ${CORE_HEADER_FILES})
list (APPEND SOURCE_FILES ${CORE_SOURCE_FILES})

include_directories (${CORE_SOURCE_DIR})

source_group (core FILES ${CORE_HEADER_FILES} ${CORE_SOURCE_FILES})
