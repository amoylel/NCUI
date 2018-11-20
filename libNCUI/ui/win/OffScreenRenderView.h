// Created by amoylel on 06/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_OFFSCREENRENDERVIEW_H__
#define AMO_OFFSCREENRENDERVIEW_H__


#include <regex>
#include <memory>
#include <amo/timer.hpp>
#include "handler/CefHeader.hpp"


#include "handler/LifeSpanHandler.h"
#include "handler/RenderHandler.h"



#include "ui/win/OffscreenDragdrop.h"



#include "ui/win/RenderView.h"
#include "ui/win/ViewRenderer.h"


#include "handler/DisplayHandler.h"




class ActiveElementInfo {
public:
    ActiveElementInfo() {
        m_empty = true;
        m_size = 0;
        left = 0;
        top = 0;
        width = 0;
        height = 0;
        right = 0;
        bottom = 0;
        scrollWidth = 0;
        scrollHeight = 0;
        id = 0;
        fontSize = 0;
        r = g = b = 0;
    }
    
    ActiveElementInfo(amo::u8json& json) {
        m_empty = json.empty();
        
        if (m_empty) {
            return;
        }
        
        m_size = json.keys().size();
        type = json.get<std::string>("type");
        left = json.get<int>("left");
        top = json.get<int>("top");
        width = json.get<int>("width");
        height = json.get<int>("height");
        right = json.get<int>("right");
        bottom = json.get<int>("bottom");
        scrollWidth = json.get<int>("scrollWidth");
        scrollHeight = json.get<int>("scrollHeight");
        id = json.get<int>("id");
        fontFamily = json.get<std::string>("fontFamily");
        fontSize = json.get<int>("fontSize");
        std::string ss = json.get<std::string>("color");
        
        do {
            std::smatch m;
            std::regex rgbName("^rgb\\b");
            std::regex rgb("\\w+\\b");
            
            if (std::regex_search(ss, m, rgbName)) {
                std::cout << "retval:" << m[0].str() << std::endl;
                ss = m.suffix();
            } else {
                break;
            }
            
            if (std::regex_search(ss, m, rgb)) {
                r = amo::u8string(m[0].str(), true).to_number<int>();
                ss = m.suffix();
            }
            
            if (std::regex_search(ss, m, rgb)) {
                g = amo::u8string(m[0].str(), true).to_number<int>();
                ss = m.suffix();
                
            }
            
            if (std::regex_search(ss, m, rgb)) {
                b = amo::u8string(m[0].str(), true).to_number<int>();
                ss = m.suffix();
            }
            
            
        } while (false);
        
        lineHeight = json.get<std::string>("lineHeight");
    }
    
    bool empty() const {
        return m_empty;
    }
    
    size_t size() const {
        return m_size;
    }
    
public:

    std::string type;
    int	left;
    int	top;
    int	width;
    int	height;
    int	right;
    int	bottom;
    int	scrollWidth;
    int	scrollHeight;
    int	id;
    std::string	fontFamily;
    int	fontSize;
    std::string	lineHeight;
    bool m_empty;
    size_t m_size;
    int r;
    int g;
    int b;
};

namespace amo {
    class WebkitView;
    class BrowserWindowSettings;
    class IMM32Manager;
    
