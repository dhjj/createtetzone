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
#include "ZoneSelectDialog.h"
#include <exception>
#include <string>
#include <sstream>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>
#include "vtkOutputWindow.h"
#include "toolbox/StateChangeNotifier.h"
#include "ADDGLBL.h"
#include "GUIDEFS.h"
#include "Tetrahedralizer.h"
#include "Lock.h"
#include "Error.h"
#include "model-view/ZoneListModel.h"
#include "model-view/ListView.h"
#include "model-view/ListSelectionModelInterface.h"
#include "DialogDropper.h"

namespace tbx = tecplot::toolbox;

namespace
{

class DataSet
    : public TecUtilDataSetInterface
{
public:
    virtual EntIndex_t numZones() const;
    virtual std::string zoneName(EntIndex_t zoneNumber) const;
    virtual ZoneList_t enabledZones() const;
};

EntIndex_t DataSet::numZones() const
{
    if (TecUtilDataSetIsAvailable())
        return TecUtilDataSetGetNumZones();
    else
        return 0;
}

std::string DataSet::zoneName(EntIndex_t zoneNumber) const
{
    char* rawZoneName = NULL;
    try
    {
        TecUtilZoneGetName(zoneNumber, &rawZoneName);
        std::string zoneName(rawZoneName);
        TecUtilStringDealloc(&rawZoneName);
        return zoneName;
    }
    catch (...)
    {
        if (rawZoneName)
            TecUtilStringDealloc(&rawZoneName);
        throw;
    }
}

TecUtilDataSetInterface::ZoneList_t DataSet::enabledZones() const
{
    Set_pa activeZoneSet = NULL;
    try
    {
        ZoneList_t activeZones;
        if (TecUtilDataSetIsAvailable())
        {
            TecUtilZoneGetEnabled(&activeZoneSet);
            SetIndex_t zone = 0;
            TecUtilSetForEachMember(zone, activeZoneSet)
                activeZones.push_back(zone);
        }
        return activeZones;
    }
    catch (...)
    {
        if (activeZoneSet)
            TecUtilSetDealloc(&activeZoneSet);
        throw;
    }
}

struct ZoneSelectDialogImpl
{
    ZoneSelectDialogImpl();

    ZoneListModel::TecUtilDataSetInterfacePtr_t m_dataSetInterface;
    ZoneListModel::StateChangeNotifierPtr_t m_stateChangeNotifier;
    std::tr1::shared_ptr<ZoneListModel> m_zoneModel;
    std::auto_ptr<ListView> m_listView;
    std::auto_ptr<DialogDropper> m_dialogDropper;

    ZoneSelectDialogImpl(ZoneSelectDialogImpl const&);
    ZoneSelectDialogImpl& operator=(ZoneSelectDialogImpl const&);
};

ZoneSelectDialogImpl::ZoneSelectDialogImpl()
    : m_dataSetInterface(new DataSet)
    , m_stateChangeNotifier(new tbx::StateChangeNotifier)
    , m_zoneModel(new ZoneListModel(m_dataSetInterface, m_stateChangeNotifier))
    , m_listView(new ListView(sourceZone_MLST_D1, ListViewInterface::MultiSelect))
    , m_dialogDropper(new DialogDropper(*m_stateChangeNotifier))
{
    m_listView->setModel(m_zoneModel);
}

std::auto_ptr<ZoneSelectDialogImpl> impl;

class OutputWindow
    : public vtkOutputWindow
{
    static OutputWindow* New()
    {
        return new OutputWindow;
    }

    virtual void DisplayText(const char*)
    {
        // do nothing
    }

};

class ProgressListener
    : public Tetrahedralizer::ProgressListenerInterface
{
public:
    virtual void begin()
    {
        Lock lock;
        TecUtilStatusStartPercentDone("Creating tetrahedral zone...",
                                      true,  // show stop
                                      true); // show progress
    }

    virtual void end()
    {
        Lock lock;
        TecUtilStatusFinishPercentDone();
    }

    virtual bool update(int percentDone)
    {
        Lock lock;
        TecUtilStatusCheckPercentDone(percentDone);

        bool keepGoing = TecUtilInterruptCheck() == FALSE;
        return keepGoing;
    }
};

}

Boolean_t STDCALL ZoneSelectDialog::okToLaunch(ArbParam_t)
{
    return TecUtilFrameGetPlotType() == PlotType_Cartesian3D;
}

void STDCALL ZoneSelectDialog::launch(ArbParam_t)
{
    BuildDialog1(MAINDIALOGID);
    TecGUIDialogLaunch(Dialog1Manager); // This immediately calls init_CB, which does more work
}

void ZoneSelectDialog::drop()
{
    TecGUIDialogDrop(Dialog1Manager);
    impl.reset();
}

