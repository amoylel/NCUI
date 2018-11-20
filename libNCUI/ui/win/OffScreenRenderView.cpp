#include "stdafx.h"


#include "ui/win/OffScreenRenderView.h"

#include <WinUser.h>
#include <shellapi.h>
#include <Gdiplus.h>
#include <Imm.h>

#include "handler/CefHeader.hpp"
#include "handler/ClientHandler.h"

#include "ui/win/RenderView.h"
#include "ui/win/OffscreenDragdrop.h"
#include "ipc/UIMessageEmitter.hpp"
#include "ipc/Any.hpp"

#include "settings/BrowserWindowSettings.h"
#include "ui/win/OffscreenDragdropEvents.h"
#include "ui/win/Imm32Manager.h"

#include "ui/win/renderer/D2D1Render.hpp"
#include "ui/win/WinUserMsg.hpp"

namespace {
    static CLIPFORMAT cf[3] = { CF_TEXT, CF_TEXT, CF_HDROP };
    
    // Render at 30fps during rotation.
    const int kRenderDelay = 1000 / 30;
    
    // Helper that calls wglMakeCurrent.
    class ScopedGLContext {
    public:
        ScopedGLContext(HDC hdc, HGLRC hglrc, bool swap_buffers)
            : hdc_(hdc),
              swap_buffers_(swap_buffers) {
            BOOL result = wglMakeCurrent(hdc, hglrc);
            ALLOW_UNUSED_LOCAL(result);
            DCHECK(result);
        }
        
        ~ScopedGLContext() {
            BOOL result = wglMakeCurrent(NULL, NULL);
            DCHECK(result);
            
            if (swap_buffers_) {
                result = SwapBuffers(hdc_);
                DCHECK(result);
            }
        }
        
    private:
        const HDC hdc_;
        const bool swap_buffers_;
    };
    
}  // namespace


namespace amo {

    OffScreenRenderView::OffScreenRenderView(
        std::shared_ptr<BrowserWindowSettings> pBrowserSettings)
        : RenderView(pBrowserSettings) {
        
        painting_popup_ = false;
        hdc_ = NULL;
        hrc_ = NULL;
        painting_popup_ = (false);
        device_scale_factor_ = GetDeviceScaleFactor();
        m_candidatePos.x = 0;
        m_candidatePos.y = 0;
        
    }
    
    OffScreenRenderView::~OffScreenRenderView() {
    
    }
    
    
    void OffScreenRenderView::OnFinalMessage(HWND hWnd) {
    
        WindowImplBase::OnFinalMessage(hWnd);
        delete this;
    }
    
    
    
