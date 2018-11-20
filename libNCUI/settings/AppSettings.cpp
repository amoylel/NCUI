#include "stdafx.h"
#include "settings/AppSettings.h"
#include "settings/NativeWindowSettings.h"


#pragma warning(disable:4800)



#define CEFSTRING_DEFAULT_ARGS_SETTINGS(name, val) CefString(&this->##name) = val; settings.put(#name, val);
#define CEFSTRING_ARGS_SETTING(val) CefString(&this->##val) = settings.getString(#val, CefString(&this->##val).ToString());

namespace amo {


    AppSettings::AppSettings() {
        //AMO_TIMER_INIT(程序开始);
        amo::timer::start_timer_foo("app start running");
        initDefaultCefSettings();
        initDefaultAppSettings();
        std::string str = settings.to_string();
    }
    
    void AppSettings::initDefaultCefSettings() {
        //!< 获取当前可执行文件目录
        amo::u8string strAppPath(amo::u8path::getExeDir(), true);
        //!< 日志文件，如果日志文件所在目录不存在，那么CEF不会使用该路径写入日志文件
        std::string strLogFile = (strAppPath + L"cef.log").to_utf8();
        
        /* amo::u8path logPath(strLogFile);
         logPath.absolute();
         logPath.parent().create_directory();*/
        
        //!< 完整路径的程序名
        std::string strExeFullName = amo::u8path::getFullExeName();
        //!< 资源目录
        std::string strResourcesPath = (strAppPath + "").to_utf8();
        //!< Local目录
        std::string strLocalesPath = (strAppPath + "\\locales").to_utf8();
        //!< 禁止当进程模式 ，该版本使用单进程模式无法渲染页
        DEFAULT_ARGS_SETTINGS(single_process,  true);
        //!< 禁用沙箱
        DEFAULT_ARGS_SETTINGS(no_sandbox, true);
        //!< 子进程路径文件，默认与当前程序文件相同
        CEFSTRING_DEFAULT_ARGS_SETTINGS(browser_subprocess_path, strExeFullName);
        //!< 禁止多线程消息循环，会出问题
        DEFAULT_ARGS_SETTINGS(multi_threaded_message_loop, false);
        //!< 设置资源目录
        CEFSTRING_DEFAULT_ARGS_SETTINGS(resources_dir_path,  strResourcesPath);
        //!< local目录
        CEFSTRING_DEFAULT_ARGS_SETTINGS(locales_dir_path,  strLocalesPath);
        //!< 默认缓存路径
        CEFSTRING_DEFAULT_ARGS_SETTINGS(cache_path,  getCachePath().to_utf8());
        //!< 语言环境默认中文
        CEFSTRING_DEFAULT_ARGS_SETTINGS(locale, "zh-CN");
        //!< 页面未加载前背景色
        DEFAULT_ARGS_SETTINGS(background_color, 0);
        //!< 指定CEF日志路径
        CEFSTRING_DEFAULT_ARGS_SETTINGS(log_file, strLogFile);
        //!< 关闭CEF日志
        DEFAULT_ARGS_SETTINGS(log_severity, LOGSEVERITY_DISABLE);
        
        // 下面的参数最好都不要去动
        //        DEFAULT_ARGS_SETTINGS(command_line_args_disabled,
        //                              false);				//!< 允许命令行参数
        //
        //        DEFAULT_ARGS_SETTINGS(persist_session_cookies, true);
        //
        //        //CefString(&user_agent) , "chrome://version");							//!< 使用默认
        //        //CefString(&product_version) , "v2.0");								//!< 不能设置该值，会出问题，使用默认。在子进程调用可能出现了问题
        //
        //        CEFSTRING_DEFAULT_ARGS_SETTINGS(log_file, strLogFile);					//!< 日志文件
        //
        //
        //        DEFAULT_ARGS_SETTINGS(log_severity, LOGSEVERITY_VERBOSE);				//!< 日志等级
        //        DEFAULT_ARGS_SETTINGS(log_severity,
        //                              LOGSEVERITY_DISABLE);				//!< 关闭, 日志太多
        //        //release_dcheck_enabled , true);										//!< 开启release check, 没什么用
        //
        //        //CefString(&javascript_flags) , "js-flags");							//!< 不知道怎么填
        //
        //        DEFAULT_ARGS_SETTINGS(pack_loading_disabled, false);					//!< .
        //        DEFAULT_ARGS_SETTINGS(remote_debugging_port, 8088);						//!< 调试端口
        //        DEFAULT_ARGS_SETTINGS(uncaught_exception_stack_size,
        //                              1000);				//!< 不知道该设置多少
        //
        //#if CHROME_VERSION_BUILD < 3071
        //        DEFAULT_ARGS_SETTINGS(context_safety_implementation, true);				//!< .
        //#endif
        //
        //        DEFAULT_ARGS_SETTINGS(ignore_certificate_errors,
        //                              true);					//!< 忽略证书错误
        //        //DEFAULT_ARGS_SETTINGS(background_color, 0xffffffff);					//!< 页面未加载前背景色
        //        DEFAULT_ARGS_SETTINGS(background_color,
        //                              0x00ffffff);					//!< 页面未加载前背景色
    }
    
