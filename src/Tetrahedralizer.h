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
#ifndef Tetrahedralizer_h__
#define Tetrahedralizer_h__

#include "boost/noncopyable.hpp"
#include "ADDGLBL.h" // For ZoneList_t typedef

class vtkUnstructuredGrid;

/*
 * Class to control the process of creating a tetrahedral zone from a given source zones.
 * It first creates a VTK pipeline, which includes a vtkDelaunay3D component, then sends
 * the output of the pipeline to a new Tecplot zone. Exceptions of type Error are thrown
 * in case of error.
 */
class Tetrahedralizer
    : public boost::noncopyable
{
public:
    /*
     * Create a tetrahedral zone out of the given source zone. The source zone must be
     * unorganized (i.e. JMax = 0, KMax = 0).
     */
    void createTetrahedralZone(ZoneList_t const& sourceZones) const;

    class ProgressListenerInterface
    {
    public:
        virtual void begin() = 0;

        virtual void end() = 0;

        // Return false to cancel the operation
        virtual bool update(int percentDone) = 0;

        virtual ~ProgressListenerInterface() {}
    };

    Tetrahedralizer(ProgressListenerInterface& listener);

private:
    void sendZoneToTecplot(ZoneList_t const& sourceZones,
                           vtkUnstructuredGrid& unstructuredGrid) const;
    void recordMacroCommand(ZoneList_t const& sourceZones) const;

    class SuspendDataSetMarking
    {
    public:
        SuspendDataSetMarking();
        ~SuspendDataSetMarking();
    };

    ProgressListenerInterface& m_listener;
};

#endif // Tetrahedralizer_h__
