#include "stdafx.h"
#include "settings/NativeWindowSettings.h"



namespace amo {

    NativeWindowSettings::NativeWindowSettings() {
        initDefaultNativeSettings();
    }
    
    void NativeWindowSettings::initDefaultNativeSettings() {
        DEFAULT_ARGS_SETTINGS(id, "0");
        DEFAULT_ARGS_SETTINGS(name, "Chromium Embedded Framework (CEF)");
        DEFAULT_ARGS_SETTINGS(parent, "0");
        
        DEFAULT_ARGS_SETTINGS(title, "Chromium Embedded Framework (CEF)");
        DEFAULT_ARGS_SETTINGS(icon, "");
        DEFAULT_ARGS_SETTINGS(titleBar, false);
        DEFAULT_ARGS_SETTINGS(url, "chrome://version");
        DEFAULT_ARGS_SETTINGS(sizebox, "2,2,2,2");
        DEFAULT_ARGS_SETTINGS(roundcorner, 0);
        DEFAULT_ARGS_SETTINGS(windowClass, "NativeWindow");
        
        DEFAULT_ARGS_SETTINGS(modal, false);
        DEFAULT_ARGS_SETTINGS(show, true);
        DEFAULT_ARGS_SETTINGS(center, true);
        DEFAULT_ARGS_SETTINGS(adjustPos, true);
        
        
        DEFAULT_ARGS_SETTINGS(x, 0);
        DEFAULT_ARGS_SETTINGS(y, 0);
        
        DEFAULT_ARGS_SETTINGS(width, 1280);
        DEFAULT_ARGS_SETTINGS(height, 720);
        DEFAULT_ARGS_SETTINGS(minWidth, 0);
        DEFAULT_ARGS_SETTINGS(minHeight, 0);
        DEFAULT_ARGS_SETTINGS(maxWidth, 0);
        DEFAULT_ARGS_SETTINGS(maxHeight, 0);
        
        DEFAULT_ARGS_SETTINGS(resizable, true);
        DEFAULT_ARGS_SETTINGS(moveable, true);
        DEFAULT_ARGS_SETTINGS(minimizable, true);
        DEFAULT_ARGS_SETTINGS(maximizable, true);
        DEFAULT_ARGS_SETTINGS(fullscreenable, true);
        DEFAULT_ARGS_SETTINGS(closable, true);
        DEFAULT_ARGS_SETTINGS(fullscreen, false);
        DEFAULT_ARGS_SETTINGS(skipTaskbar, false);
        
        DEFAULT_ARGS_SETTINGS(alwaysOnTop, false);
        DEFAULT_ARGS_SETTINGS(offscreen, false);
        DEFAULT_ARGS_SETTINGS(accelerator, false);
        DEFAULT_ARGS_SETTINGS(transparent, false);
        DEFAULT_ARGS_SETTINGS(windowColor, 0xffffffffu);
        DEFAULT_ARGS_SETTINGS(hasShadow, true);
        DEFAULT_ARGS_SETTINGS(focusable, true);
        DEFAULT_ARGS_SETTINGS(esc, false);
        DEFAULT_ARGS_SETTINGS(back_forword, false);
        DEFAULT_ARGS_SETTINGS(disable_ime, false);
        DEFAULT_ARGS_SETTINGS(compute_cursor, false);
        DEFAULT_ARGS_SETTINGS(primaryColor, "#ff3280fc");
        DEFAULT_ARGS_SETTINGS(primaryColor2, "");
        
    }
    
    
    void NativeWindowSettings::afterUpdateArgsSettings() {
        STRING_ARGS_SETTING(id);
        STRING_ARGS_SETTING(name);
        STRING_ARGS_SETTING(parent);
        
        STRING_ARGS_SETTING(title);
        STRING_ARGS_SETTING(icon);
        BOOL_ARGS_SETTING(titleBar);
        STRING_ARGS_SETTING(url);
        STRING_ARGS_SETTING(sizebox);
        INT_ARGS_SETTING(roundcorner);
        STRING_ARGS_SETTING(windowClass);
        
        BOOL_ARGS_SETTING(modal);
        BOOL_ARGS_SETTING(show);
        BOOL_ARGS_SETTING(center);
        
        INT_ARGS_SETTING(x);
        INT_ARGS_SETTING(y);
        INT_ARGS_SETTING(width);
        INT_ARGS_SETTING(height);
        INT_ARGS_SETTING(minWidth);
        INT_ARGS_SETTING(minHeight);
        INT_ARGS_SETTING(maxWidth);
        INT_ARGS_SETTING(maxHeight);
        
        BOOL_ARGS_SETTING(resizable);
        BOOL_ARGS_SETTING(moveable);
        BOOL_ARGS_SETTING(minimizable);
        BOOL_ARGS_SETTING(maximizable);
        BOOL_ARGS_SETTING(fullscreenable);
        BOOL_ARGS_SETTING(closable);
        BOOL_ARGS_SETTING(fullscreen);
        BOOL_ARGS_SETTING(skipTaskbar);
        
        BOOL_ARGS_SETTING(alwaysOnTop);
        BOOL_ARGS_SETTING(offscreen);
        BOOL_ARGS_SETTING(accelerator);
        BOOL_ARGS_SETTING(transparent);
        UINT_ARGS_SETTING(windowColor);
        BOOL_ARGS_SETTING(hasShadow);
        BOOL_ARGS_SETTING(focusable);
        
        BOOL_ARGS_SETTING(esc);
        BOOL_ARGS_SETTING(back_forword);
        BOOL_ARGS_SETTING(disable_ime);
        BOOL_ARGS_SETTING(compute_cursor);
        
        
        
        if (settings.getString("primaryColor2").empty()) {
            settings.put("primaryColor2", settings.getString("primaryColor"));
        }
        
        STRING_ARGS_SETTING(primaryColor);
        STRING_ARGS_SETTING(primaryColor2);
        
        // 如果不能改变窗口大小，那么maximizable=true;
        if (!resizable) {
            DEFAULT_ARGS_SETTINGS(maximizable, true);
        }
        
        return ;
    }
    