    //离屏模式下页面渲染窗口
    class OffScreenRenderView : public RenderView
        , public amo::RenderHandlerDelegate
        , public amo::DisplayHandlerDelegate
#if defined(CEF_USE_ATL)
        , public OsrDragEvents
#endif
    {
    public:
        OffScreenRenderView(std::shared_ptr<BrowserWindowSettings> pBrowserSettings);
        virtual ~OffScreenRenderView();
        
        LPCTSTR GetWindowClassName() const override;
        virtual void InitWindow() override;
        virtual void OnFinalMessage(HWND hWnd) override;
        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
        virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
                               BOOL& bHandled) override;
        virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                    BOOL& bHandled) override;
        virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL& bHandled) override;
        virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                BOOL& bHandled)  ;
        virtual LRESULT OnUserPaint(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                    BOOL& bHandled);
        /*   void EnableGL();
           void DisableGL();
           void Render();*/
        
        virtual void onFocusedNodeChanged(IPCMessage::SmartType msg);
        void getActiveElementInfo(CefRefPtr<CefFrame> pFrame);
        virtual void updateCaretPos(std::shared_ptr<Gdiplus::Bitmap>image) override;
        virtual void insertBitmap(std::shared_ptr<PaintResource> image) override;
        virtual bool DoPaintOnMemDC(HDC hDC, RECT rc) override;
        
    public:
    
        // ImeLifeSpanHandlerDelegate
        
        virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
        
        virtual bool GetRootScreenRect(CefRefPtr<CefBrowser> browser,
                                       CefRect& rect) override;
        virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
        virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser,
                                    int viewX, int viewY, int& screenX, int& screenY) override;
                                    
        virtual void OnPaint(CefRefPtr<CefBrowser> browser,
                             CefRenderHandler::PaintElementType type,
                             const CefRenderHandler::RectList& dirtyRects, const void* buffer, int width,
                             int height) override;
#if CHROME_VERSION_BUILD >= 2357
        virtual void OnCursorChange(CefRefPtr<CefBrowser> browser,
                                    CefCursorHandle cursor,
                                    CefRenderHandler::CursorType type,
                                    const CefCursorInfo& custom_cursor_info) override;
#else
        virtual void OnCursorChange(CefRefPtr<CefBrowser> browser,
                                    CefCursorHandle cursor);
#endif
                                    
        virtual bool StartDragging(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefDragData> drag_data,
                                   CefRenderHandler::DragOperationsMask allowed_ops, int x, int y) override;
        virtual void UpdateDragCursor(CefRefPtr<CefBrowser> browser,
                                      CefRenderHandler::DragOperation operation) override;
        virtual bool OnTooltip(CefRefPtr<CefBrowser> browser, CefString& text) override;
        
        void setFocusFrame(CefRefPtr<CefFrame> ptr);
        void setActiveElementInfo(amo::u8json& vec);
        
        int logicalToDevice(int value, float device_scale_factor);
        CefRect logicalToDevice(const CefRect& value, float device_scale_factor);
        int deviceToLogical(int value, float device_scale_factor);
        void deviceToLogical(CefMouseEvent& value, float device_scale_factor);
        
        
#if defined(CEF_USE_ATL)
        // OsrDragEvents methods.
        CefBrowserHost::DragOperationsMask OnDragEnter(CefRefPtr<CefDragData> drag_data,
                CefMouseEvent ev, CefBrowserHost::DragOperationsMask effect) override;
        CefBrowserHost::DragOperationsMask OnDragOver(CefMouseEvent ev,
                CefBrowserHost::DragOperationsMask effect) override;
        void OnDragLeave() override;
        CefBrowserHost::DragOperationsMask OnDrop(CefMouseEvent ev,
                CefBrowserHost::DragOperationsMask effect) override;
#endif  // defined(CEF_USE_ATL)
                
                
                
    private:
#if defined(CEF_USE_ATL)
        CComPtr<DropTargetWin> drop_target_;
        CefRenderHandler::DragOperation current_drag_op_;
#endif
        HDC hdc_;
        HGLRC hrc_;
        bool painting_popup_;
        float device_scale_factor_;
        CefRefPtr<CefBrowser> browser_;											//!< 浏览器指针
        POINT m_point;															//!< 光标位置
        CefRefPtr<CefFrame> m_pFocusFrame;										//!< 当前焦点iframe
        ActiveElementInfo m_oActiveElementInfo;									//!< 焦点控件信息
        WebkitView* m_pWebkit;													//!< 浏览器控件
        std::shared_ptr<IMM32Manager> imm32_manager_;							//!< 输入法管理器
        //OpenGLRenderer renderer_;												//!< OPENGL渲染器
        ViewRenderer* m_pViewRender;											//!< GDI+渲染器
        POINT m_candidatePos;													// 光标候选位置，通过光标闪烁来定位光标,不划算
    };
}
#endif // AMO_OFFSCREENRENDERVIEW_H__