    LRESULT OffScreenRenderView::HandleMessage(UINT uMsg, WPARAM wParam,
            LPARAM lParam) {
        static POINT lastMousePos, curMousePos;
        static bool mouseRotation = false;
        static bool mouseTracking = false;
        
        static int lastClickX = 0;
        static int lastClickY = 0;
        static CefBrowserHost::MouseButtonType lastClickButton = MBT_LEFT;
        static int gLastClickCount = 0;
        static double gLastClickTime = 0;
        
        static bool gLastMouseDownOnView = false;
        BOOL bHandled = false;
        LRESULT lRes = false;
        CefKeyEvent event;
        CefRefPtr<CefBrowserHost> pBrowserHost;
        
        if (m_pBrowser) {
            pBrowserHost  = m_pBrowser->GetHost();
        }
        
        CefBrowserHost::MouseButtonType btnType;
        UINT message = uMsg;
        LONG currentTime = 0;
        bool cancelPreviousClick = false;
        
        if (message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN ||
                message == WM_MBUTTONDOWN || message == WM_MOUSEMOVE ||
                message == WM_MOUSELEAVE) {
            currentTime = GetMessageTime();
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            cancelPreviousClick = (abs(lastClickX - x) > (GetSystemMetrics(
                                       SM_CXDOUBLECLK) / 2))
                                  || (abs(lastClickY - y) > (GetSystemMetrics(SM_CYDOUBLECLK) / 2))
                                  || ((currentTime - gLastClickTime) > GetDoubleClickTime());
                                  
            if (cancelPreviousClick &&
                    (message == WM_MOUSEMOVE || message == WM_MOUSELEAVE)) {
                gLastClickCount = 0;
                lastClickX = 0;
                lastClickY = 0;
                gLastClickTime = 0;
            }
        }
        
        switch (message) {
        case WM_PAINT:
            lRes = 	OnPaint(uMsg, wParam, lParam, bHandled);
            break;
            
        case WM_USER_PAINT:
            lRes = OnUserPaint(uMsg, wParam, lParam, bHandled);
            break;
            
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
            //case WM_MBUTTONDOWN:														// 不响应鼠标中键
        {
            SetCapture(m_hWnd);
            SetFocus(m_hWnd);
            btnType = (message == WM_LBUTTONDOWN ? MBT_LEFT : (
                           message == WM_RBUTTONDOWN ? MBT_RIGHT : MBT_MIDDLE));
                           
            if (pBrowserHost.get())  {
                CefMouseEvent mouse_event;
                mouse_event.x = GET_X_LPARAM(lParam);
                mouse_event.y = GET_Y_LPARAM(lParam);
                mouse_event.modifiers = GetCefMouseModifiers(wParam);
                pBrowserHost->SendMouseClickEvent(mouse_event, btnType, false, 1);
            }
            
            break;
        }
        
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
            //case WM_MBUTTONUP:														// 不响应鼠标中键
        {
            btnType = (message == WM_LBUTTONUP ? MBT_LEFT : (
                           message == WM_RBUTTONUP ? MBT_RIGHT : MBT_MIDDLE));
                           
            if (GetCapture() == m_hWnd) {
                ReleaseCapture();
            }
            
            if (pBrowserHost.get()) {
                CefMouseEvent mouse_event;
                mouse_event.x = GET_X_LPARAM(lParam);
                mouse_event.y = GET_Y_LPARAM(lParam);
                pBrowserHost->SendMouseClickEvent(mouse_event, btnType, true, 1);
            }
            
            break;
        }
        
        case WM_MOUSEMOVE: {
            if (pBrowserHost.get()) {
                CefMouseEvent mouse_event;
                mouse_event.x = GET_X_LPARAM(lParam);
                mouse_event.y = GET_Y_LPARAM(lParam);
                mouse_event.modifiers = GetCefMouseModifiers(wParam);
                pBrowserHost->SendMouseMoveEvent(mouse_event, false);
            }
            
            break;
        }
        
        case WM_MOUSELEAVE: {
            if (pBrowserHost.get()) {
                CefMouseEvent mouse_event;
                mouse_event.x = 0;
                mouse_event.y = 0;
                mouse_event.modifiers = GetCefMouseModifiers(wParam);
                pBrowserHost->SendMouseMoveEvent(mouse_event, true);
            }
            
            break;
        }
        
        case WM_MOUSEWHEEL: {
            if (!pBrowserHost.get()) {
                break;
            }
            
            POINT screen_point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            HWND scrolled_wnd = ::WindowFromPoint(screen_point);
            
            if (scrolled_wnd != m_hWnd) {
                break;
            }
            
            ::ScreenToClient(m_hWnd, &screen_point);
            int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            
            CefMouseEvent mouse_event;
            mouse_event.x = screen_point.x;
            mouse_event.y = screen_point.y;
            mouse_event.modifiers = GetCefMouseModifiers(wParam);
            pBrowserHost->SendMouseWheelEvent(mouse_event,
                                              IsKeyDown(VK_SHIFT) ? delta : 0,
                                              !IsKeyDown(VK_SHIFT) ? delta : 0);
                                              
            break;
        }
        
        case WM_SETFOCUS:
        case WM_KILLFOCUS:
            if (pBrowserHost.get()) {
                pBrowserHost->SendFocusEvent(message == WM_SETFOCUS);
            }
            
            return 0;
            break;
            
        case WM_CAPTURECHANGED:
        case WM_CANCELMODE:
            if (pBrowserHost.get()) {
                pBrowserHost->SendCaptureLostEvent();
            }
            
            break;
            
        case WM_SYSCHAR:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_CHAR: {
            event.windows_key_code = wParam;
            event.native_key_code = lParam;
            event.is_system_key = message == WM_SYSCHAR ||
                                  message == WM_SYSKEYDOWN ||
                                  message == WM_SYSKEYUP;
                                  
            if (message == WM_KEYDOWN || message == WM_SYSKEYDOWN) {
                event.type = KEYEVENT_RAWKEYDOWN;
            } else if (message == WM_KEYUP || message == WM_SYSKEYUP) {
                event.type = KEYEVENT_KEYUP;
            } else {
                event.type = KEYEVENT_CHAR;
            }
            
            event.modifiers = GetCefKeyboardModifiers(wParam, lParam);
            
            if (pBrowserHost.get()) {
                pBrowserHost->SendKeyEvent(event);
            }
            
            break;
        }
        
        case WM_SETCURSOR:
        case WM_DROPFILES:
            break;
            
        case WM_ERASEBKGND:
            return 0; //Don't erase the background.
            
        default:
            break;
        }
        
        if (lRes == TRUE && bHandled == TRUE) {
            return TRUE;
        }
        
        return RenderView::HandleMessage(uMsg, wParam, lParam);
    }
    
