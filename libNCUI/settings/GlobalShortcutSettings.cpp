#include "stdafx.h"
#include "GlobalShortcutSettings.h"

namespace amo {

    GlobalShortcutSettings::GlobalShortcutSettings() {
        initDefaultShortcutSettings();
    }
    
    void GlobalShortcutSettings::initDefaultShortcutSettings() {
    
        DEFAULT_ARGS_SETTINGS(ctrl, false);
        DEFAULT_ARGS_SETTINGS(win, false);
        DEFAULT_ARGS_SETTINGS(alt, false);
        DEFAULT_ARGS_SETTINGS(shift, false);
        DEFAULT_ARGS_SETTINGS(key, "");
        DEFAULT_ARGS_SETTINGS(action, "");
    }
    
    
    
    void GlobalShortcutSettings::afterUpdateArgsSettings() {
        BOOL_ARGS_SETTING(ctrl);
        BOOL_ARGS_SETTING(win);
        BOOL_ARGS_SETTING(alt);
        BOOL_ARGS_SETTING(shift);
        STRING_ARGS_SETTING(key);
        STRING_ARGS_SETTING(action);
        return;
    }
    
    
    
    amo::u8json GlobalShortcutSettings::toJson() {
        UPDATE_ARGS_SETTINGS(ctrl);
        UPDATE_ARGS_SETTINGS(win);
        UPDATE_ARGS_SETTINGS(alt);
        UPDATE_ARGS_SETTINGS(shift);
        UPDATE_ARGS_SETTINGS(key);
        UPDATE_ARGS_SETTINGS(action);
        return BasicSettings::toJson();
    }
    
    bool GlobalShortcutSettings::isSameGlobalShortcut(
        std::shared_ptr<GlobalShortcutSettings> pOther) {
        if (ctrl != pOther->ctrl) {
            return false;
        }
        
        if (win != pOther->win) {
            return false;
        }
        
        if (alt != pOther->alt) {
            return false;
        }
        
        if (shift != pOther->shift) {
            return false;
        }
        
        if (key != pOther->key) {
            return false;
        }
        
        return true;
    }
    
}

