// NCUI.cpp : 定义应用程序的入口点。
//


#include "stdafx.h"

#include "resource.h"
#include <string>
#include <amo/config.hpp>
#include <amo/loader_lite.hpp>
#include <amo/system.hpp>


static std::string GetFullAppPathA() {
    char executionDir[MAX_PATH];
    
    if (::GetModuleFileNameA(NULL, executionDir,
                             sizeof executionDir) == NULL) {
        executionDir[0] = 0;
    }
    
    std::string retval = executionDir;
    int index = retval.find_last_of('\\');
    
    if (index >= 0) {
        return retval.substr(0, index);
    }
    
    return retval;
}



static std::wstring GetFullAppPathW() {
    wchar_t executionDir[MAX_PATH];
    
    if (::GetModuleFileNameW(NULL, executionDir,
                             sizeof executionDir) == NULL) {
        executionDir[0] = 0;
    }
    
    std::wstring retval = executionDir;
    int index = retval.find_last_of('\\');
    
    if (index >= 0) {
        return retval.substr(0, index);
    }
    
    return retval;
}

int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPTSTR    lpCmdLine,
                      int       nCmdShow) {
                      
                      
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    
    /*amo::loader_lite loader;
    amo::path p(amo::path::getExeDir());
    p.append("libNCUI.dll");
    loader.load(p.c_str());
    typedef int(*fnRun)(HINSTANCE,
                        HINSTANCE,
                        LPTSTR,
                        int);
    fnRun fnUpdateAppSettings = (fnRun)loader.load_symbol("runSample");
    fnUpdateAppSettings(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    return 0;*/
    
    //return runNCUI();
    
    // XP下可能无法正确加载dll，不知道为啥,加载amo/config.hpp后
    // 里面boost的源代码不知道做了什么操作，就可以了(大小增加20k)
    amo::loader_litew loader;
    std::wstring dllPath = GetFullAppPathW();
    dllPath += L"\\startup.dll";
    
    if (!loader.load(dllPath)) {
        std::string sss = amo::system::getLastErrorMessage();
        MessageBoxA(NULL, sss.c_str(), "Error", MB_OK);
        return 0;
    }
    
    typedef int(*fnRun)(void);
    fnRun rnRunNCUI = (fnRun)loader.load_symbol("runNCUI");
    return  rnRunNCUI();
    
    
}