    void OffScreenRenderView::InitWindow() {
    
#if defined(CEF_USE_ATL)
        drop_target_ = DropTargetWin::Create(this, m_hWnd);
        HRESULT register_res = RegisterDragDrop(m_hWnd, drop_target_);
        DCHECK_EQ(register_res, S_OK);
#endif
        imm32_manager_.reset(new IMM32Manager(m_hWnd));
        CHorizontalLayoutUI* pRenderLayout = static_cast<CHorizontalLayoutUI*>(
                m_PaintManager.FindControl(_T("reanderLayout")));
                
        ASSERT(pRenderLayout != NULL);
        m_pViewRender = new ViewRenderer();
        
        m_pViewRender->setPaintSettings(m_oBrowserSettings);
        
        if (m_oBrowserSettings->offscreen && m_oBrowserSettings->accelerator) {
        
            if (!m_oBrowserSettings->transparent) {
                m_pViewRender->drawBackground(true);
            }
            
            m_pViewRender->setAccelerator(true);
        }
        
        if (m_oBrowserSettings->offscreen) {
            m_oBrowserSettings->background_color = CefColorSetARGB(0, 0, 0, 0);
        }
        
        pRenderLayout->Add(m_pViewRender);
        
        
        RECT rect;
        ::GetClientRect(m_hWnd, &rect);
        CefWindowInfo windowInfo;
#if CHROME_VERSION_BUILD >=3071
        windowInfo.SetAsWindowless(m_hWnd);
        
        if (m_oBrowserSettings->transparent) {
            m_oBrowserSettings->background_color = CefColorSetARGB(0, 0, 0, 0);
        }
        
        
        
#else
        windowInfo.SetAsWindowless(m_hWnd, m_oBrowserSettings->offscreen);
#endif
        
        
        if (m_pClientHandler) {
            m_pClientHandler->RegisterLifeSpanHandlerDelegate(this);
            m_pClientHandler->RegisterRenderHandlerDelegate(this);
            m_pClientHandler->RegisterDisplayHandlerDelegate(this);
        }
        
        std::string redirectUrl = UrlResourceHandlerFactory::getInstance()->redirectUrl(
                                      m_oBrowserSettings->url);
                                      
        if (!redirectUrl.empty()) {
            m_oBrowserSettings->url = redirectUrl;
        }
        
        CefBrowserHost::CreateBrowser(windowInfo,
                                      m_pClientHandler.get(),
                                      m_oBrowserSettings->url,
                                      *m_oBrowserSettings,
                                      NULL);
                                      
    }
    
    LRESULT OffScreenRenderView::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                        BOOL& bHandled) {
        if (m_pBrowser) {
            m_pBrowser->GetHost()->WasResized();
        }
        
        if (m_pViewRender) {
            m_pViewRender->SetPos({ 0, 0, LOWORD(lParam), HIWORD(lParam) });
        }
        
        SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
        
        if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
            CDuiRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++;
            rcWnd.bottom++;
            HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left,
                                             rcWnd.top,
                                             rcWnd.right,
                                             rcWnd.bottom,
                                             szRoundCorner.cx,
                                             szRoundCorner.cy);
            ::SetWindowRgn(*this, hRgn, TRUE);
            ::DeleteObject(hRgn);
        }
        
