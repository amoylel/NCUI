#include "stdAfx.h"

#include "handler/ContextMenuHandler.h"

#include <amo/logger.hpp>


namespace amo {
    void ContextMenuHandler::OnContextMenuDismissed(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnContextMenuDismissed(browser, frame);
        }
    }
    
    bool ContextMenuHandler::OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefContextMenuParams> params,
            int command_id, EventFlags event_flags) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->OnContextMenuCommand(browser,
                                                   frame,
                                                   params,
                                                   command_id,
                                                   event_flags);
                                                   
            if (bHandled) {
                return true;
            }
        }
        
        return CefContextMenuHandler::OnContextMenuCommand(browser, frame,
                params, command_id, event_flags);
    }
    
    ContextMenuHandler::ContextMenuHandler() {
    
    }
    
    ContextMenuHandler::~ContextMenuHandler() {
    
    }
    
    void ContextMenuHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefContextMenuParams> params,
            CefRefPtr<CefMenuModel> model) {
            
        //$clog(amo::cdevel << func_orient << params->GetEditStateFlags()  << "\t" << params->GetMediaStateFlags() << "\t" << params->GetTypeFlags() << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        int count = model->GetCount();
        
        for (int i = 0; i < count; ++i) {
        
            /*MENUITEMTYPE_NONE,
            	MENUITEMTYPE_COMMAND,
            	MENUITEMTYPE_CHECK,
            	MENUITEMTYPE_RADIO,
            	MENUITEMTYPE_SEPARATOR,
            	MENUITEMTYPE_SUBMENU,
            */
            
            int key_code;
            bool shift;
            bool ctrl;
            bool alt;
            model->GetAcceleratorAt(i, key_code, shift, ctrl, alt);
            //$cdevel("type:{}, value:{}, keycode:{}, shift:{}, ctrl:{}, alt:{}, command:{}", (int)model->GetTypeAt(i), amo::string(model->GetLabelAt(i).ToString(), true).to_ansi(), key_code, shift, ctrl, alt, model->GetCommandIdAt(i));
        }
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnBeforeContextMenu(browser, frame, params, model);
        }
    }
}