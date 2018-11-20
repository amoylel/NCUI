#include "stdafx.h"
#include "node/NodeRenderHandler.h"
#include "node/NodeHandlerHelper.h"

namespace amo {

    NodeRenderHandler::NodeRenderHandler(std::shared_ptr<NodeHandlerHelper>& pHelper)
        : m_pHelper(pHelper) {
        
    }
    
    
    bool NodeRenderHandler::GetRootScreenRect(CefRefPtr<CefBrowser> browser,
            CefRect& rect) {
        return false;
    }
    
    bool NodeRenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser,
                                        CefRect& rect) {
        return false;
    }
    
    bool NodeRenderHandler::GetScreenPoint(CefRefPtr<CefBrowser> browser,
                                           int viewX,
                                           int viewY,
                                           int& screenX,
                                           int& screenY) {
        return false;
    }
    
    bool NodeRenderHandler::GetScreenInfo(CefRefPtr<CefBrowser> browser,
                                          CefScreenInfo& screen_info) {
        return false;
    }
    
    void NodeRenderHandler::OnPopupShow(CefRefPtr<CefBrowser> browser,
                                        bool show) {
        return ;
    }
    
    void NodeRenderHandler::OnPopupSize(CefRefPtr<CefBrowser> browser,
                                        const CefRect& rect) {
        return ;
    }
    
    void NodeRenderHandler::OnPaint(CefRefPtr<CefBrowser> browser,
                                    CefRenderHandler::PaintElementType type,
                                    const CefRenderHandler::RectList& dirtyRects,
                                    const void* buffer,
                                    int width,
                                    int height) {
        return ;
    }
    
    bool NodeRenderHandler::StartDragging(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefDragData> drag_data,
                                          CefRenderHandler::DragOperationsMask allowed_ops,
                                          int x,
                                          int y) {
        return false;
    }
    void NodeRenderHandler::UpdateDragCursor(CefRefPtr<CefBrowser> browser,
            CefRenderHandler::DragOperation operation) {
        return;
    }
    
#if CHROME_VERSION_BUILD >= 2357
    void NodeRenderHandler::OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser,
            double x,
            double y) {
        return;
    }
    
#else
    
    
    void NodeRenderHandler::OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser) {
        return;
    }
#endif
    
    
#if CHROME_VERSION_BUILD >= 2272
    void NodeRenderHandler::OnCursorChange(CefRefPtr<CefBrowser> browser,
                                           CefCursorHandle cursor,
                                           CefRenderHandler::CursorType type,
                                           const CefCursorInfo& custom_cursor_info) {
        return ;
    }
    
    
    
#else
    
    
    void NodeRenderHandler::OnCursorChange(CefRefPtr<CefBrowser> browser,
                                           CefCursorHandle cursor) {
        return  ;
    }
    
    
#endif
    
}

