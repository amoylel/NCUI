#include "stdafx.h"
#include "settings/BrowserWindowSettings.h"
#include <amo/string.hpp>



namespace amo {
    BrowserWindowSettings::BrowserWindowSettings() {
        initDefaultCefBrowserSettings();
        initDefaultBrowserSettings();
    }
    
    BrowserWindowSettings::~BrowserWindowSettings() {
    }
    
    void BrowserWindowSettings::initDefaultCefBrowserSettings() {
        CefString(&this->default_encoding) = amo::u8string("ISO-8859-1",
                                             true).to_utf8();
        plugins = STATE_ENABLED;
        remote_fonts = STATE_DEFAULT;
        javascript = STATE_ENABLED;
#if CHROME_VERSION_BUILD < 3112
        javascript_open_windows = STATE_ENABLED;
#endif
        
        javascript_close_windows = STATE_ENABLED;
        javascript_access_clipboard = STATE_ENABLED;
        javascript_dom_paste = STATE_ENABLED;
#if CHROME_VERSION_BUILD < 2924
        caret_browsing = STATE_ENABLED;
#endif
        
        //java = STATE_ENABLED;
        
        universal_access_from_file_urls = STATE_ENABLED;
        file_access_from_file_urls = STATE_ENABLED;
        web_security =
            STATE_DISABLED;		//访问外网时不能disable, 否则有部分网站容易崩溃
        //web_security = STATE_ENABLED;
        image_loading = STATE_ENABLED;
        image_shrink_standalone_to_fit = STATE_ENABLED;
        text_area_resize = STATE_ENABLED;
        tab_to_links = STATE_ENABLED;
        local_storage = STATE_ENABLED;
        databases = STATE_ENABLED;
        application_cache = STATE_ENABLED;
        webgl = STATE_DEFAULT;
        //accelerated_compositing = STATE_DEFAULT;
        //background_color = 0xffffffff;
        background_color = 0;
    }
    
    void BrowserWindowSettings::initDefaultBrowserSettings() {
        DEFAULT_ARGS_SETTINGS(url, "chrome://version");
        DEFAULT_ARGS_SETTINGS(offscreen, false);
        DEFAULT_ARGS_SETTINGS(transparent, false);
        DEFAULT_ARGS_SETTINGS(main, false);
        DEFAULT_ARGS_SETTINGS(windowClass, "BrowserWindow");
        //TODO: 改成false,默认不允许刷新
        DEFAULT_ARGS_SETTINGS(reload, true);
        DEFAULT_ARGS_SETTINGS(dragBlacklist, 0);
    }
    
    
    void BrowserWindowSettings::afterUpdateArgsSettings() {
        NativeWindowSettings::afterUpdateArgsSettings();
        BOOL_ARGS_SETTING(main);
        STRING_ARGS_SETTING(windowClass);
        BOOL_ARGS_SETTING(reload);
        INT_ARGS_SETTING(dragBlacklist);
        JSON_ARGS_SETTING(cssList);
        JSON_ARGS_SETTING(javascriptList);
        
    }
    
    
    
    
    amo::u8json BrowserWindowSettings::toJson() {
        UPDATE_ARGS_SETTINGS(main);
        UPDATE_ARGS_SETTINGS(windowClass);
        UPDATE_ARGS_SETTINGS(reload);
        UPDATE_ARGS_SETTINGS(dragBlacklist);
        UPDATE_ARGS_SETTINGS(cssList);
        UPDATE_ARGS_SETTINGS(javascriptList);
        return NativeWindowSettings::toJson();
    }
    
}