    amo::u8json NativeWindowSettings::toJson() {
        UPDATE_ARGS_SETTINGS(id);
        UPDATE_ARGS_SETTINGS(name);
        UPDATE_ARGS_SETTINGS(parent);
        
        UPDATE_ARGS_SETTINGS(title);
        UPDATE_ARGS_SETTINGS(icon);
        UPDATE_ARGS_SETTINGS(titleBar);
        UPDATE_ARGS_SETTINGS(url);
        UPDATE_ARGS_SETTINGS(sizebox);
        UPDATE_ARGS_SETTINGS(roundcorner);
        UPDATE_ARGS_SETTINGS(windowClass);
        
        UPDATE_ARGS_SETTINGS(modal);
        UPDATE_ARGS_SETTINGS(show);
        UPDATE_ARGS_SETTINGS(center);
        
        UPDATE_ARGS_SETTINGS(x);
        UPDATE_ARGS_SETTINGS(y);
        UPDATE_ARGS_SETTINGS(width);
        UPDATE_ARGS_SETTINGS(height);
        UPDATE_ARGS_SETTINGS(minWidth);
        UPDATE_ARGS_SETTINGS(minHeight);
        UPDATE_ARGS_SETTINGS(maxWidth);
        UPDATE_ARGS_SETTINGS(maxHeight);
        
        UPDATE_ARGS_SETTINGS(resizable);
        UPDATE_ARGS_SETTINGS(moveable);
        UPDATE_ARGS_SETTINGS(minimizable);
        UPDATE_ARGS_SETTINGS(maximizable);
        UPDATE_ARGS_SETTINGS(fullscreenable);
        UPDATE_ARGS_SETTINGS(closable);
        UPDATE_ARGS_SETTINGS(fullscreen);
        UPDATE_ARGS_SETTINGS(skipTaskbar);
        
        UPDATE_ARGS_SETTINGS(alwaysOnTop);
        UPDATE_ARGS_SETTINGS(offscreen);
        UPDATE_ARGS_SETTINGS(accelerator);
        UPDATE_ARGS_SETTINGS(transparent);
        UPDATE_ARGS_SETTINGS(windowColor);
        UPDATE_ARGS_SETTINGS(hasShadow);
        UPDATE_ARGS_SETTINGS(focusable);
        
        UPDATE_ARGS_SETTINGS(esc);
        UPDATE_ARGS_SETTINGS(back_forword);
        UPDATE_ARGS_SETTINGS(disable_ime);
        UPDATE_ARGS_SETTINGS(compute_cursor);
        
        UPDATE_ARGS_SETTINGS(primaryColor);
        UPDATE_ARGS_SETTINGS(primaryColor2);
        
        return BasicSettings::toJson();
        
    }
    
    amo::u8json NativeWindowSettings::getThemeJson() {
        amo::u8json json;
        json.put("primaryColor", primaryColor);
        json.put("primaryColor2", primaryColor2);
        return json;
    }
    
}

