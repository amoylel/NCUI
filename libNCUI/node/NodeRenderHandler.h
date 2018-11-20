// Created by amoylel on 07/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NODERENDERHANDLER_H__
#define AMO_NODERENDERHANDLER_H__

#include <memory>
#include "handler/RenderHandler.h"


namespace amo {
    class NodeHandlerHelper;
    class NodeRenderHandler : public RenderHandlerDelegate {
    public:
        NodeRenderHandler(std::shared_ptr<NodeHandlerHelper>& pHelper);
        
        
        
        virtual bool GetRootScreenRect(CefRefPtr<CefBrowser> browser,
                                       CefRect& rect) override;
                                       
                                       
        virtual bool GetViewRect(CefRefPtr<CefBrowser> browser,
                                 CefRect& rect) override;
                                 
                                 
        virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser,
                                    int viewX,
                                    int viewY,
                                    int& screenX,
                                    int& screenY) override;
                                    
                                    
        virtual bool GetScreenInfo(CefRefPtr<CefBrowser> browser,
                                   CefScreenInfo& screen_info) override;
                                   
                                   
        virtual void OnPopupShow(CefRefPtr<CefBrowser> browser,
                                 bool show) override;
                                 
                                 
        virtual void OnPopupSize(CefRefPtr<CefBrowser> browser,
                                 const CefRect& rect) override;
                                 
                                 
        virtual void OnPaint(CefRefPtr<CefBrowser> browser,
                             CefRenderHandler::PaintElementType type,
                             const CefRenderHandler::RectList& dirtyRects,
                             const void* buffer,
                             int width,
                             int height) override;
        virtual bool StartDragging(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefDragData> drag_data,
                                   CefRenderHandler::DragOperationsMask allowed_ops,
                                   int x,
                                   int y) override;
                                   
                                   
        virtual void UpdateDragCursor(CefRefPtr<CefBrowser> browser,
                                      CefRenderHandler::DragOperation operation) override;
                                      
#if CHROME_VERSION_BUILD >= 2357
                                      
                                      
                                      
        virtual void OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser,
                                           double x,
                                           double y) override;
                                           
#else
                                           
                                           
        virtual void OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser) override;
#endif
                                           
#if CHROME_VERSION_BUILD >= 2272
        virtual void OnCursorChange(CefRefPtr<CefBrowser> browser,
                                    CefCursorHandle cursor,
                                    CefRenderHandler::CursorType type,
                                    const CefCursorInfo& custom_cursor_info) override;
                                    
                                    
                                    
                                    
#else
        virtual void OnCursorChange(CefRefPtr<CefBrowser> browser,
                                    CefCursorHandle cursor) override;
                                    
                                    
#endif
                                    
    private:
        std::shared_ptr<NodeHandlerHelper>& m_pHelper;
    };
}


#endif // AMO_NODERENDERHANDLER_H__




