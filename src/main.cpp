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
#include "ADDGLBL.h"
#include "Tetrahedralizer.h"
#include "gui/ZoneSelectDialog.h"
#include "gui/StatusLineUpdater.h"
#include "macro/MacroCommandParser.h"
#include "Error.h"
#include "Lock.h"

Boolean_t STDCALL macroCommandCallback(char*  commandString,
                                       char** errorMessageRaw);

AddOn_pa AddOnID;

EXPORTFROMADDON void STDCALL InitTecAddOn()
{
    TecUtilLockOn();

    AddOnID = TecUtilAddOnRegister(120,
                                   ADDON_NAME,
                                   "V"ADDON_VERSION"("TecVersionId") "ADDON_DATE,
                                   "Tecplot, Inc.");

    Menu_pa toolsMenu = TecUtilMenuGetStandard(StandardMenu_Tools);
    if (toolsMenu)
    {
        Menu_pa menuEntry = TecUtilMenuInsertOption(toolsMenu,
                                                    MENU_POSITION_LAST,
                                                    "Create &Tetrahedral Zone...",
                                                    ZoneSelectDialog::launch,
                                                    ArbParam_t(0));
        if (menuEntry != NULL)
            TecUtilMenuRegisterSensitivityCallback(menuEntry,
                                                   ZoneSelectDialog::okToLaunch,
                                                   ArbParam_t(0));
    }

    TecUtilMacroAddCommandCallback(ADDON_NAME, macroCommandCallback);

    TecUtilLockOff();
}

std::auto_ptr<Tetrahedralizer::ProgressListenerInterface> createProgressListener() 
{
    class NullProgressListener
        : public Tetrahedralizer::ProgressListenerInterface
    {
        virtual void begin() {}
        virtual void end() {}
        virtual bool update(int) { return true; }
    };

    if (TecUtilMacroIsBatchModeActive())
        return new NullProgressListener;
    else
        return new StatusLineUpdater;
}

Boolean_t STDCALL macroCommandCallback(char*  commandString,
                                       char** errorMessageRaw)
{
    try
    {
        MacroCommandParser parser;
        parser.parse(commandString);

        std::auto_ptr<Tetrahedralizer::ProgressListenerInterface> progressListener = createProgressListener();
        Tetrahedralizer tetrahedralizer(*progressListener);
        tetrahedralizer.createTetrahedralZone(parser.getSourceZones());
        return true;
    }
    catch (Error const& e)
    {
        Lock lockStart;
        *errorMessageRaw = TecUtilStringAlloc(static_cast<int>(strlen(ADDON_NAME) + strlen(e.what()) + 10),
                                              NULL);
        if (*errorMessageRaw != NULL)
        {
#if defined(_MSC_VER)
# pragma warning (push)
# pragma warning (disable : 4996) // 'sprintf': This function or variable may be unsafe. 
#endif
            sprintf(*errorMessageRaw, "%s error:\n%s",
                    ADDON_NAME,
                    e.what());
#if defined(_MSC_VER)
# pragma warning(pop)
#endif
        }
        return false;
    }
}
