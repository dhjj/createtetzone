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
#ifndef ZoneSelectDialog_h__
#define ZoneSelectDialog_h__

#include <string>
#include "TECADDON.h"
#include "ADDGLBL.h" // For ZoneList_t

class ZoneSelectDialog
{
public:
    /**
     * Note: these must be static to be registered as TGB callbacks.
     */
    static Boolean_t STDCALL okToLaunch(ArbParam_t clientData = ArbParam_t(0));
    static void STDCALL launch(ArbParam_t clientData = ArbParam_t(0));
    static void drop();

private:
    /*
     * The functions in guibld.cpp that are relevant to this class must be declared
     * friends if we want to make the GUI callbacks private.
     */
    friend void BuildDialog1(LgIndex_t ParentDialog);

    /*
     * GUI Callbacks.
     */
    static void init_CB();
    static void okButton_CB();
    static void compute_BTN_CB();
    static void dummyButton_CB();
    static void sourceZone_MLST_CB(LgIndex_t const*);

    static void showSuccessMessage(EntIndex_t destZone);
    static void replaceVTKOutputWindow();

    /*
     * These #defines make the functions referenced in guibld.cpp resolve to (static)
     * member functions of this class.
     */
#define Dialog1Init_CB ZoneSelectDialog::init_CB
#define Dialog1CloseButton_CB ZoneSelectDialog::okButton_CB
#define Dialog1OkButton_CB ZoneSelectDialog::okButton_CB
#define compute_BTN_D1_CB ZoneSelectDialog::compute_BTN_CB
#define sourceZone_MLST_D1_CB ZoneSelectDialog::sourceZone_MLST_CB
};

#endif // ZoneSelectDialog_h__
