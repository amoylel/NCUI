// Created by amoylel on 06/04/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_CEFHEADER_HPP__
#define AMO_CEFHEADER_HPP__

#include "include/cef_version.h"


#if CHROME_VERSION_BUILD >= 2454
#include "include/base/cef_logging.h"
#include "include/cef_resource_bundle.h"
#include "include/wrapper/cef_helpers.h"
#elif CHROME_VERSION_BUILD >= 2272
#include "include/base/cef_logging.h"
#include "include/wrapper/cef_helpers.h"
#else
#include "libcef_dll/cef_logging.h"
#endif



#if CHROME_VERSION_BUILD <2704
#include "include/cef_runnable.h"
#endif

#include "include/cef_cookie.h"
#include "include/cef_base.h"
#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/wrapper/cef_message_router.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/cef_sandbox_win.h"
#include "include/cef_v8.h"
#include "include/cef_browser.h"
#include "include/cef_urlrequest.h"
#include "include/cef_browser.h"
#include "include/cef_callback.h"
#include "include/cef_frame.h"
#include "include/cef_resource_handler.h"
#include "include/cef_response.h"
#include "include/cef_request.h"
#include "include/cef_scheme.h"
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"

#include "include/cef_browser.h"
#include "include/wrapper/cef_stream_resource_handler.h"
#include "include/cef_resource_bundle_handler.h"
#include "include/cef_request_handler.h"
#include "include/wrapper/cef_message_router.h"
#include "include/cef_render_process_handler.h"
#include "include/cef_v8.h"
#include "include/cef_render_handler.h"
#include "include/wrapper/cef_message_router.h"

#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/internal/cef_ptr.h"
#include "include/internal/cef_string.h"
#include "include/cef_load_handler.h"
#include "include/cef_life_span_handler.h"
#include "include/wrapper/cef_message_router.h"
#include "include/cef_life_span_handler.h"
#include "include/wrapper/cef_message_router.h"
#include "include/cef_app.h"
#include "include/cef_keyboard_handler.h"
#include "include/cef_jsdialog_handler.h"
#include "include/internal/cef_ptr.h"

#if CHROME_VERSION_BUILD >= 3325
#include "handler/patch/cef_geolocation_handler.h"
#else
#include "include/cef_geolocation_handler.h"
#endif

#include "include/cef_focus_handler.h"
#include "include/wrapper/cef_message_router.h"
#include "include/cef_client.h"
#include "include/cef_drag_handler.h"
#include "include/cef_base.h"
#include "include/cef_download_handler.h"
#include "include/cef_base.h"
#include "include/cef_download_handler.h"
#include "include/cef_dom.h"
#include "include/cef_dom.h"
#include "include/cef_dom.h"
#include "include/cef_display_handler.h"
#include "include/cef_dialog_handler.h"
#include "include/cef_context_menu_handler.h"
#include "include/internal/cef_ptr.h"
#include "include/wrapper/cef_byte_read_handler.h"
#include "include/internal/cef_types_wrappers.h"
#include "include/cef_command_line.h"
#include "include/cef_stream.h"
#include "include/cef_web_plugin.h"
#include "include/wrapper/cef_byte_read_handler.h"
#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/wrapper/cef_message_router.h"
#include "include/cef_app.h"

#include "include/cef_browser_process_handler.h"
#include "include/cef_browser_process_handler.h"
#include "include/internal/cef_string.h"

#include "include/internal/cef_string.h"
#include "include/internal/cef_string_wrappers.h"
#include "include/internal/cef_string.h"
#include "include/internal/cef_ptr.h"
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_browser.h"
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_sandbox_win.h"

#include "include/internal/cef_ptr.h"
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/internal/cef_types.h"
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/internal/cef_types.h"
#include "include/internal/cef_ptr.h"
#include "include/cef_frame.h"
#include "include/cef_urlrequest.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_stream_resource_handler.h"



#include <functional>

#if CHROME_VERSION_BUILD >= 2987
class ClosureHelper : public CefBaseRefCounted {
#else
class ClosureHelper : public CefBase {
#endif

public:
    ClosureHelper(std::function < void(void)> fn) : m_fn(fn) {};
    ~ClosureHelper() {
    
    }
    void Execute() {
        if (m_fn) {
            m_fn();
        }
    }
    
    static CefRefPtr<ClosureHelper>  Create(std::function<void(void)> fn) {
        CefRefPtr<ClosureHelper> ptr = new ClosureHelper(fn);
        return ptr;
    }
    
    IMPLEMENT_REFCOUNTING(ClosureHelper);
    
private:
    std::function<void(void)> m_fn;
    
};

#ifndef CLOSURE_HELPER
#define CLOSUER_HELPER(fn)\
	base::Bind(&ClosureHelper::Execute, ClosureHelper::Create(fn))
#endif


#ifndef CEF_VERSION_REGION
#define CEF_VERSION_REGION(low, high) (CHROME_VERSION_BUILD >= low  && CHROME_VERSION_BUILD < high)
#endif

#ifndef CEF_VERSION_EQ
#define CEF_VERSION_EQ(val) (CHROME_VERSION_BUILD == val )
#endif

#ifndef CEF_VERSION_GE
#define CEF_VERSION_GE(low) (CHROME_VERSION_BUILD >= low )
#endif

#ifndef CEF_VERSION_LT
#define CEF_VERSION_LT(high) (CHROME_VERSION_BUILD < high)
#endif


#endif // AMO_CEFHEADER_HPP__