// Created by amoylel on 07/28/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NATIVEWINDOWSETTINGS_H__
#define AMO_NATIVEWINDOWSETTINGS_H__

#include <string>
#include <memory>
#include <amo/json.hpp>
#include <amo/stdint.hpp>
#include "settings/BasicSettings.h"


namespace amo {

    /**
     * @class	NativeWindowSettings
     *
     * @chapter settings
     *
     * @unexport
     *
     * @brief	创建时原生窗口可用参数.<br>
     * 			说明:**配置参数不能在程序运行过程中修改，只有程序内部函数可以修改这些值。**
     *
     * @sa	BasicSettings
     */
    
    class NativeWindowSettings : public BasicSettings {
    public:
        NativeWindowSettings();
        
        /*!
         * @fn	void NativeWindowSettings::InitDefaultNativeSettings();
         * @ignore
         * @brief	Init default native settings.
         */
        
        void initDefaultNativeSettings();
        virtual void afterUpdateArgsSettings() override;
        
        virtual amo::u8json toJson() override;
        
        virtual amo::u8json getThemeJson();
    public:
    
        /*! @var #String=0 id	窗口ID，由程序设置该值，你不应该修改该值，当通过ID {@link fromID=BrowserWindow.fromID}查找窗口时可以使用该值 {@tag const}. */
        std::string id;
        
        /*! @var #String=Chromium{&nbsp;}Embedded{&nbsp;}Framework{&nbsp;}(CEF) name	窗口名，当通过名称{@link fromName=BrowserWindow.fromName}查找窗口时可以使用该值，如果想要通过名称查找窗口，那么这个值应该被设置成唯一的，默认为Chromium Embedded Framework (CEF) */
        std::string name;
        
        /*! @var #String=NULL parent	当前窗口的父窗口，创建窗口时设置. {@tag const}*/
        std::string parent;
        
        /*! @var #String=Chromium{&nbsp;}Embedded{&nbsp;}Framework{&nbsp;}(CEF) title	窗口标题（你可以在任何窗口预览的地方看到该值，如任务栏，任务管理器），默认为Chromium Embedded Framework (CEF),程序会根据页面的title标签内容自己修改该值. */
        std::string title;
        
        /*! @var #String="" icon	窗口图标，默认为空. */
        std::string icon;
        
        /*! @var #Boolean=false titleBar	是否显示标题栏. */
        bool titleBar;
        
        /*! @var #String=chrome://version url	需要打开的网页地址.{@tag const} */
        std::string url;
        
        /*! @var #String=2,2,2,2 sizebox	 可以被拖动改变窗口大小的边框距.{@tag const} */
        std::string sizebox;
        
        /*! @var #Int=0 roundcorner	 窗口圆角.{@tag const} */
        int roundcorner;
        
        /** @var #String=BrowserWindow windowClass	窗口类名.{@tag const} */
        std::string windowClass;
        
        /*! @var #Boolean=false modal	当前窗口是否为模态窗口，如果是那么在当前窗口之后所创建的窗口都是模态窗口，不管有没有指定modal=true {@tag const}. */
        bool modal;
        
        /*! @var #Boolean=true	 show 创建时是否显示窗口. */
        bool show;
        
        /*! @var #Boolean=true	 center 创建时窗口是否居中{@tag const}. */
        bool center;
        
        /*! @var #Boolean=true	adjustPos 是否允许调整窗口位置. */
        bool adjustPos;
        
        /*! @var #Int=0 x 窗口创建时的X轴坐标, adjustPos = true && center = false时有效. */
        int x;
        
        /*! @var #Int=0	 y 窗口创建时的Y轴坐标, adjustPos = true && center = false时有效. */
        int y;
        
        /*! @var #Int=1280 width	窗口宽度. */
        int width;
        
        /*! @var #Int=720 height 	窗口高度. */
        int height;
        
        /*! @var #Int=0 minWidth	窗口最小宽度，为0时不限制. */
        int minWidth;
        
        /*! @var #Int=0 minHeight	窗口最小宽度，为0时不限制. */
        int minHeight;
        
        /*! @var #Int=0 maxWidth	窗口最小宽度，为0时不限制. */
        int maxWidth;
        
        /*! @var #Int=0 maxHeight	窗口最小宽度，为0时不限制. */
        int maxHeight;
        
        /*! @var #Boolean=true resizable 	是否可以改变窗口大小. */
        bool resizable;
        
        /*! @var #Boolean=true	moveable 是否可以移动窗口. */
        bool moveable;
        
        /*! @var #Boolean=true minimizable	是否可以最小化窗口. */
        bool minimizable;
        
        /*! @var #Boolean=true	maximizable 是否要以最大化窗口，如果resizable==false, maximizable会被重置为true, 不管之前传入的是什么值. */
        bool maximizable;
        
        /*! @var #Boolean=true	fullscreenable 是否要以全屏窗口. */
        bool fullscreenable;
        
        /*! @var #Boolean=true	closable 是否要以关闭窗口. */
        bool closable;
        
        /*! @var #Boolean=false fullscreen	是否以全屏的方式启动窗口. */
        bool fullscreen;
        
        // @var #Boolean=false skipTaskbar	没什么用.
        bool skipTaskbar;
        
        /*! @var #Boolean=false alwaysOnTop	是否保持窗口前端显示. */
        bool alwaysOnTop;
        
        /*! @var #Boolean=false offscreen	是否使用离屏模式创建窗口{@tag const}. */
        bool offscreen;
        
        /*! @var #Boolean=false accelerator 是否使用GPU渲染窗口，offscreen=true是有效，需要Drect2D支持{@tag const}. */
        bool accelerator;
        
        /*! @var #Boolean=false transparent 是否使用透明窗口，offscreen=true时有效{@tag const}. */
        bool transparent;
        
        /*! @var #Int=0xffffffff windowColor 浏览器控件背景颜色. */
        uint32_t windowColor;
        
        /*! @var #Boolean=false hasShadow	是否显示窗口阴影. */
        bool hasShadow;
        
        /*! @var #Boolean=true	focusable 是否允许窗口获得焦点. */
        bool focusable;
        
        /*! @var #Boolean=false esc	是否允许ESC关闭窗口. */
        bool esc;
        
        /** @var #Boolean=false back_forword	是否允许前进后退{@tag const}. */
        bool back_forword;
        
        /*! @var #Boolean=false disable_ime 是否禁用输入法. */
        bool disable_ime;
        
        /*! @var #Boolean=false compute_cursor 计算光标位置，offscreen=true时有效. */
        bool compute_cursor;
        
        /** @var #String=#ff3280fc primaryColor 窗口主要颜色（一般为标题栏背景颜色）. */
        std::string primaryColor;
        
        /** @var #String=#ff3280fc primaryColor2 窗口主要颜色（一般为标题栏背景颜色2，与bkcolr配合可实现标题栏渐变色）. */
        std::string primaryColor2;
    };
}

#endif // AMO_NATIVEWINDOWSETTINGS_H__