    void AppSettings::initDefaultAppSettings() {
    
        amo::u8path p(amo::u8path::getExeName());
        p.remove_extension();
        amo::u8string strAppName(p.c_str(), true);
        
        
        DEFAULT_ARGS_SETTINGS(manifest, true);
        DEFAULT_ARGS_SETTINGS(appID, "783a02fd-b493-45ad-aa7f-ddbefeec1122");
        DEFAULT_ARGS_SETTINGS(singleInstance, false);
        DEFAULT_ARGS_SETTINGS(showSplash, false);
        DEFAULT_ARGS_SETTINGS(useNode, false);
        DEFAULT_ARGS_SETTINGS(useNodeProcess, false);
        DEFAULT_ARGS_SETTINGS(debugNode, false);
        DEFAULT_ARGS_SETTINGS(ipcTimeout, 0ull);
        DEFAULT_ARGS_SETTINGS(main, "main.js");  // utf8;
        
        DEFAULT_ARGS_SETTINGS(appPath,
                              amo::u8string(amo::u8path::getFullExeName(), true).to_utf8());
        DEFAULT_ARGS_SETTINGS(appDir, amo::u8string(amo::u8path::getExeDir(),
                              true).to_utf8());
        DEFAULT_ARGS_SETTINGS(appName, strAppName.to_utf8());
        
        DEFAULT_ARGS_SETTINGS(workDir, appDir);
        DEFAULT_ARGS_SETTINGS(skinDir, "%appDir%skin");
        DEFAULT_ARGS_SETTINGS(webDir, "%appDir%web");
        
        DEFAULT_ARGS_SETTINGS(homeDir, getUserHomeDir().to_utf8());
        DEFAULT_ARGS_SETTINGS(downloadsDir, "%homeDir%downloads");
        DEFAULT_ARGS_SETTINGS(musicsDir, getSpecialFolder(CSIDL_MYMUSIC));
        DEFAULT_ARGS_SETTINGS(picturesDir, getSpecialFolder(CSIDL_MYPICTURES));
        DEFAULT_ARGS_SETTINGS(videosDir, getSpecialFolder(CSIDL_MYVIDEO));
        
        DEFAULT_ARGS_SETTINGS(dragClassName, "drag");
        DEFAULT_ARGS_SETTINGS(noDragClassName, "no-drag");
        
        DEFAULT_ARGS_SETTINGS(appDataDir, getSpecialFolder(CSIDL_LOCAL_APPDATA));
        
        
        
        
        DEFAULT_ARGS_SETTINGS(temp, getSpecialFolder(CSIDL_TEMPLATES));
        
        DEFAULT_ARGS_SETTINGS(desktop, getSpecialFolder(CSIDL_DESKTOPDIRECTORY));
        DEFAULT_ARGS_SETTINGS(documents, getSpecialFolder(CSIDL_MYDOCUMENTS));
        
        
        DEFAULT_ARGS_SETTINGS(startTime, (int64_t)amo::timer::now());
        DEFAULT_ARGS_SETTINGS(debugMode, true);
        DEFAULT_ARGS_SETTINGS(dump, false);
        
        
        nonGlobalModules.set_array();
        //DEFAULT_ARGS_SETTINGS(debugMode, false);
        
    }
    
