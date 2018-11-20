// Created by amoylel on 06/09/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_APPSETTINGS_H__
#define AMO_APPSETTINGS_H__

#include <amo/json.hpp>
#include <amo/singleton.hpp>
#include <amo/string.hpp>
#include "handler/CefHeader.hpp"
#include "BasicSettings.h"

namespace amo {

    /*!
     * @class 启动参数
     *
     * @id settingsAppSettings
     *
     * @desc 启动参数
     *
     *
     * @chapter settings
     *
     * @brief	应用程序启动参数，这些参数大部分不可更改，且只在程序启动前设置才能生效.<br>
     * 			如果使用NodeJS可以在JS代码里面调用{@link app.setConfig}设置启动参数.<br>
     * 			如果未使用NodeJS可以通过程序目录下的manifest.json文件达到同样的效果.<br>
     * 			说明:**配置参数不能在程序运行过程中修改，只有程序内部函数可以修改这些值。**
     *
     */
    
    class AppSettings : public CefSettings, public BasicSettings,
        public amo::singleton<AppSettings> {
    public:
        AppSettings();
        
        
        bool updateCefAppSettings();
        
        virtual void afterUpdateArgsSettings();
        
        virtual amo::u8json toJson()   override;
    private:
        void initDefaultCefSettings();
        
        void initDefaultAppSettings();
        
        amo::u8string getCachePath();
        
        /*!
         * @fn	amo::u8string AppSettings::getUserHomeDir();
         * @ignore
         * @brief	用户数据根目录.
         *
         * @return	The user home dir.
         */
        amo::u8string getUserHomeDir();
        
        /*!
         * @fn	std::string AppSettings::getSpecialFolder(int nType);
         * @ignore
         * @brief	获取Windows上的特殊目录.
         *
         * @param	nType	The type.
         *
         * @return	The special folder.
         */
        std::string getSpecialFolder(int nType);
    public:
    
    
        /*! @var	#String=%appDir%locales locales  语言文件所在目录  {@tag const}*/
        /*! @var	#Boolean=true single_process  是否使用单进程模式，如果程序比较复杂，应该尽量使用多进程模式{@tag const},cef3440及以上不再支持单进程模式，该字段失效*/
        /*! @var	#String=%appDir% resources_dir_path  资源文件所在目录{@tag const}*/
        /*! @var	#String=%APPDATA%appName/cache cache_path  CEF缓存文件所在目录{@tag const}*/
        /*! @var	#String=zh-CN locale  CEF语言环境{@tag const}*/
        
        
        
        /*! @var	#Boolean=true manifest	是否允许外部配置文件manifest.json；如果要使用，该文件必须与应用程序所在目录相同且只能使用ANSI编码， 不能用UTF8, 程序会将编码转换为UTF8{@tag const} */
        bool manifest;
        
        /*! @var	#String=783a02fd-b493-45ad-aa7f-ddbefeec1122 appID	程序唯一ID，默认为NCUI的程序ID. 其他程序应该修改该值，否则会影响单例模式的使用（appID相同的程序会被判定为同一个程序程序）{@tag const}. */
        std::string appID;
        
        /*! @var	#Boolean=false singleInstance	单例模式,该模式下只允许一个实例运行，如果尝试启动多个程序将不会成功 */
        bool singleInstance;
        
        /*! @var	#Boolean=false showSplash	是否显示启动画面.如果需要启动画面，需要修改配置参数{@link 启动画面} */
        bool showSplash;
        
        /*! @var	#Boolean=false useNode	是否使用NodeJs.{@tag const} */
        bool useNode;
        
        /*! @var	#Boolean=false useNodeProcess 是否在单独的进程中使用NodeJs.当useNode=true时该字段生效，如果在单独的进程中使用NodeJs,那么NCUI和Node将会在不同地进程运行，并通过通道进行消息同步 {@tag const}*/
        bool useNodeProcess;
        
        /** @var	#Int=0 ipcTimeout	进程通信的超时时间，如果超过这个时间没有得到返回结果，该消息将被丢弃。可以用来解决部分死锁问题。默认为0（永不超时）. */
        int64_t ipcTimeout;
        
        /*! @var	#String=main.js main	NodeJs所要调用的JS文件所在位置. useNode为true 时，该值有效{@tag const} */
        std::string main;
        
        /*! @var	#Boolean=false debugNode 是否允许调试NodeJs.如果未在单进行上运行Node时要调试NodeJs 需要使用NCUI-DEV.exe,即必须在命令行模式下运行NCUI(会相对于正常的图形界面多出一个命令行窗口){@tag const} */
        bool debugNode;
        
        /*! @var	#String appPath 程序在磁盘上的完整路径.{@tag const} */
        std::string appPath;
        
        /*! @var	#String appDir	程序所在目录.{@tag const} */
        std::string appDir;
        
        /*! @var	#String appName	程序名称，不包含目录，不包含扩展名.{@tag const} */
        std::string appName;
        
        /*! @var	#String=%appDir%skin skinDir	Duilib skin 目录.{@tag const} */
        std::string skinDir;
        
        /*! @var	#String=%appDir% workDir	工作目录,默认与程序所有目录相同.{@tag const} */
        std::string workDir;
        
        /*! @var	#String=%appDir%web webDir	html 源代码目录.{@tag const} */
        std::string webDir;
        
        /*! @var	#String homeDir	储存应用程序设置文件的文件夹，默认是 "我的文档" 文件夹附加应用的名称. */
        std::string homeDir;
        
        /*! @var	#String=%homeDir%downloads downloadsDir 用户下载目录的路径. */
        std::string downloadsDir;
        
        /*! @var	#String=%homeDir%musics musicsDir	用户音乐目录的路径. */
        std::string musicsDir;
        
        /*! @var	#String=%homeDir%pictures picturesDir	用户图片目录的路径. */
        std::string picturesDir;
        
        /*! @var	#String=%homeDir%videos videosDir	用户视频目录的路径. */
        std::string videosDir;
        
        /*! @var	#String=drag dragClassName	拖拽移动窗口的元素类名. */
        std::string dragClassName;
        
        /*! @var	#String=no-drag noDragClassName	禁止窗口元素类名. */
        std::string noDragClassName;
        
        /*! @var	#String=%APPDATA% appDataDir 当前用户的应用数据文件夹. */
        std::string appDataDir;
        
        /*!@var #String=%TEMP% temp	临时文件夹. {@tag const}*/
        std::string temp;
        
        /*! @var #String desktop 当前用户的桌面文件夹. {@tag const}*/
        std::string desktop;
        /*!
         * @var #String documents 用户文档目录的路径. {@tag const} */
        std::string documents;
        
        /*!
         * @var #Int startTime  启动时间（本地时间戳ms）.
         */
        int64_t startTime;
        
        /*!
         * @var #Boolean=true debugMode 是否以调试模式运行,默认true.该模式下F12可以打开调试窗口，F5可以刷新页面，并会输出日志
         */
        bool debugMode;
        
        /*! @var #Boolean=false clearCache 是否在应用程序启动时清除浏览器缓存（调试用）. {@tag const}*/
        bool clearCache;
        
        /** @var #JsonArray=[]	nonGlobalModules 禁止被导出到全局变量的类列表. */
        amo::u8json nonGlobalModules;
        
        /*! @var #Boolean=false dump 是否在崩溃时创建dump文件. */
        bool dump;
#if CEF_VERSION_REGION(3440, 10000)
        // const char kSingleProcess[]                 = "single-process";
        // command_line 添加命令行可以开户单进程模式
        bool single_process;  // 3340不再支持单进程模式，补一个变量以免大量修改代码
#endif
        
        
    };
}

#endif // AMO_APPSETTINGS_H__
