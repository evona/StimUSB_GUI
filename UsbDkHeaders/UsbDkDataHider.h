/**********************************************************************
* Copyright (c) 2013-2014  Red Hat, Inc.
*
* Developed by Daynix Computing LTD.
*
* Authors:
*     Dmitry Fleytman <dmitry@daynix.com>
*     Kirill Moizik <kirill@daynix.com>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
**********************************************************************/

#pragma once

#include "UsbDkDataHiderPublic.h"

#ifdef __cplusplus
class USB_DK_HIDE_RULE : public USB_DK_HIDE_RULE_PUBLIC
{
public:
    USB_DK_HIDE_RULE(PUSB_DK_HIDE_RULE_PUBLIC PublicRule = NULL, ULONG RuleType = USBDK_HIDER_RULE_DEFAULT) :
        Type(RuleType)
    {
        if (PublicRule)
        {
            Hide  = PublicRule->Hide;
            Class = PublicRule->Class;
            VID   = PublicRule->VID;
            PID   =  PublicRule->PID;
            BCD   = PublicRule->BCD;
        }
    }
    ULONG64 Type;
};

typedef USB_DK_HIDE_RULE *PUSB_DK_HIDE_RULE;
#endif
