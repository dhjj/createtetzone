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
#include "Tetrahedralizer.h"
#include <stdexcept>
#include <vector>
#include "vtkDelaunay3D.h"
#include "vtkCommand.h"
#include "vtkSmartPointer.h"
#include "vtkUnstructuredGrid.h"
#include "Lock.h"
#include "ZoneToVTKPolyDataSource.h"
#include "Error.h"
#include "VTKUnstructuredGridToTecplotZone.h"

namespace
{

// Adapter between the VTK observation model and the simplified
// Tetrahedralizer::ProgressListenerInterface
class ProgressObserver
    : public vtkCommand
{
public:
    static ProgressObserver *New()
    {
        return new ProgressObserver;
    }

    void observe(vtkAlgorithm& algorithm)
    {
        algorithm.AddObserver(vtkCommand::StartEvent, this);
        algorithm.AddObserver(vtkCommand::EndEvent, this);
        algorithm.AddObserver(vtkCommand::ProgressEvent, this);
        algorithm.AddObserver(vtkCommand::WarningEvent, this);
    }

    void setListener(Tetrahedralizer::ProgressListenerInterface& listener)
    {
        m_listener = &listener;
    }

    bool warningEventOccurred() const
    {
        return m_warningEventOccurred;
    }

    virtual void Execute(vtkObject* caller,
                         unsigned long eventId,
                         void* /*callData*/)
    {
        if (m_listener)
        {
            switch (eventId)
            {
                case(vtkCommand::WarningEvent):
                {
                    m_warningEventOccurred = true;
                } break;

                case(vtkCommand::ProgressEvent):
                {
                    vtkAlgorithm& algorithm = dynamic_cast<vtkAlgorithm&>(*caller);
                    bool keepGoing = m_listener->update(algorithm.GetProgress() * 100.0);
                    if (!keepGoing)
                        algorithm.SetAbortExecute(true);
                } break;

                case(vtkCommand::StartEvent): m_listener->begin(); break;
                case(vtkCommand::EndEvent): m_listener->end(); break;

                default:
                    break;
            }
        }
    }

protected:
    ProgressObserver()
        : m_listener(NULL)
        , m_warningEventOccurred(false)
    {
    }

private:
    Tetrahedralizer::ProgressListenerInterface* m_listener;
    bool m_warningEventOccurred;
};

} // end anonymous namespace

Tetrahedralizer::Tetrahedralizer(ProgressListenerInterface& listener)
    : m_listener(listener)
{
}

void Tetrahedralizer::createTetrahedralZone(ZoneList_t const& sourceZones) const
{
    vtkSmartPointer<ZoneToVTKPolyDataSource> zoneSource = vtkSmartPointer<ZoneToVTKPolyDataSource>::New();
    zoneSource->setSourceZones(sourceZones);

    // vtkDelaunay3D does the real work of constructing tets
    vtkSmartPointer<vtkDelaunay3D> delaunay3D = vtkSmartPointer<vtkDelaunay3D>::New();
    delaunay3D->SetInputConnection(zoneSource->GetOutputPort());

    vtkSmartPointer<ProgressObserver> progressObserver = vtkSmartPointer<ProgressObserver>::New();
    progressObserver->observe(*delaunay3D);
    progressObserver->setListener(m_listener);

    delaunay3D->Update();

    if (delaunay3D->GetAbortExecute())
        throw Error("Creation of tetrahedral zone canceled.");

    if (delaunay3D->GetOutput()->GetNumberOfCells() == 0)
        throw Error("Error creating tetrahedral zone: No cells could be constructed.");

    if (progressObserver->warningEventOccurred())
    {
        Lock lockObject;
        TecUtilDialogMessageBox("Degenerate cells encountered. Mesh quality is suspect.",
                                MessageBox_Information);
    }

    VTKUnstructuredGridToTecplotZone().sendVTKUnstructuredGridToTecplot(sourceZones, *delaunay3D->GetOutput());
}
