// Created by amoylel on 10/06/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_SPLASHWINDOWSETTINGS_H__
#define AMO_SPLASHWINDOWSETTINGS_H__
#include "settings/NativeWindowSettings.h"
namespace amo {



    /*!
     * @class 启动画面
     *
     * @id	settingsSplashWindowSettings
     *
     * @desc 启动画面
     *
     * @chapter settings
     *
     *
     * @brief	启动画面相关参数.
     */
    
    class SplashWindowSettings
        : public NativeWindowSettings {
    public:
        SplashWindowSettings();
        ~SplashWindowSettings();
        
        
        /*!
        * @fn	void BrowserWindowSettings::InitDefaultBrowserSettings();
        *
        * @ignore
        *
        * @brief	初始化默认启动画面参数.
        */
        
        void initDefaultSplashSettings();
        
        virtual void afterUpdateArgsSettings() override;
        
        virtual amo::u8json toJson() override;
    public:
        /*! @var #String image 启动窗口显示图片. */
        std::string image;
        
        /*! @var #Int=0 duration	 显示时间， ms如果为0，表示不自动关闭. */
        int duration;
        
        /*! @var #Int=1000 fadeout 关闭动画持续时间ms. */
        int fadeout;
        
    };
}

#endif // AMO_SPLASHWINDOWSETTINGS_H__

