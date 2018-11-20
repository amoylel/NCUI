// Created by amoylel on 07/28/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_MENUWINDOWSETTINGS_H__
#define AMO_MENUWINDOWSETTINGS_H__

#include "settings/NativeWindowSettings.h"

namespace amo {
    class MenuWindowSettings : public NativeWindowSettings {
    public:
        MenuWindowSettings();
        
        /*!
         * @fn	void MenuWindowSettings::InitDefaultMenuSettings();
         *
         * @brief	Init default menu settings.
         */
        
        void initDefaultMenuSettings();
        
        virtual void afterUpdateArgsSettings() override;
        
        virtual amo::u8json toJson() override;
        
    public:
        /*! @brief	Height of the line. */
        int lineHeight;
        /*! @brief	The separator color. */
        std::string separatorColor;
    };
}

#endif // AMO_MENUWINDOWSETTINGS_H__
