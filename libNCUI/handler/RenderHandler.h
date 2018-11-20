// Created by amoylel on 6/22/2017.
// Copyright (c) 2017 amoylel. All rights reserved.

#ifndef AMO_RENDERHANDLER_H__
#define AMO_RENDERHANDLER_H__

#include "handler/CefHeader.hpp"
#include "handler/HandlerDelegate.hpp"

namespace amo {

    /*!
     * @class	RenderHandlerDelegate
     *
     * @brief	离屏渲染处理类，该类中的方法会在UI线程中被调用.
     */
    
    class RenderHandlerDelegate {
    public:
        RenderHandlerDelegate() {}
        
        virtual ~RenderHandlerDelegate() {}
        
        virtual bool GetRootScreenRect(CefRefPtr<CefBrowser> browser,
                                       CefRect& rect) {
            return false;
        }
        
        virtual bool GetViewRect(CefRefPtr<CefBrowser> browser,
                                 CefRect& rect) {
            return false;
        }
        
        virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser,
                                    int viewX,
                                    int viewY,
                                    int& screenX,
                                    int& screenY) {
            return false;
        }
        
        virtual bool GetScreenInfo(CefRefPtr<CefBrowser> browser,
                                   CefScreenInfo& screen_info) {
            return false;
        }
        
        virtual void OnPopupShow(CefRefPtr<CefBrowser> browser,
                                 bool show) {
        }
        
        virtual void OnPopupSize(CefRefPtr<CefBrowser> browser,
                                 const CefRect& rect) {
        }
        
        virtual void OnPaint(CefRefPtr<CefBrowser> browser,
                             CefRenderHandler::PaintElementType type,
                             const CefRenderHandler::RectList& dirtyRects,
                             const void* buffer,
                             int width,
                             int height) {
        }
        
        
        
        virtual void UpdateDragCursor(CefRefPtr<CefBrowser> browser,
                                      CefRenderHandler::DragOperation operation) {
                                      
        }
        
        virtual bool StartDragging(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefDragData> drag_data,
                                   CefRenderHandler::DragOperationsMask allowed_ops,
                                   int x,
                                   int y) {
            return false;
        }
        
#if CHROME_VERSION_BUILD >= 2357
        virtual void OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser,
                                           double x,
                                           double y) {
        }
        
#else
        virtual void OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser) {
        }
#endif
        
#if CHROME_VERSION_BUILD >= 2272
        
        virtual void OnCursorChange(CefRefPtr<CefBrowser> browser,
                                    CefCursorHandle cursor,
                                    CefRenderHandler::CursorType type,
                                    const CefCursorInfo& custom_cursor_info) {
        }
        
        
#else
        
        
        
        virtual void OnCursorChange(CefRefPtr<CefBrowser> browser,
                                    CefCursorHandle cursor) {
        }
        
        