#endif
        bHandled = FALSE;
        return FALSE;
    }
    
    void OffScreenRenderView::insertBitmap(std::shared_ptr<PaintResource> image) {
        m_pViewRender->insertBitmap(image);
    }
    
    bool OffScreenRenderView::DoPaintOnMemDC(HDC hDC, RECT rc) {
        CControlUI* pRoot = m_PaintManager.GetRoot();
        
        if (!pRoot) {
            return false;
            
        }
        
        return  m_pViewRender->DoPaint(hDC, rc, m_pViewRender);
    }
    
    void OffScreenRenderView::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
        if (m_hWnd == (browser->GetHost()->GetWindowHandle())) {
            m_pBrowser = browser;
            browser_ = browser;
        }
    }
    
    LPCTSTR OffScreenRenderView::GetWindowClassName() const {
        return _T("OffScreenRenderView");
    }
    
    LRESULT OffScreenRenderView::OnNcHitTest(UINT uMsg, WPARAM wParam,
            LPARAM lParam, BOOL& bHandled) {
        return HTCLIENT;
    }
    
    bool OffScreenRenderView::GetRootScreenRect(CefRefPtr<CefBrowser> browser,
            CefRect& rect) {
        RECT window_rect = { 0 };
        HWND root_window = GetAncestor(m_hWnd, GA_ROOT);
        
        if (::GetWindowRect(root_window, &window_rect))  {
            rect = CefRect(window_rect.left,
                           window_rect.top,
                           window_rect.right - window_rect.left,
                           window_rect.bottom - window_rect.top);
            return true;
        }
        
        return false;
    }
    
    bool OffScreenRenderView::GetViewRect(CefRefPtr<CefBrowser> browser,
                                          CefRect& rect) {
        RECT clientRect;
        ::GetClientRect(m_hWnd, &clientRect);
        rect.x = rect.y = 0;
        rect.width = clientRect.right - clientRect.left;
        rect.height = clientRect.bottom - clientRect.top;
        return true;
    }
    
    bool OffScreenRenderView::GetScreenPoint(CefRefPtr<CefBrowser> browser,
            int viewX,
            int viewY,
            int& screenX,
            int& screenY) {
        if (!::IsWindow(m_hWnd)) {
            return false;
        }
        
        POINT screen_pt = {viewX, viewY};
        ::ClientToScreen(m_hWnd, &screen_pt);
        screenX = screen_pt.x;
        screenY = screen_pt.y;
        return true;
    }
    
    //VC下把HBITMAP保存为bmp图片
    BOOL  SaveBmp(HBITMAP     hBitmap, const char*     FileName) {
        HDC     hDC;
        //当前分辨率下每象素所占字节数
        int     iBits;
        //位图中每象素所占字节数
        WORD     wBitCount;
        //定义调色板大小，     位图中像素字节大小     ，位图文件大小     ，     写入文件字节数
        DWORD     dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
        //位图属性结构
        BITMAP     Bitmap;
        //位图文件头结构
        BITMAPFILEHEADER     bmfHdr;
        //位图信息头结构
        BITMAPINFOHEADER     bi;
        //指向位图信息头结构
        LPBITMAPINFOHEADER     lpbi;
        //定义文件，分配内存句柄，调色板句柄
        HANDLE     fh, hDib, hPal, hOldPal = NULL;
        
        //计算位图文件每个像素所占字节数
        hDC = CreateDCA("DISPLAY", NULL, NULL, NULL);
        iBits = GetDeviceCaps(hDC, BITSPIXEL)     *     GetDeviceCaps(hDC, PLANES);
        DeleteDC(hDC);
        
        if (iBits <= 1) {
            wBitCount = 1;
        } else  if (iBits <= 4) {
            wBitCount = 4;
        } else if (iBits <= 8) {
            wBitCount = 8;
        } else {
            wBitCount = 24;
        }
        
        GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
        bi.biSize = sizeof(BITMAPINFOHEADER);
        bi.biWidth = Bitmap.bmWidth;
        bi.biHeight = Bitmap.bmHeight;
        bi.biPlanes = 1;
        bi.biBitCount = wBitCount;
        bi.biCompression = BI_RGB;
        bi.biSizeImage = 0;
        bi.biXPelsPerMeter = 0;
        bi.biYPelsPerMeter = 0;
        bi.biClrImportant = 0;
        bi.biClrUsed = 0;
        
        dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
        
        //为位图内容分配内存
        hDib = GlobalAlloc(GHND,
                           dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
        lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
        *lpbi = bi;
        
        //     处理调色板
        hPal = GetStockObject(DEFAULT_PALETTE);
        
        if (hPal) {
            hDC = ::GetDC(NULL);
            hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
            RealizePalette(hDC);
        }
        
        //     获取该调色板下新的像素值
        GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight,
                  (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize,
                  (BITMAPINFO*)lpbi, DIB_RGB_COLORS);
                  
        //恢复调色板
        if (hOldPal) {
            ::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
            RealizePalette(hDC);
            ::ReleaseDC(NULL, hDC);
        }
        
        //创建位图文件
        fh = CreateFileA(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                         FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
                         
        if (fh == INVALID_HANDLE_VALUE) {
            return     FALSE;
        }
        
        //     设置位图文件头
        bmfHdr.bfType = 0x4D42;     //     "BM"
        dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize
                    + dwBmBitsSize;
        bmfHdr.bfSize = dwDIBSize;
        bmfHdr.bfReserved1 = 0;
        bmfHdr.bfReserved2 = 0;
        bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(
                               BITMAPINFOHEADER) + dwPaletteSize;
        //     写入位图文件头
        WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
        //     写入位图文件其余内容
        WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
        //清除
        GlobalUnlock(hDib);
        GlobalFree(hDib);
        CloseHandle(fh);
        
        return     TRUE;
    }
    
    void OffScreenRenderView::OnPaint(CefRefPtr<CefBrowser> browser,
                                      CefRenderHandler::PaintElementType type,
                                      const CefRenderHandler::RectList& dirtyRects,
                                      const void* buffer,
                                      int width,
                                      int height) {
        CEF_REQUIRE_UI_THREAD();
        
        if (!m_oBrowserSettings->offscreen) {
            return;
        }
        
        
    }
    
    
    LRESULT OffScreenRenderView::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                         BOOL& bHandled) {
        CefRefPtr<CefBrowserHost> pBrowserHost;
        
        if (m_pBrowser) {
            pBrowserHost = m_pBrowser->GetHost();
        }
        
        //if (pBrowserHost.get()) {
        //    RECT rect;
        //    ::GetClientRect(m_hWnd, &rect);
        //    CefRect rt(rect.left,
        //               rect.top,
        //               rect.right - rect.left,
        //               rect.bottom - rect.top);
        //    //pBrowserHost->Invalidate(PET_VIEW);
        //}
        
        static std::shared_ptr<amo::d2d1::D2D1HWNDRenderer> renderer;
        
        /*   if (!renderer) {
               renderer.reset(new amo::d2d1::D2D1HWNDRenderer());
               renderer->SetHWND(m_hWnd);
               renderer->Init();
           }
        
           PAINTSTRUCT ps = { 0 };
           ::BeginPaint(m_hWnd, &ps);
           renderer->BeginDraw();
           renderer->DrawImage("splash.jpg", 0, 0, 0, 0);
           renderer->EndDraw();
        
           m_pViewRender->PaintBitmap(m_hWnd);
           ::EndPaint(m_hWnd, &ps);*/
        //m_pViewRender->PaintStatusImage(GetDC(m_hWnd));
        
        bHandled = TRUE;
        return FALSE;
    }
    
    LRESULT OffScreenRenderView::OnUserPaint(UINT uMsg, WPARAM wParam,
            LPARAM lParam, BOOL& bHandled) {
        CefRefPtr<CefBrowserHost> pBrowserHost;
        
        if (m_pBrowser) {
            pBrowserHost = m_pBrowser->GetHost();
        }
        
        //if (pBrowserHost.get()) {
        //    RECT rect;
        //    ::GetClientRect(m_hWnd, &rect);
        //    CefRect rt(rect.left,
        //               rect.top,
        //               rect.right - rect.left,
        //               rect.bottom - rect.top);
        //    //pBrowserHost->Invalidate(PET_VIEW);
        //}
        
        /*   static std::shared_ptr<amo::d2d1::D2D1HWNDRenderer> renderer;
        
           if (!renderer) {
               renderer.reset(new amo::d2d1::D2D1HWNDRenderer());
               renderer->SetHWND(m_hWnd);
               renderer->Init();
           }*/
        
        PAINTSTRUCT ps = { 0 };
        ::BeginPaint(m_hWnd, &ps);
        
        /*      renderer->BeginDraw();
              renderer->DrawImage("splash.jpg", 0, 0, 0, 0);
              renderer->EndDraw();*/
        if (m_oBrowserSettings->offscreen && !m_oBrowserSettings->transparent) {
            m_pViewRender->PaintBitmap(m_hWnd);
        }
        
        //m_pViewRender->PaintBitmap(GetDC(m_hWnd));
        ::EndPaint(m_hWnd, &ps);
        //m_pViewRender->PaintStatusImage(GetDC(m_hWnd));
        
        bHandled = TRUE;
        return FALSE;
    }
    
    LRESULT OffScreenRenderView::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                           BOOL& bHandled) {
#if defined(CEF_USE_ATL)
        // Revoke/delete the drag&drop handler.
        RevokeDragDrop(m_hWnd);
        drop_target_ = NULL;
#endif
        
        if (m_pViewRender != NULL) {
            m_pViewRender->insertBitmap(std::shared_ptr<PaintResource>());
        }
        
        if (m_pClientHandler) {
            m_pClientHandler->UnregisterLifeSpanHandlerDelegate(this);
            m_pClientHandler->UnregisterRenderHandlerDelegate(this);
            m_pClientHandler->UnregisterDisplayHandlerDelegate(this);
        }
        
        m_pClientHandler = NULL;
        return RenderView::OnDestroy(uMsg, wParam, lParam, bHandled);
        
    }
    
    bool OffScreenRenderView::OnTooltip(CefRefPtr<CefBrowser> browser,
                                        CefString& text) {
        if (m_pViewRender == NULL) {
            return false;
        }
        
        amo::u8string strTips(text.ToString(), true);
        m_pViewRender->SetToolTip(strTips.to_unicode().c_str());
        return true;
    }
    
    
    
    
    void OffScreenRenderView::setFocusFrame(CefRefPtr<CefFrame> ptr) {
        m_pFocusFrame = ptr;
    }
    
    void OffScreenRenderView::setActiveElementInfo(amo::u8json& vec) {
        m_oActiveElementInfo = ActiveElementInfo(vec);
        
        if (m_oActiveElementInfo.empty()) {
            //当前节点不是输入框， 关闭输入法窗口
            //imm32_manager_->CancelIME();
            imm32_manager_->cleanupComposition();
            imm32_manager_->disableIME();
            return;
        } else {
            imm32_manager_->enableIME();	// 开启输入法
        }
    }
    
    CefRect OffScreenRenderView::logicalToDevice(const CefRect& value,
            float device_scale_factor) {
        return CefRect(logicalToDevice(value.x, device_scale_factor),
                       logicalToDevice(value.y, device_scale_factor),
                       logicalToDevice(value.width, device_scale_factor),
                       logicalToDevice(value.height, device_scale_factor));
    }
    
    int OffScreenRenderView::logicalToDevice(int value,
            float device_scale_factor) {
        float scaled_val = static_cast<float>(value) * device_scale_factor;
        return static_cast<int>(std::floor(scaled_val));
    }
    
    void OffScreenRenderView::deviceToLogical(CefMouseEvent& value,
            float device_scale_factor) {
        value.x = deviceToLogical(value.x, device_scale_factor);
        value.y = deviceToLogical(value.y, device_scale_factor);
    }
    
    int OffScreenRenderView::deviceToLogical(int value,
            float device_scale_factor) {
        float scaled_val = static_cast<float>(value) / device_scale_factor;
        return static_cast<int>(std::floor(scaled_val));
    }
