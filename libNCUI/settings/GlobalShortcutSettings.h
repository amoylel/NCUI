// Created by amoylel on 08/06/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_GLOBALSHORTCUTSETTINGS_H__
#define AMO_GLOBALSHORTCUTSETTINGS_H__

#include <string>
#include <memory>
#include <amo/json.hpp>
#include <amo/stdint.hpp>

#include "settings/NativeWindowSettings.h"

namespace amo {
    class GlobalShortcutSettings : public BasicSettings {
    public:
        GlobalShortcutSettings();
        
        /*!
         * @fn	void GlobalShortcutSettings::InitDefaultShortcutSettings();
         *
         * @brief	Init default shortcut settings.
         */
        
        void initDefaultShortcutSettings();
        
        virtual void afterUpdateArgsSettings() override;
        
        virtual amo::u8json toJson() override;
        
        /*!
         * @fn	bool GlobalShortcutSettings::isSameGlobalShortcut(std::shared_ptr<GlobalShortcutSettings> pOther);
         *
         * @brief	Query if 'pOther' is same global shortcut.
         *
         * @param	pOther	The other.
         *
         * @return	true if same global shortcut, false if not.
         */
        
        bool isSameGlobalShortcut(std::shared_ptr<GlobalShortcutSettings> pOther);
    public:
    
    
        /*! @brief	control. */
        bool ctrl;
        /*! @brief	win. */
        bool win;
        /*! @brief	alt. */
        bool alt;
        /*! @brief	shift. */
        bool shift;
        /*! @brief	char 类型 1个字符. */
        std::string key;
        /*! @brief	触发事件. */
        std::string action;
        
    };
}

#endif // AMO_GLOBALSHORTCUTSETTINGS_H__

