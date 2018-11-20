#include "stdafx.h"
#include "settings/SplashWindowSettings.h"

namespace amo {

    SplashWindowSettings::SplashWindowSettings() {
        initDefaultSplashSettings();
    }
    
    SplashWindowSettings::~SplashWindowSettings() {
    
    }
    
    
    void SplashWindowSettings::initDefaultSplashSettings() {
    
        DEFAULT_ARGS_SETTINGS(alwaysOnTop, true);
        DEFAULT_ARGS_SETTINGS(hasShadow, false);
        DEFAULT_ARGS_SETTINGS(offscreen, true);
        DEFAULT_ARGS_SETTINGS(transparent, true);
        
        DEFAULT_ARGS_SETTINGS(image, "");
        DEFAULT_ARGS_SETTINGS(duration, 0);
        DEFAULT_ARGS_SETTINGS(fadeout, 1000);
    }
    
    void SplashWindowSettings::afterUpdateArgsSettings() {
        NativeWindowSettings::afterUpdateArgsSettings();
        STRING_ARGS_SETTING(image);
        INT_ARGS_SETTING(duration);
        INT_ARGS_SETTING(fadeout);
    }
    
    
    amo::u8json SplashWindowSettings::toJson() {
        UPDATE_ARGS_SETTINGS(image);
        UPDATE_ARGS_SETTINGS(duration);
        UPDATE_ARGS_SETTINGS(fadeout);
        return NativeWindowSettings::toJson();;
    }
    
}