#if CHROME_VERSION_BUILD >= 2357
    void OffScreenRenderView::OnCursorChange(CefRefPtr<CefBrowser> browser,
            CefCursorHandle cursor,
            CefRenderHandler::CursorType type,
            const CefCursorInfo& custom_cursor_info) {
        if (!::IsWindow(m_hWnd)) {
            return;
        }
        
        SetClassLongPtr(m_hWnd, GCLP_HCURSOR,
                        static_cast<LONG>(reinterpret_cast<LONG_PTR>(cursor)));
        SetCursor(cursor);
    }
#else
    void OffScreenRenderView::OnCursorChange(CefRefPtr<CefBrowser> browser,
            CefCursorHandle cursor) {
        if (!::IsWindow(m_hWnd)) {
            return;
        }
    
        SetClassLongPtr(m_hWnd, GCLP_HCURSOR,
                        static_cast<LONG>(reinterpret_cast<LONG_PTR>(cursor)));
        SetCursor(cursor);
    }
#endif
    
    
    
    
    bool OffScreenRenderView::StartDragging(CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefDragData> drag_data,
                                            CefRenderHandler::DragOperationsMask allowed_ops,
                                            int x,
                                            int y) {
        CEF_REQUIRE_UI_THREAD();
        
#if defined(CEF_USE_ATL)
        
        if (!drop_target_) {
            return false;
        }
        
        current_drag_op_ = DRAG_OPERATION_NONE;
        CefBrowserHost::DragOperationsMask result;
        result = drop_target_->StartDragging(browser,
                                             drag_data,
                                             allowed_ops,
                                             x,
                                             y);
        current_drag_op_ = DRAG_OPERATION_NONE;
        POINT pt = {};
        GetCursorPos(&pt);
        ::ScreenToClient(m_hWnd, &pt);
        
        browser->GetHost()->DragSourceEndedAt(deviceToLogical(pt.x,
                                              device_scale_factor_),
                                              deviceToLogical(pt.y,
                                                      device_scale_factor_),
                                              result);
        browser->GetHost()->DragSourceSystemDragEnded();
        return true;
#else
        // Cancel the drag. The dragging implementation requires ATL support.
        return false;
#endif
    }
    
    void OffScreenRenderView::UpdateDragCursor(
        CefRefPtr<CefBrowser> browser,
        CefRenderHandler::DragOperation operation) {
        CEF_REQUIRE_UI_THREAD();
        
#if defined(CEF_USE_ATL)
        current_drag_op_ = operation;
#endif
    }
    
    
    
    //void OffScreenRenderView::EnableGL() {
    //    CEF_REQUIRE_UI_THREAD();
    //
    //    PIXELFORMATDESCRIPTOR pfd;
    //    int format;
    //
    //    // Get the device context.
    //    hdc_ = GetDC(m_hWnd);
    //
    //    // Set the pixel format for the DC.
    //    ZeroMemory(&pfd, sizeof(pfd));
    //    pfd.nSize = sizeof(pfd);
    //    pfd.nVersion = 1;
    //    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    //    pfd.iPixelType = PFD_TYPE_RGBA;
    //    pfd.cColorBits = 24;
    //    pfd.cDepthBits = 16;
    //    pfd.iLayerType = PFD_MAIN_PLANE;
    //    format = ChoosePixelFormat(hdc_, &pfd);
    //    SetPixelFormat(hdc_, format, &pfd);
    //
    //    // Create and enable the render context.
    //    hrc_ = wglCreateContext(hdc_);
    //
    //    ScopedGLContext scoped_gl_context(hdc_, hrc_, false);
    //    renderer_.Initialize();
    //}
    //
    //void OffScreenRenderView::DisableGL() {
    //    CEF_REQUIRE_UI_THREAD();
    //
    //    if (!hdc_) {
    //        return;
    //    }
    //
    //    {
    //        //ScopedGLContext scoped_gl_context(hdc_, hrc_, false);
    //    }
    //
    //    renderer_.Cleanup();
    //
    //    if (::IsWindow(m_hWnd)) {
    //        // wglDeleteContext will make the context not current before deleting it.
    //        BOOL result = wglDeleteContext(hrc_);
    //        ALLOW_UNUSED_LOCAL(result);
    //        DCHECK(result);
    //        ::ReleaseDC(m_hWnd, hdc_);
    //    }
    //
    //    hdc_ = NULL;
    //    hrc_ = NULL;
    //}
    /*
    void OffScreenRenderView::Render() {
    CEF_REQUIRE_UI_THREAD();
    
    if (!hdc_) {
    EnableGL();
    }
    
    ScopedGLContext scoped_gl_context(hdc_, hrc_, true);
    renderer_.Render();
    }*/
    
