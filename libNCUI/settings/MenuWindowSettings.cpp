#include "stdafx.h"
#include "settings/MenuWindowSettings.h"

namespace amo {

    MenuWindowSettings::MenuWindowSettings() {
        initDefaultMenuSettings();
    }
    
    void MenuWindowSettings::initDefaultMenuSettings() {
        DEFAULT_ARGS_SETTINGS(lineHeight, 32);
        DEFAULT_ARGS_SETTINGS(center, false);
        DEFAULT_ARGS_SETTINGS(adjustPos, false);
        DEFAULT_ARGS_SETTINGS(separatorColor, "0xffe8e8e8");
        
    }
    
    void MenuWindowSettings::afterUpdateArgsSettings() {
        INT_ARGS_SETTING(lineHeight);
        UINT_ARGS_SETTING(windowColor);
        STRING_ARGS_SETTING(separatorColor);
        return NativeWindowSettings::afterUpdateArgsSettings();
    }
    
    amo::u8json MenuWindowSettings::toJson() {
        UPDATE_ARGS_SETTINGS(lineHeight);
        UPDATE_ARGS_SETTINGS(windowColor);
        UPDATE_ARGS_SETTINGS(separatorColor);
        return BasicSettings::toJson();
    }
    
}

