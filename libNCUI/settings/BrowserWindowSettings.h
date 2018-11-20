// Created by amoylel on 06/09/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_BROWSERSETTINGS_H__
#define AMO_BROWSERSETTINGS_H__

#include "handler/CefHeader.hpp"
#include "settings/NativeWindowSettings.h"

namespace amo {

    /**
     * @class	浏览器窗口参数
     *
     * @id settingsBrowserWindowSettings
     *
     * @brief	创建浏览器窗口时可用的属性.<br>
     * 			说明:**配置参数不能在程序运行过程中修改，只有程序内部函数可以修改这些值。**
     *
     * @chapter settings
     *
     * @copy NativeWindowSettings
     */
    
    class BrowserWindowSettings
        : public NativeWindowSettings
        , public CefBrowserSettings {
    public:
        BrowserWindowSettings();
        ~BrowserWindowSettings();
        
        /*!
         * @fn	void BrowserWindowSettings::InitDefaultCefBrowserSettings();
         * @ignore
         *
         * @brief	Init default cef browser settings.
         */
        
        void initDefaultCefBrowserSettings();  // 浏览器默认参数
        
        /*!
         * @fn	void BrowserWindowSettings::InitDefaultBrowserSettings();
         * @ignore
         *
         * @brief	Init default browser settings.
         */
        
        void initDefaultBrowserSettings();
        
        virtual void afterUpdateArgsSettings() override;
        
        virtual amo::u8json toJson() override;
    public:
        /*! @var #Boolean=false main 当前窗口是否为主窗口.主窗口只能有一个，如果设置为主窗口将取消之前的主窗口设定，新的主窗口会替代原来的主窗口.
         *	你可以在程序运行过程中修改该值，但要保证任何时候都最多只能有一个浏览器窗口被标记为main */
        bool main;
        
        /** @var #Boolean=false relad	是否允许页面刷新. 默认false */
        bool reload;
        
        /** @var #Int=0	 dragBlackList	不允许的拖拽操作黑名单,设定后对应类型的拖拽将不被允许。以下是可以使用的值,可以组合使用<br>
        /** @brief  DRAG_OPERATION_NONE    = 0,<br>
        			DRAG_OPERATION_COPY    = 1,<br>
        			DRAG_OPERATION_LINK    = 2,<br>
        			DRAG_OPERATION_GENERIC = 4,<br>
        			DRAG_OPERATION_PRIVATE = 8,<br>
        			DRAG_OPERATION_MOVE    = 16,<br>
        			DRAG_OPERATION_DELETE  = 32,<br>
        			DRAG_OPERATION_EVERY   = 0xffffffff */
        int dragBlacklist;
        
        /** @var #JsonArray=[]	cssList 需要注入到页面的样式列表，必须是一个URL。 程序会在主页面加载完成后将里面的文件注入到页面中. */
        amo::u8json cssList;
        
        /** @var #JsonArray=[] javascriptList 需要注入到页面的Javascript列表，必须是一个URL。 程序会在主页面加载完成后将里面的文件注入到页面中. */
        amo::u8json javascriptList;
        
        
        
        
    };
}

#endif // AMO_BROWSERSETTINGS_H__