#endif
        
        
        
        
        
        
        
    };
    
    // 离屏渲染处理类，该类中的方法会在UI线程中被调用
    class RenderHandler : public CefRenderHandler
        , public HandlerDelegate < RenderHandlerDelegate > {
    public:
        RenderHandler() {}
        
        virtual ~RenderHandler() {}
        
        /*!
         * @fn	virtual bool RenderHandler::GetRootScreenRect(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRect& rect);
         *
         * @brief	获取浏览器父窗口在屏幕中的显示区域
         * 			B UI.
         *
         * @param	browser			The browser.
         * @param 	rect	The rectangle.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool GetRootScreenRect(CefRefPtr<CefBrowser> browser,
                                       CefRect& rect) override;
                                       
        /*!
         * @fn	virtual bool RenderHandler::GetViewRect(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRect& rect);
         *
         * @brief	获取页面渲染显示区域
         * 			B UI.
         *
         * @param	browser			The browser.
         * @param 	rect	The rectangle.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool GetViewRect(CefRefPtr<CefBrowser> browser,
                                 CefRect& rect) override;
                                 
        /*!
         * @fn	virtual bool RenderHandler::GetScreenPoint(
         * 		CefRefPtr<CefBrowser> browser,
         * 		int viewX,
         * 		int viewY,
         * 		int& screenX,
         * 		int& screenY);
         *
         * @brief	转换客户区坐标到屏幕坐标
         * 			B UI.
         *
         * @param	browser		   	The browser.
         * @param	viewX		   	The view x coordinate.
         * @param	viewY		   	The view y coordinate.
         * @param 	screenX	The screen x coordinate.
         * @param 	screenY	The screen y coordinate.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser,
                                    int viewX,
                                    int viewY,
                                    int& screenX,
                                    int& screenY) override;
                                    
        /*!
         * @fn	virtual bool RenderHandler::GetScreenInfo(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefScreenInfo& screen_info);
         *
         * @brief	获取屏幕信息
         * 			B UI.
         *
         * @param	browser			   	The browser.
         * @param 	screen_info	Information describing the screen.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool GetScreenInfo(CefRefPtr<CefBrowser> browser,
                                   CefScreenInfo& screen_info) override;
                                   
        /*!
         * @fn	virtual void RenderHandler::OnPopupShow(
         * 		CefRefPtr<CefBrowser> browser,
         * 		bool show);
         *
         * @brief	浏览器要显示或隐藏弹出窗口时触发
         * 			B UI.
         *
         * @param	browser	The browser.
         * @param	show   	true to show, false to hide.
         */
        
        virtual void OnPopupShow(CefRefPtr<CefBrowser> browser,
                                 bool show) override;
                                 
        /*!
         * @fn	virtual void RenderHandler::OnPopupSize(
         * 		CefRefPtr<CefBrowser> browser,
         * 		const CefRect& rect);
         *
         * @brief	页面要移动或改变窗口大小时触发
         * 			B UI.
         *
         * @param	browser	The browser.
         * @param	rect   	The rectangle.
         */
        
        virtual void OnPopupSize(CefRefPtr<CefBrowser> browser,
                                 const CefRect& rect) override;
                                 
        /*!
         * @fn	virtual void RenderHandler::OnPaint(
         * 		CefRefPtr<CefBrowser> browser,
         * 		PaintElementType type,
         * 		const RectList& dirtyRects,
         * 		const void* buffer,
         * 		int width,
         * 		int height);
         *
         * @brief	页面重绘时触发
         * 			B UI.
         *
         * @param	browser   	The browser.
         * @param	type	  	The type.
         * @param	dirtyRects	The dirty rectangles.
         * @param	buffer	  	The buffer.
         * @param	width	  	The width.
         * @param	height	  	The height.
         */
        
        virtual void OnPaint(CefRefPtr<CefBrowser> browser,
                             PaintElementType type,
                             const RectList& dirtyRects,
                             const void* buffer,
                             int width,
                             int height) override;
                             
                             
        virtual bool StartDragging(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefDragData> drag_data,
                                   DragOperationsMask allowed_ops,
                                   int x,
                                   int y) override;
                                   
        virtual void UpdateDragCursor(CefRefPtr<CefBrowser> browser,
                                      DragOperation operation);
                                      
#if CHROME_VERSION_BUILD >= 2357
        virtual void OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser,
                                           double x,
                                           double y) override;
#else
        virtual void OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser) override;
#endif
                                           
                                           
#if CHROME_VERSION_BUILD >= 2272
                                           
        /*!
         * @fn	virtual void RenderHandler::OnCursorChange(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefCursorHandle cursor,
         * 		CursorType type,
         * 		const CefCursorInfo& custom_cursor_info);
         *
         * @brief	光标发生改变时触发
         * 			B UI.
         *
         * @param	browser			  	The browser.
         * @param	cursor			  	The cursor.
         * @param	type			  	The type.
         * @param	custom_cursor_info	Information describing the custom cursor.
         */
        
        virtual void OnCursorChange(CefRefPtr<CefBrowser> browser,
                                    CefCursorHandle cursor,
                                    CursorType type,
                                    const CefCursorInfo& custom_cursor_info) override;
                                    
                                    
                                    
#else
        virtual void OnCursorChange(CefRefPtr<CefBrowser> browser,
                                    CefCursorHandle cursor) override;
                                    
                                    
#endif
                                    
        IMPLEMENT_REFCOUNTING(RenderHandler);
        
    };
}
#endif // AMO_RENDERHANDLER_H__