#if defined(CEF_USE_ATL)
    
    CefBrowserHost::DragOperationsMask OffScreenRenderView::OnDragEnter(
        CefRefPtr<CefDragData> drag_data, CefMouseEvent ev,
        CefBrowserHost::DragOperationsMask effect) {
        if (browser_) {
            deviceToLogical(ev, device_scale_factor_);
            browser_->GetHost()->DragTargetDragEnter(drag_data, ev, effect);
            browser_->GetHost()->DragTargetDragOver(ev, effect);
        }
        
        return current_drag_op_;
    }
    
    CefBrowserHost::DragOperationsMask OffScreenRenderView::OnDragOver(
        CefMouseEvent ev,
        CefBrowserHost::DragOperationsMask effect) {
        if (browser_) {
            deviceToLogical(ev, device_scale_factor_);
            browser_->GetHost()->DragTargetDragOver(ev, effect);
        }
        
        return current_drag_op_;
    }
    
    void OffScreenRenderView::OnDragLeave() {
        if (browser_) {
            browser_->GetHost()->DragTargetDragLeave();
        }
    }
    
    CefBrowserHost::DragOperationsMask OffScreenRenderView::OnDrop(CefMouseEvent ev,
            CefBrowserHost::DragOperationsMask effect) {
        if (browser_) {
            deviceToLogical(ev, device_scale_factor_);
            browser_->GetHost()->DragTargetDragOver(ev, effect);
            browser_->GetHost()->DragTargetDrop(ev);
        }
        
        return current_drag_op_;
    }
