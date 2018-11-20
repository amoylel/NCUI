#include "stdafx.h"
#include "libNCUI.h"


#include <WinSock2.h>
#include <windows.h>

// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
#include "stdafx.h"
#include <windows.h>

#include "include/cef_sandbox_win.h"
#include "handler/ClientApp.h"
#include "ui/win/BrowserWindow.h"
#include "ui/win/BrowserWindowManager.h"
#include "context/AppContext.h"
#include "ui/win/RenderView.h"
#include "handler/RenderHandler.h"
#include "ui/win/RenderView.h"
#include "settings/SplashWindowSettings.h"



#include <memory>
#include <unordered_map>



#ifdef _DEBUG
//#pragma comment(lib, "libcef_dll_wrapperUD32.lib")
//#pragma comment(lib, "libcef.lib")
//#pragma comment(lib, "libuvUD32.lib")
//#pragma comment(lib, "Psapi.lib")
//#pragma comment(lib, "Userenv.lib")
//#pragma comment(lib, "Ws2_32.lib")
//#pragma comment(lib, "Iphlpapi.lib")
//#pragma comment(lib, "glu32.lib")
//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "shlwapi.lib")
#else
//#pragma comment(lib, "libcef_dll_wrapperUR32.lib")
//#pragma comment(lib, "libcef.lib")
//#pragma comment(lib, "libuvUR32.lib")
//#pragma comment(lib, "Psapi.lib")
//#pragma comment(lib, "Userenv.lib")
//#pragma comment(lib, "Ws2_32.lib")
//#pragma comment(lib, "Iphlpapi.lib")
//#pragma comment(lib, "glu32.lib")
//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "shlwapi.lib")
#endif



#if defined(CEF_USE_SANDBOX)
// The cef_sandbox.lib static library is currently built with VS2013. It may not
// link successfully with other VS versions.
#pragma comment(lib, "cef_sandbox.lib")
#endif

using namespace amo;


NCUI_EXPORT void updateAppSettings(const std::string& str) {
    amo::AppContext::getInstance()->getDefaultAppSettings()->updateArgsSettings(
        str);
}

NCUI_EXPORT void updateBrowserSettings(const std::string& str) {
    if (!amo::AppContext::getInstance()->getDefaultAppSettings()->manifest) {
        return;
    }
    
    amo::AppContext::getInstance()->getDefaultBrowserSettings()->updateArgsSettings(
        str);
}

NCUI_EXPORT void updateSplashSettings(const std::string& str) {
    if (!amo::AppContext::getInstance()->getDefaultAppSettings()->manifest) {
        return;
    }
    
    amo::AppContext::getInstance()->getDefaultSplashSettings()->updateArgsSettings(
        str);
}

NCUI_EXPORT int run(HINSTANCE hInstance) {

#if CHROME_VERSION_BUILD >= 2526
    // Enable High-DPI support on Windows 7 or newer.
    CefEnableHighDPISupport();
#endif
    CefMainArgs main_args(hInstance);
    {
        amo::AppContext::getInstance()->initialize(hInstance);
        amo::AppContext::getInstance()->executeProcess(main_args);
        amo::AppContext::getInstance()->run(main_args);
    }
    
    amo::AppContext::releaseInstance();
    return 0;
    
}

NCUI_EXPORT void setMessageQueue(const std::string& strName) {
    auto handler = ::AppContext::getInstance()->getNodeMessageHandler();
    
    if (handler) {
        handler->setMessageQueue(strName);
    }
}