    amo::u8string AppSettings::getCachePath() {
        WCHAR path[MAX_PATH];// 缓存目录
        ZeroMemory(path, MAX_PATH);
        SHGetSpecialFolderPathW(NULL, path, CSIDL_LOCAL_APPDATA, FALSE);
        
        if (path[lstrlen(path) - 1] != '\\') {
            _tcscat(path, _T("\\"));
        }
        
        amo::u8path p(amo::u8path::getExeName());
        p.remove_extension().append("cache");
        
        amo::u8string strCacheDir(path);
        strCacheDir += p.generic_wstring();
        
        ::CreateDirectoryW(strCacheDir.to_wide().c_str(), NULL);
        return strCacheDir;
    }
    
    amo::u8string AppSettings::getUserHomeDir() {
    
        amo::u8string strUserDir(getSpecialFolder(CSIDL_MYDOCUMENTS), true);
        amo::u8path p(amo::u8path::getExeName());
        p.remove_extension();
        
        amo::u8path pData(strUserDir);
        pData.append(p);// 设置用户数据根目录
        
        ::CreateDirectoryA(pData.c_str(), NULL);				// 创建目录
        return amo::u8string(pData.c_str(), true);
    }
    
    std::string AppSettings::getSpecialFolder(int nType) {
        char path[MAX_PATH] = { 0 };
        SHGetSpecialFolderPathA(NULL, path, nType, FALSE);
        return amo::u8string(path, false).to_utf8();
    }
    
    bool AppSettings::updateCefAppSettings() {
    
        BOOL_ARGS_SETTING(
            single_process);										//!< 禁止当进程模式 ，该版本使用单进程模式无法渲染页
        BOOL_ARGS_SETTING(no_sandbox);											//!< 沙箱
        CEFSTRING_ARGS_SETTING(browser_subprocess_path);						//!< 子进程路径
        BOOL_ARGS_SETTING(
            multi_threaded_message_loop);							//!< 禁止多线程消息循环，会出问题
        BOOL_ARGS_SETTING(command_line_args_disabled);							//!< 允许命令行参数
        CEFSTRING_ARGS_SETTING(cache_path);										//!< 缓存路径
        BOOL_ARGS_SETTING(persist_session_cookies);
        
        //CefString(&user_agent) , "chrome://version");							//!< 使用默认
        //CefString(&product_version) , "v2.0");								//!< 不能设置该值，会出问题，使用默认。在子进程调用可能出现了问题
        CEFSTRING_ARGS_SETTING(locale);											//!< 语言环境
        CEFSTRING_ARGS_SETTING(log_file);										//!< 日志文件
        
        
        //CEFSTRING_ARGS_SETTING(log_severity);										//!< 日志等级
        //CEFSTRING_ARGS_SETTING(log_severity);										//!< 关闭
        
        //CefString(&javascript_flags) , "js-flags");							//!< 不知道怎么填
        CEFSTRING_ARGS_SETTING(resources_dir_path);								//!< 设置资源目录
        CEFSTRING_ARGS_SETTING(locales_dir_path);								//!< local目录
        BOOL_ARGS_SETTING(pack_loading_disabled);								//!< .
        INT_ARGS_SETTING(remote_debugging_port);								//!< 调试端口
        INT_ARGS_SETTING(
            uncaught_exception_stack_size);						//!< 不知道该设置多少
#if CHROME_VERSION_BUILD < 3071
        BOOL_ARGS_SETTING(context_safety_implementation); //!< .
#endif
        
        
        BOOL_ARGS_SETTING(ignore_certificate_errors);							//!< 忽略证书错误
        UINT_ARGS_SETTING(background_color);									//!< 页面未加载前背景色
        
        return true;
    }
    
    
    
