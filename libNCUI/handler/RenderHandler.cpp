#include "stdAfx.h"
#include "handler/RenderHandler.h"
#include <amo/logger.hpp>

namespace amo {





    bool RenderHandler::StartDragging(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefDragData> drag_data,
                                      DragOperationsMask allowed_ops,
                                      int x,
                                      int y) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->StartDragging(browser, drag_data, allowed_ops, x, y);
        }
        
        return CefRenderHandler::StartDragging(browser, drag_data, allowed_ops, x, y);
    }
    
    void RenderHandler::UpdateDragCursor(CefRefPtr<CefBrowser> browser,
                                         DragOperation operation) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->UpdateDragCursor(browser, operation);
        }
        
        return CefRenderHandler::UpdateDragCursor(browser, operation);
        
        
    }
    
    
#if CHROME_VERSION_BUILD >= 2357
    void RenderHandler::OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser,
            double x,
            double y) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnScrollOffsetChanged(browser, x, y);
        }
    }
    
    
#else
    
    void RenderHandler::OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
    
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnScrollOffsetChanged(browser);
        }
    }
    
    
#endif
    
#if CHROME_VERSION_BUILD >= 2272
    
    void RenderHandler::OnCursorChange(CefRefPtr<CefBrowser> browser,
                                       CefCursorHandle cursor,
                                       CursorType type,
                                       const CefCursorInfo& custom_cursor_info) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnCursorChange(browser, cursor, type, custom_cursor_info);
        }
        
        return CefRenderHandler::OnCursorChange(browser, cursor, type, custom_cursor_info);
    }
    
#else
    
    void RenderHandler::OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
    
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnScrollOffsetChanged(browser);
        }
    }
    
    
#endif
    
    
    void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser,
                                PaintElementType type,
                                const RectList& dirtyRects,
                                const void* buffer,
                                int width,
                                int height) {
        //$clog(amo::cdevel << func_orient << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnPaint(browser, type, dirtyRects, buffer, width, height);
        }
    }
    
    void RenderHandler::OnPopupSize(CefRefPtr<CefBrowser> browser,
                                    const CefRect& rect) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnPopupSize(browser, rect);
        }
    }
    
    void RenderHandler::OnPopupShow(CefRefPtr<CefBrowser> browser,
                                    bool show) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnPopupShow(browser, show);
        }
    }
    
    bool RenderHandler::GetScreenInfo(CefRefPtr<CefBrowser> browser,
                                      CefScreenInfo& screen_info) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->GetScreenInfo(browser, screen_info);
            
            if (bHandled) {
                return true;
            }
        }
        
        return CefRenderHandler::GetScreenInfo(browser, screen_info);
    }
    
    bool RenderHandler::GetScreenPoint(CefRefPtr<CefBrowser> browser,
                                       int viewX,
                                       int viewY,
                                       int& screenX,
                                       int& screenY) {
        //$clog(amo::cdevel << func_orient << amo::endl;);
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->GetScreenPoint(browser, viewX, viewY, screenX, screenY);
            
            if (bHandled) {
                return true;
            }
        }
        
        return CefRenderHandler::GetScreenPoint(browser, viewX, viewY, screenX, screenY);
    }
    
    bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser,
                                    CefRect& rect) {
        //$clog(amo::cdevel << func_orient << amo::endl;);
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->GetViewRect(browser, rect);
            
            if (bHandled) {
                return true;
            }
        }
        
        return bHandled;
    }
    
    bool RenderHandler::GetRootScreenRect(CefRefPtr<CefBrowser> browser,
                                          CefRect& rect) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->GetRootScreenRect(browser, rect);
            
            if (bHandled) {
                return true;
            }
        }
        
        return CefRenderHandler::GetRootScreenRect(browser, rect);
    }
    
}