void ZoneSelectDialog::init_CB()
{
    replaceVTKOutputWindow();
    impl.reset(new ZoneSelectDialogImpl);
    TecGUISetInputFocus(sourceZone_MLST_D1);
}

void ZoneSelectDialog::okButton_CB()
{
    drop();
}

void ZoneSelectDialog::compute_BTN_CB()
{
    Lock lock;
    try
    {
        IndexList_t selectedIndices = impl->m_listView->selectionModel().getSelectedItems();
        if (selectedIndices.empty())
        {
            TecUtilDialogErrMsg("No source zone selected.");
        }
        else
        {
            ZoneList_t sourceZones;
            //std::transform(selectedIndices.begin(), selectedIndices.end(),
            //               std::back_inserter(sourceZones),
            //               std::tr1::bind(&ZoneListModel::data, impl->m_zoneModel.get()));
            for (IndexList_t::iterator index = selectedIndices.begin(); index != selectedIndices.end(); ++index)
                sourceZones.push_back(impl->m_zoneModel->data(*index));

            ProgressListener progressListener;
            Tetrahedralizer tetrahedralizer(progressListener);

            tetrahedralizer.createTetrahedralZone(sourceZones);

            showSuccessMessage(TecUtilDataSetGetNumZones());
            TecUtilRedraw(true);
        }
    }
    catch (Error const& e)
    {
        TecUtilDialogErrMsg(e.what());
    }

    launch();
}

void ZoneSelectDialog::sourceZone_MLST_CB(LgIndex_t const*)
{
}

void ZoneSelectDialog::replaceVTKOutputWindow()
{
    vtkOutputWindow::SetInstance(new OutputWindow);
}

#if 0
ZoneList_t ZoneSelectDialog::getSelectedSourceZones()
{
    LgIndex_t* selectedItemList = NULL;
    LgIndex_t selectedItemCount = 0;
    TecGUIListGetSelectedItems(sourceZone_MLST_D1,
                               &selectedItemList,
                               &selectedItemCount);

    ZoneList_t selectedZones;
    try
    {
        for (LgIndex_t selectedItemIndex = 0; selectedItemIndex < selectedItemCount; ++selectedItemIndex)
        {
            char* listEntry = TecGUIListGetString(sourceZone_MLST_D1,
                                                  selectedItemList[selectedItemIndex]);
            std::istringstream buffer(listEntry);
            TecUtilStringDealloc(&listEntry);

            EntIndex_t selectedZone;
            buffer >> selectedZone;
            selectedZones.push_back(selectedZone);
        }
        TecUtilArrayDealloc((void**)&selectedItemList);
    }
    catch (...)
    {
        TecUtilArrayDealloc((void**)&selectedItemList);
        throw;
    }

    return selectedZones;
}

bool ZoneSelectDialog::zoneIsUnordered(EntIndex_t zone)
{
    EntIndex_t iMax, jMax, kMax;
    TecUtilZoneGetIJK(zone, &iMax, &jMax, &kMax);
    return jMax == 1 && kMax == 1;
}

std::string ZoneSelectDialog::createZoneListEntry(EntIndex_t zone)
{
    std::ostringstream buffer;
    buffer << zone << " : ";

    char* zoneName = NULL;
    TecUtilZoneGetName(zone, &zoneName);
    buffer << zoneName;
    TecUtilStringDealloc(&zoneName);

    return buffer.str();
}

ZoneList_t ZoneSelectDialog::createCandidateZoneList()
{
    ZoneList_t candidateZones;

    Set_pa enabledZones;
    TecUtilZoneGetEnabled(&enabledZones);

    EntIndex_t zone = 0;
    TecUtilSetForEachMember(zone, enabledZones)
    {
        //if (zoneIsUnordered(zone))
        candidateZones.push_back(zone);
    }
    TecUtilSetDealloc(&enabledZones);

    return candidateZones;
}

void ZoneSelectDialog::fillZoneList()
{
    TecGUIListDeleteAllItems(sourceZone_MLST_D1);

    ZoneList_t candidateZones = createCandidateZoneList();
    for (ZoneList_t::iterator zone = candidateZones.begin(); zone != candidateZones.end(); ++zone)
        TecGUIListAppendItem(sourceZone_MLST_D1,
                             createZoneListEntry(*zone).c_str());
}
#endif // 0

void ZoneSelectDialog::showSuccessMessage(EntIndex_t destZone)
{
    std::ostringstream msg;
    msg << "Tetrahedral zone successfully created.\n";
    EntIndex_t iMax, jMax, kMax;
    TecUtilZoneGetIJK(destZone, &iMax, &jMax, &kMax);
    msg << iMax << " Nodes, " << jMax << " Tetrahedrons";
    Lock lock;
    TecUtilDialogMessageBox(msg.str().c_str(),
                            MessageBox_Information);
}

void ZoneSelectDialog::dummyButton_CB()
{

}