    void AppSettings::afterUpdateArgsSettings() {
        AMO_TIMER_ELAPSED();
        
        // 如果源代码级不支持manifest，那么不允许修改程序启动参数
        if (!manifest) {
            return;
        }
        
        bool enableDebugMode = true;
        
        // 如果源代码级禁止调试模式，那么不允许开启调试工具
        if (!debugMode) {
            DEFAULT_ARGS_SETTINGS(debugMode, false);
        }
        
        updateCefAppSettings();
        
        
        BOOL_ARGS_SETTING(manifest);
        STRING_ARGS_SETTING(appID);
        BOOL_ARGS_SETTING(singleInstance);
        BOOL_ARGS_SETTING(showSplash);
        BOOL_ARGS_SETTING(useNode);
        BOOL_ARGS_SETTING(useNodeProcess);
        BOOL_ARGS_SETTING(debugNode);
        UINT64_ARGS_SETTING(ipcTimeout);
        
        STRING_ARGS_SETTING(main);
        
        STRING_ARGS_SETTING(workDir);
        STRING_ARGS_SETTING(skinDir);
        STRING_ARGS_SETTING(webDir);
        
        STRING_ARGS_SETTING(appDataDir);
        STRING_ARGS_SETTING(homeDir);
        
        STRING_ARGS_SETTING(dragClassName);
        STRING_ARGS_SETTING(noDragClassName);
        
        STRING_ARGS_SETTING(temp);
        
        
        STRING_ARGS_SETTING(desktop);
        STRING_ARGS_SETTING(documents);
        STRING_ARGS_SETTING(downloadsDir);
        STRING_ARGS_SETTING(musicsDir);
        STRING_ARGS_SETTING(picturesDir);
        STRING_ARGS_SETTING(videosDir);
        
        INT64_ARGS_SETTING(startTime);
        BOOL_ARGS_SETTING(debugMode);
        BOOL_ARGS_SETTING(clearCache);
        
        BOOL_ARGS_SETTING(dump);
        JSON_ARGS_SETTING(nonGlobalModules);
        
        
        
        ::SetCurrentDirectoryW(amo::u8string(workDir, true).to_unicode().c_str());
        AMO_TIMER_ELAPSED();
        return BasicSettings::afterUpdateArgsSettings();
    }
    
    amo::u8json AppSettings::toJson()   {
    
        UPDATE_ARGS_SETTINGS(manifest);
        UPDATE_ARGS_SETTINGS(appID);
        UPDATE_ARGS_SETTINGS(singleInstance);
        UPDATE_ARGS_SETTINGS(showSplash);
        UPDATE_ARGS_SETTINGS(useNode);
        UPDATE_ARGS_SETTINGS(useNodeProcess);
        UPDATE_ARGS_SETTINGS(debugNode);
        UPDATE_ARGS_SETTINGS(ipcTimeout);
        
        UPDATE_ARGS_SETTINGS(main);
        
        UPDATE_ARGS_SETTINGS(workDir);
        UPDATE_ARGS_SETTINGS(skinDir);
        UPDATE_ARGS_SETTINGS(webDir);
        
        UPDATE_ARGS_SETTINGS(appDataDir);
        UPDATE_ARGS_SETTINGS(homeDir);
        
        UPDATE_ARGS_SETTINGS(dragClassName);
        UPDATE_ARGS_SETTINGS(noDragClassName);
        
        UPDATE_ARGS_SETTINGS(temp);
        
        
        UPDATE_ARGS_SETTINGS(desktop);
        UPDATE_ARGS_SETTINGS(documents);
        UPDATE_ARGS_SETTINGS(downloadsDir);
        UPDATE_ARGS_SETTINGS(musicsDir);
        UPDATE_ARGS_SETTINGS(picturesDir);
        UPDATE_ARGS_SETTINGS(videosDir);
        
        UPDATE_ARGS_SETTINGS(startTime);
        UPDATE_ARGS_SETTINGS(debugMode);
        UPDATE_ARGS_SETTINGS(clearCache);
        UPDATE_ARGS_SETTINGS(nonGlobalModules);
        UPDATE_ARGS_SETTINGS(dump);
        
        return BasicSettings::toJson();
    }
    
}