#endif
    
    void OffScreenRenderView::getActiveElementInfo(CefRefPtr<CefFrame> pFrame) {
        if (!pFrame) {
            return;
        }
        
        std::shared_ptr<UIMessageEmitter> runner(new UIMessageEmitter(pFrame));
        runner->setValue(IPCArgsPosInfo::TransferName, "ipcRenderer");
        runner->setValue(IPCArgsPosInfo::JsFuncName, "getActiveElementInfo");
        Any result = runner->syncExecute("runJSFunction");
        amo::u8json json = result;
        setFocusFrame(pFrame);
        setActiveElementInfo(json);
    }
    
    void OffScreenRenderView::onFocusedNodeChanged(IPCMessage::SmartType msg) {
        std::vector<int64_t> identifiers;
        m_pBrowser->GetFrameIdentifiers(identifiers);
        
        if (!msg->getArgumentList()->getBool(0)) {
            amo::u8json json;
            setFocusFrame(NULL);
            setActiveElementInfo(json);
            return;
        }
        
        int nFrameID = msg->getArgumentList()->getInt(IPCArgsPosInfo::FrameID);
        CefRefPtr<CefFrame> pFrame = m_pBrowser->GetFrame(nFrameID);
        Any val = msg->getArgumentList()->getValue(1);
        
        if (val.is<amo::u8json>()) {
            setFocusFrame(pFrame);
            setActiveElementInfo(val.As<amo::u8json>());
        } else {
            getActiveElementInfo(pFrame);
        }
        
    }
    
    void OffScreenRenderView::updateCaretPos(std::shared_ptr<Gdiplus::Bitmap>
            image) {
        amo::timer t1;
        
        if (m_oActiveElementInfo.empty()) {
            return;
        }
        
        if (!m_pFocusFrame) {
            return;
        }
        
        BitmapData bmpData;
        int w = image->GetWidth();
        int h = image->GetHeight();
        int left = m_oActiveElementInfo.left;
        
        if (left < 0) {
            left = 0;
        }
        
        int top = m_oActiveElementInfo.top;
        
        if (top < 0) {
            top = 0;
        }
        
        int width = m_oActiveElementInfo.width;
        
        if (left + m_oActiveElementInfo.width > w) {
            width = w - left;
        }
        
        int height = m_oActiveElementInfo.height;
        
        if (top + m_oActiveElementInfo.height > h) {
            height = h - top;
        }
        
        Gdiplus::Rect rt(left, top, width, height);
        /*       $clog(amo::cdevel << "查找范围:" << left << ", " << top << ", " << width
                     << ", " << height << amo::endl;);*/
        Gdiplus::Status status = image->LockBits(&rt, ImageLockModeRead,
                                 PixelFormat32bppARGB, &bmpData);
                                 
        if (status != Gdiplus::Status::Ok) {
            return;
        }
        
        const BYTE* imageData = (BYTE*)bmpData.Scan0;
        int stride = bmpData.Stride;
        int bytesPerPixel = 4;
        int srcA = 3;
        int srcR = 2;
        int srcG = 1;
        int srcB = 0;
        
        int nCount = 0;
        
        bool bFind = false;
        
        /*	if (m_candidatePos.y != 0) {
        		int x = m_candidatePos.x;
        		for ()
        		{
        		}
        		const int CUR_LINE = ((y)* stride);
        		const int CUR_POS = (y * w + x);
        		BYTE r = imageData[CUR_LINE + x * bytesPerPixel + srcR];
        		BYTE g = imageData[CUR_LINE + x * bytesPerPixel + srcG];
        		BYTE b = imageData[CUR_LINE + x * bytesPerPixel + srcB];
        		BYTE a = imageData[CUR_LINE + x * bytesPerPixel + srcA];
        
        		if (b == m_oActiveElementInfo.b
        			&& r == m_oActiveElementInfo.r
        			&& g == m_oActiveElementInfo.g) {
        			++nCount;
        		}
        		else {
        			nCount = 0;
        		}
        
        	}*/
        
        
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                const int CUR_LINE = ((y) * stride);
                const int CUR_POS = (y * w + x);
                BYTE r = imageData[CUR_LINE + x * bytesPerPixel + srcR];
                BYTE g = imageData[CUR_LINE + x * bytesPerPixel + srcG];
                BYTE b = imageData[CUR_LINE + x * bytesPerPixel + srcB];
                BYTE a = imageData[CUR_LINE + x * bytesPerPixel + srcA];
                
                if (b == m_oActiveElementInfo.b
                        && r == m_oActiveElementInfo.r
                        && g == m_oActiveElementInfo.g) {
                    ++nCount;
                } else {
                    nCount = 0;
                }
                
                
                if (nCount >= m_oActiveElementInfo.fontSize) {
                    $clog(amo::cdevel << "found:" << x << ", " << y << amo::endl;);
                    m_point.x = x + left - 1;
                    m_point.y = y - m_oActiveElementInfo.fontSize + top;
                    
                    bFind = true;
                    break;
                }
            }
            
            if (bFind) {
                break;
            }
        }
        
        image->UnlockBits(&bmpData);
        imm32_manager_->updateImeWindow(m_point);
        //$clog(amo::cdevel << "查找用时t1:" << t1.elapsed() << amo::endl;);
    }
    
}


