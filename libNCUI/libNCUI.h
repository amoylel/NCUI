// Created by amoylel on 06/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_GRAPHIC_H__
#define AMO_GRAPHIC_H__



#include <WinSock2.h>
#include <windows.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <amo/smart_ptr.hpp>


#include "transfer/Transfer.hpp"
#include "transfer/FunctionWrapper.hpp"
#include "settings/BrowserWindowSettings.h"


#ifndef BUILDING_NCUI_STATIC
#ifdef BUILDING_NCUI_SHARED
#define NCUI_EXPORT __declspec(dllexport)
#else
#define NCUI_EXPORT __declspec(dllimport)
#endif
#else
#define  NCUI_EXPORT
#endif


#define NCUI_CALLBACK __stdcall




using namespace amo;
namespace amo {
    class AppContext;
}

using amo::IPCMessage;
using amo::Any;


extern "C" {

    /*!
     * @fn	NCUI_EXPORT int run(HINSTANCE hInstance);
     *
     * @brief	运行App.
     *
     * @param	hInstance	The instance.
     *
     * @return	An int.
     */
    
    NCUI_EXPORT int run(HINSTANCE hInstance);
    
    /*!
     * @fn	NCUI_EXPORT void updateAppSettings(const std::string& str);
     *
     * @brief	设置App参数.
     *
     * @param	str	The string.
     */
    
    NCUI_EXPORT void updateAppSettings(const std::string& str);
    
    /*!
     * @fn	NCUI_EXPORT void updateBrowserSettings(const std::string& str);
     *
     * @brief	设置默认BrowserWindow参数.
     *
     * @param	str	The string.
     */
    
    NCUI_EXPORT void updateBrowserSettings(const std::string& str);
    
    /*!
     * @fn	NCUI_EXPORT void updateSplashSettings(const std::string& str);
     *
     * @brief	设置默认Splash参数.
     *
     * @param	str	The string.
     */
    
    NCUI_EXPORT void updateSplashSettings(const std::string& str);
    
    /*!
     * @fn	NCUI_EXPORT void setMessageQueue(const std::string& strName);
     *
     * @brief	设置与Node交互的消息队列名.
     *
     * @param	strName	The name.
     */
    
    NCUI_EXPORT void setMessageQueue(const std::string& strName);
    
    /*!
     * @fn	NCUI_EXPORT int runSample(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
     *
     * @brief	CefSimple 测试.
     *
     * @param	hInstance	 	The instance.
     * @param	hPrevInstance	The previous instance.
     * @param	lpCmdLine	 	The command line.
     * @param	nCmdShow	 	The command show.
     *
     * @return	An int.
     */
    
    NCUI_EXPORT int runSample(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
}




#endif // AMO_GRAPHIC_H__