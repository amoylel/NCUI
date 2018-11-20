#include "stdafx.h"

#include "ui/win/WebkitView.h"

#include <thread>
#include <algorithm>
#include <functional>
#include <shellapi.h>
#include <future>
#include <iostream>



#include <amo/logger.hpp>
#include <amo/loader.hpp>
#include <amo/rect.hpp>
#include <amo/file_mapping.hpp>
#include <amo/rect.hpp>
#include <amo/utility.hpp>

#include "ui/win/OffScreenRenderView.h"
#include "ui/win/MessageWindow.h"
#include "module/dll/DllManager.h"
#include "transfer/TransferMgr.h"
#include "ipc/AsyncFunctionManager.hpp"
#include "ui/win/RenderView.h"
#include "settings/BrowserWindowSettings.h"
#include "transfer/AppTransfer.h"
#include "scheme/NativeFileHandler.h"
#include "transfer/BrowserTransfer.h"
#include "transfer/FrameTransfer.h"
#include "transfer/BrowserHostTransfer.h"
#include "utility/utility.hpp"
#include "scheme/UrlResourceHandlerFactory.h"
#include <regex>
#include "ui/win/overlap/OverlapData.hpp"
#include "ui/win/overlap/DefaultOverlapData.hpp"
#include "ui/win/overlap/FilemappingOverlapData.hpp"

#include "Bitmap.hpp"
#include "ui/win/WinUserMsg.hpp"
#include <amo/directory.hpp>


namespace {
#define  MAKE_ERROR_CODE_STR(val) {val, #val}

    static std::unordered_map<int, std::string> CefErrorCodeMap = {
        MAKE_ERROR_CODE_STR(ERR_NONE),
        MAKE_ERROR_CODE_STR(ERR_FAILED),
        MAKE_ERROR_CODE_STR(ERR_ABORTED),
        MAKE_ERROR_CODE_STR(ERR_INVALID_ARGUMENT),
        MAKE_ERROR_CODE_STR(ERR_INVALID_HANDLE),
        MAKE_ERROR_CODE_STR(ERR_FILE_NOT_FOUND),
        MAKE_ERROR_CODE_STR(ERR_TIMED_OUT),
        MAKE_ERROR_CODE_STR(ERR_FILE_TOO_BIG),
        MAKE_ERROR_CODE_STR(ERR_UNEXPECTED),
        MAKE_ERROR_CODE_STR(ERR_ACCESS_DENIED),
        MAKE_ERROR_CODE_STR(ERR_NOT_IMPLEMENTED),
        MAKE_ERROR_CODE_STR(ERR_CONNECTION_CLOSED),
        MAKE_ERROR_CODE_STR(ERR_CONNECTION_RESET),
        MAKE_ERROR_CODE_STR(ERR_CONNECTION_REFUSED),
        MAKE_ERROR_CODE_STR(ERR_CONNECTION_ABORTED),
        MAKE_ERROR_CODE_STR(ERR_CONNECTION_FAILED),
        MAKE_ERROR_CODE_STR(ERR_NAME_NOT_RESOLVED),
        MAKE_ERROR_CODE_STR(ERR_INTERNET_DISCONNECTED),
        MAKE_ERROR_CODE_STR(ERR_SSL_PROTOCOL_ERROR),
        MAKE_ERROR_CODE_STR(ERR_ADDRESS_INVALID),
        MAKE_ERROR_CODE_STR(ERR_ADDRESS_UNREACHABLE),
        MAKE_ERROR_CODE_STR(ERR_SSL_CLIENT_AUTH_CERT_NEEDED),
        MAKE_ERROR_CODE_STR(ERR_TUNNEL_CONNECTION_FAILED),
        MAKE_ERROR_CODE_STR(ERR_NO_SSL_VERSIONS_ENABLED),
        MAKE_ERROR_CODE_STR(ERR_SSL_VERSION_OR_CIPHER_MISMATCH),
        MAKE_ERROR_CODE_STR(ERR_SSL_RENEGOTIATION_REQUESTED),
        MAKE_ERROR_CODE_STR(ERR_CERT_COMMON_NAME_INVALID),
        
        
        MAKE_ERROR_CODE_STR(ERR_CERT_DATE_INVALID),
        MAKE_ERROR_CODE_STR(ERR_CERT_AUTHORITY_INVALID),
        MAKE_ERROR_CODE_STR(ERR_CERT_CONTAINS_ERRORS),
        MAKE_ERROR_CODE_STR(ERR_CERT_NO_REVOCATION_MECHANISM),
        MAKE_ERROR_CODE_STR(ERR_CERT_UNABLE_TO_CHECK_REVOCATION),
        MAKE_ERROR_CODE_STR(ERR_CERT_REVOKED),
        MAKE_ERROR_CODE_STR(ERR_CERT_INVALID),
        
        // -209 is available: was ERR_CERT_NOT_IN_DNS.
        
        
        
        
        MAKE_ERROR_CODE_STR(ERR_CERT_END),
        MAKE_ERROR_CODE_STR(ERR_INVALID_URL),
        MAKE_ERROR_CODE_STR(ERR_DISALLOWED_URL_SCHEME),
        MAKE_ERROR_CODE_STR(ERR_UNKNOWN_URL_SCHEME),
        MAKE_ERROR_CODE_STR(ERR_TOO_MANY_REDIRECTS),
        MAKE_ERROR_CODE_STR(ERR_UNSAFE_REDIRECT),
        MAKE_ERROR_CODE_STR(ERR_UNSAFE_PORT),
        MAKE_ERROR_CODE_STR(ERR_INVALID_RESPONSE),
        MAKE_ERROR_CODE_STR(ERR_INVALID_CHUNKED_ENCODING),
        MAKE_ERROR_CODE_STR(ERR_METHOD_NOT_SUPPORTED),
        MAKE_ERROR_CODE_STR(ERR_UNEXPECTED_PROXY_AUTH),
        MAKE_ERROR_CODE_STR(ERR_EMPTY_RESPONSE),
        MAKE_ERROR_CODE_STR(ERR_RESPONSE_HEADERS_TOO_BIG),
        MAKE_ERROR_CODE_STR(ERR_CACHE_MISS),
        
        
#if CEF_VERSION_GE(2526)
        MAKE_ERROR_CODE_STR(ERR_CERT_BEGIN),
        MAKE_ERROR_CODE_STR(ERR_CERT_NAME_CONSTRAINT_VIOLATION),
        MAKE_ERROR_CODE_STR(ERR_CERT_NON_UNIQUE_NAME),
        MAKE_ERROR_CODE_STR(ERR_CERT_VALIDITY_TOO_LONG),
        MAKE_ERROR_CODE_STR(ERR_CERT_WEAK_KEY),
        MAKE_ERROR_CODE_STR(ERR_CERT_WEAK_SIGNATURE_ALGORITHM),
#endif
        
        MAKE_ERROR_CODE_STR(ERR_INSECURE_RESPONSE)
    };
    
    std::string getErrorStringByErrorCode(int code) {
        auto iter = CefErrorCodeMap.find(code);
        
        if (iter == CefErrorCodeMap.end()) {
            return "";
        }
        
        return iter->second;
    }
}

namespace amo {

    void WebkitView::DoEvent(TEventUI& event) {
        if (event.Type == UIEVENT_TIMER) {
            /*if (event.wParam == REPAINT_TIMER_ID
            		&& m_pBrowserSettings
            		&&  m_pBrowserSettings->offscreen) {
            	m_pBrowser->GetHost()->Invalidate(PET_VIEW);
            	return;
            }*/
            
            
            /* if (m_pBrowserSettings->transparent) {
                 Invalidate();
             } else {
                 m_pRenderWnd->needUpdate();
             }*/
            
            if (m_pBrowserSettings->transparent) {
                ::PostMessage(::GetParent(m_hBrowserWnd), WM_PAINT, NULL, NULL);
                
            } else {
                ::PostMessage(m_pRenderWnd->GetHWND(), WM_USER_PAINT, NULL, NULL);
            }
            
            
        }
        
        return LayerViewRender::DoEvent(event);
    }
    
    WebkitView::WebkitView(std::shared_ptr<BrowserWindowSettings> pBrowserSettings)
        : ClassTransfer("ipcMain") {
        m_pBrowserSettings = pBrowserSettings;
        m_pRenderWnd = NULL;
        m_hBrowserWnd = NULL;
        m_bIsClosing = false;
        m_pClientHandler = new amo::ClientHandler();
        m_hParentWnd = NULL;
        m_pBrowser = NULL;
        this->setPaintSettings(m_pBrowserSettings);
        
        if (m_pBrowserSettings->offscreen && m_pBrowserSettings->transparent &&
                m_pBrowserSettings->accelerator) {
            this->drawBackground(false);
            this->setAccelerator(true);
        }
        
    }
    
    WebkitView::~WebkitView() {
        if (m_pClientHandler) {
            m_pClientHandler->UnregisterLifeSpanHandlerDelegate(this);
            m_pClientHandler->UnregisterDelegate(this);
            m_pClientHandler->UnregisterLoadHandlerDelegate(this);
            m_pClientHandler->UnregisterDisplayHandlerDelegate(this);
            m_pClientHandler->UnregisterJSDialogHandlerDelegate(this);
            m_pClientHandler->UnregisterDownloadHandlerDelegate(this);
            m_pClientHandler->UnregisterRequestHandlerDelegate(this);
            m_pClientHandler->UnregisterContextMenuHandlerDelegate(this);
            m_pClientHandler->UnregisterDialogHandlerDelegate(this);
            m_pClientHandler->UnregisterRenderHandlerDelegate(this);
            m_pClientHandler->UnregisterKeyboardHandlerDelegate(this);
        }
        
        m_pClientHandler = NULL;
        m_pRenderWnd = NULL;
        m_pBrowser = NULL;
        
        
    }
    
    LPCTSTR WebkitView::GetClass() const {
        return _T("WebkitView");
    }
    
    
    LPVOID WebkitView::GetInterface(LPCTSTR pstrName) {
        if (_tcscmp(pstrName, _T("WebkitView")) == 0) {
            return static_cast<WebkitView*>(this);
        }
        
        return CControlUI::GetInterface(pstrName);
    }
    
    void WebkitView::SetVisible(bool bVisible) {
        CControlUI::SetVisible(bVisible);
        
        if (m_pRenderWnd) {
            ::ShowWindow(m_pRenderWnd->GetHWND(), bVisible ? SW_SHOW : SW_HIDE);
        }
    }
    
    void WebkitView::SetInternVisible(bool bVisible /*= true*/) {
        CControlUI::SetInternVisible(bVisible);
        
        if (m_pRenderWnd) {
            ::ShowWindow(m_pRenderWnd->GetHWND(), bVisible ? SW_SHOW : SW_HIDE);
        }
    }
    
    void WebkitView::DoInit() {
        AMO_TIMER_ELAPSED();
        m_hParentWnd = GetManager()->GetPaintWindow();
        //添加事件处理函数
        m_pClientHandler->RegisterLifeSpanHandlerDelegate(this);
        m_pClientHandler->RegisterLoadHandlerDelegate(this);
        m_pClientHandler->RegisterDelegate(this);
        m_pClientHandler->RegisterDisplayHandlerDelegate(this);
        m_pClientHandler->RegisterJSDialogHandlerDelegate(this);
        m_pClientHandler->RegisterDownloadHandlerDelegate(this);
        m_pClientHandler->RegisterRequestHandlerDelegate(this);
        m_pClientHandler->RegisterContextMenuHandlerDelegate(this);
        m_pClientHandler->RegisterDialogHandlerDelegate(this);
        m_pClientHandler->RegisterRenderHandlerDelegate(this);
        m_pClientHandler->RegisterKeyboardHandlerDelegate(this);
        m_pClientHandler->RegisterDisplayHandlerDelegate(this);
        this->registerFunction();
        
        if (m_pBrowserSettings->offscreen) {   //离屏
            OffScreenRenderView* window = NULL;
            window = new OffScreenRenderView(m_pBrowserSettings);
            window->setClientHandler(m_pClientHandler);								//事件ClientHandler
            m_pRenderWnd = window;
            
        } else { //非离屏
        
            RenderView* window = (new RenderView(m_pBrowserSettings));
            window->setClientHandler(m_pClientHandler);
            m_pRenderWnd = window;
        }
        
        m_pRenderWnd->Create(m_hParentWnd, NULL, UI_WNDSTYLE_CHILD,
                             NULL); //UI_WNDSTYLE_CHILD 防止闪屏
        //UpdateBrowserPos();	//设置窗口位置
        //SetPos(GetPos());
        m_hBrowserWnd = m_pRenderWnd->GetHWND();	//获取浏览器窗口句柄
        ::ShowWindow(m_pRenderWnd->GetHWND(),  SW_SHOW);
        AMO_TIMER_ELAPSED();
        return LayerViewRender::DoInit();
    }
    
    void WebkitView::SetPos(RECT rect, bool bNeedInvalidate /*= true*/) {
        LayerViewRender::SetPos(rect, bNeedInvalidate);
        
        if (::IsIconic(m_hParentWnd)) {
            // 最小化时不能改变浏览器的大小，否则还原时左上角会显示滚动条
            return;
        }
        
        /* RECT rcRect = { 0 };
         ::GetClientRect(m_hParentWnd, &rcRect);
         amo::rect rect2 = rcRect;
         rect2.width();
         amo::rect rect1 = rect;
        
         if (rect1.width() > rect2.width()) {
             rect.right = rect.left + rect2.width();
         }
        
         if (rect1.height() > rect2.height()) {
             rect.bottom = rect.left + rect2.height();
         }*/
        
        CControlUI::SetPos(rect);
        ::SetWindowPos((HWND)*m_pRenderWnd,
                       NULL,
                       rect.left,
                       rect.top,
                       rect.right - rect.left,
                       rect.bottom - rect.top,
                       NULL);
    }
    
    bool WebkitView::DoPaint(HDC hDC, const RECT& rcPaint,
                             CControlUI* pStopControl) {
                             
        if (!m_pBrowserSettings->offscreen) {
            return ViewRenderer::DoPaint(hDC, rcPaint, pStopControl);
        }
        
        if (!m_pBrowserSettings->transparent) {
        
            HWND m_hWnd = m_pRenderWnd->GetHWND();
            PAINTSTRUCT ps = { 0 };
            ::BeginPaint(m_hWnd, &ps);
            RECT rcClient;
            ::GetWindowRect(m_hWnd, &rcClient);
            int nWidth = rcClient.right - rcClient.left;
            int nHeight = rcClient.bottom - rcClient.top;
            SIZE wndSize = { rcClient.right - rcClient.left, rcClient.bottom - rcClient.top };
            HDC hDC2 = ::GetDC(m_hWnd);
            
            HDC memDC = NULL;
            
            if (memDC == NULL) {
                memDC = ::CreateCompatibleDC(hDC2);
            }
            
            BITMAPINFO bitmapinfo;
            bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bitmapinfo.bmiHeader.biBitCount = 32;
            bitmapinfo.bmiHeader.biHeight = nHeight;
            bitmapinfo.bmiHeader.biWidth = nWidth;
            bitmapinfo.bmiHeader.biPlanes = 1;
            bitmapinfo.bmiHeader.biCompression = BI_RGB;
            bitmapinfo.bmiHeader.biXPelsPerMeter = 0;
            bitmapinfo.bmiHeader.biYPelsPerMeter = 0;
            bitmapinfo.bmiHeader.biClrUsed = 0;
            bitmapinfo.bmiHeader.biClrImportant = 0;
            bitmapinfo.bmiHeader.biSizeImage = bitmapinfo.bmiHeader.biWidth
                                               * bitmapinfo.bmiHeader.biHeight
                                               * bitmapinfo.bmiHeader.biBitCount / 8;
                                               
            HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, wndSize.cx, wndSize.cy);
            
            //HBITMAP hBitmap = ::CreateDIBSection(hDC, &bitmapinfo, 0, NULL, 0, 0);
            HBITMAP hOldBitmap = (HBITMAP)::SelectObject(memDC, hBitmap);
            
            
            m_pRenderWnd->DoPaintOnMemDC(memDC, GetPos());
            RECT rc = GetPos();
            ::BitBlt(hDC, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, memDC, 0,
                     0, SRCCOPY);
                     
            DWORD wd = GetLastError();
            ::SelectObject(memDC, hOldBitmap);
            DeleteObject(hBitmap);
            
            
            ::ReleaseDC(m_hWnd, memDC);
            
            ::ReleaseDC(m_hWnd, hDC2);
            ::EndPaint(m_hWnd, &ps);
            
            
            return true;
        } else {
            return ViewRenderer::DoPaint(hDC, rcPaint, pStopControl);
        }
        
    }
    
    void WebkitView::loadURL(const std::string& url) {
        CEF_REQUIRE_UI_THREAD();
        // 不能直接LoadURL不知为什么
        CefRefPtr<CefRequest> request(CefRequest::Create());
        request->SetURL(url);
        m_pBrowser->GetMainFrame()->LoadRequest(request);
    }
    
    void WebkitView::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
        ClientHandler::RemoveBrowserByID(browser->GetIdentifier()); // 移除浏览器
        
        if (m_nBrowserID != browser->GetIdentifier()) {
            auto manager = BrowserTransferMgr::getInstance();
            manager->removeTransfer(browser->GetIdentifier(), transferName());
            return;
        }
        
        auto manager = BrowserTransferMgr::getInstance();
        manager->removeTransfer(browser->GetIdentifier(), transferName());
        
        
        BrowserTransferMgr::getInstance()->removeTransfer(browser->GetIdentifier());
        PostMessage(::GetParent(m_hBrowserWnd), WM_CLOSE, 255, 0);
        
        m_LastBitmap.reset();  // 不能放到析构函数里面去,太晚了
        m_paintingRes.clear();
        return;
    }
    
    bool WebkitView::DoClose(CefRefPtr<CefBrowser> browser) {
    
        std::vector<int64_t> vec;
        browser->GetFrameIdentifiers(vec);
        //TODO: 页面重新加载后Frame会发生改变
        auto browserMgr = TransferMappingMgr<BrowserTransfer>::getInstance();
        auto frameMgr = TransferMappingMgr<FrameTransfer>::getInstance();
        auto hostMgr = TransferMappingMgr<BrowserHostTransfer>::getInstance();
        
        for (auto& p : vec) {
            CefRefPtr<CefFrame> pFrame = browser->GetFrame(p);
            auto pTransfer = frameMgr->toTransfer(pFrame);
            ClassTransfer::removeTransfer(pTransfer->getObjectID());
            frameMgr->removeMapping(pFrame);
        }
        
        {
            auto pTransfer = browserMgr->toTransfer(browser);
            ClassTransfer::removeTransfer(pTransfer->getObjectID());
            browserMgr->removeMapping(browser);
        }
        
        {
            auto pTransfer = hostMgr->toTransfer(browser->GetHost());
            ClassTransfer::removeTransfer(pTransfer->getObjectID());
            hostMgr->removeMapping(browser->GetHost());
        }
        
        if (m_pBrowser->IsSame(browser)) {
            m_bIsClosing = true;
        }
        
        // 可能应该放到BeforeClose函数里，这里暂时还没发现问题，留意js unload函数
        // BrowserTransferMgr::getInstance()->removeTransfer(browser->GetIdentifier());
        
        
        return false;
    }
    
    void WebkitView::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
        CEF_REQUIRE_UI_THREAD();
        AMO_TIMER_ELAPSED();
        
        if (m_pBrowser) {
            //m_pDevBrowser = browser;
            
            auto manager = BrowserTransferMgr::getInstance();
            manager->addTransfer(browser->GetIdentifier(), this);
            return;
        }
        
        HWND hBrowserHostWnd = browser->GetHost()->GetWindowHandle();
        
        if (!m_pBrowserSettings->offscreen) {
            hBrowserHostWnd = ::GetParent(hBrowserHostWnd);
        }
        
        if (m_hBrowserWnd == hBrowserHostWnd) {
            m_pBrowser = browser;
            m_nBrowserID = browser->GetIdentifier();
        } else {
            return;
        }
        
        //{
        //    //设置是否允许前进后退，离屏模式下，不允许前进后退
        //    CefRefPtr<CefProcessMessage> message;
        //    message = CefProcessMessage::Create(MSG_ENABLE_BACK_FORWORD);
        //    message->GetArgumentList()->SetBool(0, false);
        //    m_pBrowser->SendProcessMessage(PID_RENDERER, message);
        //}
        //
        
        
        
        {
            //
            CefRefPtr<CefProcessMessage> message;
            message = CefProcessMessage::Create(MSG_BROWSER_SETTINGS);
            message->GetArgumentList()->SetString(0,
                                                  m_pBrowserSettings->settings.to_string());
            m_pBrowser->SendProcessMessage(PID_RENDERER, message);
        }
        
        // 设置窗口大小
        RECT rect;
        GetClientRect(m_hParentWnd, &rect);
        ::PostMessage(m_hParentWnd,
                      WM_SIZE,
                      0,
                      (LPARAM)(MAKELPARAM(rect.right, rect.bottom)));
        auto manager = BrowserTransferMgr::getInstance();
        manager->addTransfer(browser->GetIdentifier(), this);
        
        
        amo::u8directory dir(amo::u8path::getFullPathInExeDir("browser_modules"));
        dir.transfer([&](amo::u8path & p) {
            if (p.is_directory()) {
                return;
            }
            
            if (p.find_extension() != ".dll") {
                return;
            }
            
            
            amo::u8string module(p.strip_path().remove_extension(), true);
            loadExternalTransfer(module, browser);
        }, false);
        
        return;
    }
    
    void WebkitView::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               int httpStatusCode) {
                               
        //m_gThread.reset(new std::thread(std::bind(&WebkitView::foo2, this)));
        AMO_TIMER_ELAPSED();
        
        if (m_nBrowserID != browser->GetIdentifier()) {
        
            return;
        }
        
        auto appSettings = AppContext::getInstance()->getDefaultAppSettings();
        
        
        
        if (frame->IsMain()) {
        
            std::vector<amo::u8json> cssList;
            
            if (m_pBrowserSettings->cssList.is_array()) {
                cssList = m_pBrowserSettings->cssList.to_array();
            }
            
            for (auto& p : cssList) {
                std::string url = p.get<std::string>();
                
                if (url.empty()) {
                    continue;
                }
                
                std::shared_ptr<UIMessageEmitter> runner(new UIMessageEmitter(frame));
                runner->setValue(IPCArgsPosInfo::TransferName, "ipcRenderer");
                runner->setValue(IPCArgsPosInfo::TransferID, 0);
                runner->setValue(IPCArgsPosInfo::JsFuncName, "loadCSS");
                runner->execute("runJSFunction", url);
            }
            
            
            std::vector<amo::u8json> javascriptList;
            
            if (m_pBrowserSettings->javascriptList.is_array()) {
                javascriptList = m_pBrowserSettings->javascriptList.to_array();
            }
            
            for (auto& p : javascriptList) {
                std::string url = p.get<std::string>();
                
                if (url.empty()) {
                    continue;
                }
                
                std::shared_ptr<UIMessageEmitter> runner(new UIMessageEmitter(frame));
                runner->setValue(IPCArgsPosInfo::TransferName, "ipcRenderer");
                runner->setValue(IPCArgsPosInfo::TransferID, 0);
                runner->setValue(IPCArgsPosInfo::JsFuncName, "loadScript");
                runner->execute("runJSFunction", url);
            }
            
        }
        
        
        
        
        std::string url = frame->GetURL();
        
        if (!util::isDevUrl(url)) {
            // 允许页面通过document.dispatchEvent 向ipc发送数据
            std::shared_ptr<UIMessageEmitter> runner(new UIMessageEmitter(frame));
            runner->setValue(IPCArgsPosInfo::TransferName, "ipcRenderer");
            runner->setValue(IPCArgsPosInfo::TransferID, 0);
            runner->setValue(IPCArgsPosInfo::JsFuncName, "listenDocumentEvent");
            runner->execute("runJSFunction",
                            appSettings->dragClassName,
                            appSettings->noDragClassName);
        }
        
        
        
#if CHROME_VERSION_BUILD <2704
        // RenderProcess OnContextCreated 不能正确触发，导致窗口不能拖动，
        // 这里再调用一次
        
        
        if (!util::isDevUrl(url)) {
            std::shared_ptr<UIMessageEmitter> runner(new UIMessageEmitter(frame));
            runner->setValue(IPCArgsPosInfo::TransferName, "ipcRenderer");
            runner->setValue(IPCArgsPosInfo::TransferID, 0);
            runner->setValue(IPCArgsPosInfo::JsFuncName,
                             "include(\"BrowserWindow\").currentWindow.dragable");
            runner->execute("runJSFunction",
                            appSettings->dragClassName,
                            appSettings->noDragClassName);
            // 给页面一个鼠标移动的消息，触发mouseover事件
            POINT pt = { 0 };
            ::GetCursorPos(&pt);
            CefMouseEvent mouse_event;
            mouse_event.x = pt.x;
            mouse_event.y = pt.y;
            mouse_event.modifiers = 0;
            browser->GetHost()->SendMouseMoveEvent(mouse_event, false);
        }
        
        
        
#endif
        
        {
            OverlapSettings settings;
            settings.width = 1920;
            settings.height = 1080;
            settings.name = "face-overlap";
            settings.index = 9;
            settings.regions.reset(new OverlapRegions());
            
            
            amo::rect rt(0, 0, 1980, 1080);
            std::vector<amo::rect> srcRects = 	rt.split();
            amo::rect dstRect = GetPos();// (0, 0, 900, 900);
            std::vector<amo::rect> dstRects = dstRect.split();
            
            //for (size_t i = 0 ; i < dstRects.size(); ++i) {
            //    if (i == 4) {
            //        continue;
            //    }
            //
            //    OverlapRect rect;
            //    rect.dst = dstRects[i];
            //    rect.src = srcRects[i];
            //    rect.renderMode = 2;
            //    rect.index = i + 10;
            //
            //    settings.regions->m_regions.push_back(rect);
            //    //break;
            //}
            
            //{
            //    OverlapRect rect;
            //    rect.dst = amo::rect(0, 0, 100, 400);
            //    rect.src = amo::rect(0, 0, 1920, 1080);
            //    rect.renderMode = 0;
            //    rect.index = 1;
            //    settings.regions->m_regions.push_back(rect);
            //}
            //
            //
            //{
            //    OverlapRect rect;
            //    rect.dst = amo::rect(200, 0, 100, 400);
            //    rect.src = amo::rect(0, 0, 120, 1080);
            //    rect.renderMode = 1;
            //    rect.index = 2;
            //    settings.regions->m_regions.push_back(rect);
            //}
            //
            //{
            //    OverlapRect rect;
            //    rect.dst = amo::rect(400, 0, 100, 400);
            //    rect.src = amo::rect(0, 0, 1920, 1080);
            //    rect.renderMode = 2;
            //    rect.index = 3;
            //    settings.regions->m_regions.push_back(rect);
            //}
            //
            {
                OverlapRect rect;
                //rect.dst = amo::rect(0, 0, 100, 400);
                rect.src = amo::rect(0, 0, 1920, 1080);
                rect.renderMode = 1;
                rect.index = 9;
                settings.regions->m_regions.push_back(rect);
            }
            
            //
            
            settings.renderMode = 1;
            auto msg = IPCMessage::Empty();
            msg->getArgumentList()->setValue(0, settings.toJson());
            
            //addOverlap(msg);
        }
        
        {
            //browser->GetHost()->SetWindowlessFrameRate(50);
            auto msg = IPCMessage::Empty();
            msg->getArgumentList()->setValue(0, true);
            msg->getArgumentList()->setValue(1, 30);
            //repaint(msg);
            
        }
        
        
        AMO_TIMER_ELAPSED();
        
        
    }
    
    void WebkitView::OnLoadError(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 CefLoadHandler::ErrorCode errorCode,
                                 const CefString & errorText,
                                 const CefString & failedUrl) {
        if (m_nBrowserID != browser->GetIdentifier()) {
            return;
        }
        
        // Don't display an error for downloaded files.
        if (errorCode == ERR_ABORTED) {
            return;
        }
        
        // Don't display an error for external protocols that we allow the OS to
        // handle. See OnProtocolExecution().
        if (errorCode == ERR_UNKNOWN_URL_SCHEME) {
            std::string urlStr = frame->GetURL();
            
            if (urlStr.find("spotify:") == 0) {
                return;
            }
        }
        
        std::wstring url = failedUrl;
        std::transform(url.begin(), url.end(), url.begin(), ::towlower);
        
        
        
        std::string strErrorCode = std::to_string(errorCode);
        amo::u8string str(skin404, true);
        std::string errorStr = "";
        
        for (size_t i = 0; i < strErrorCode.size(); ++i) {
            errorStr += "<span>";
            errorStr += strErrorCode[i];
            errorStr += "</span>";
        }
        
        amo::u8json json;
        json.put("errorCode", errorStr);
        json.put("errorText", getErrorStringByErrorCode(errorCode));
        str = str.format(json);
        
        frame->LoadString(str.to_utf8(), failedUrl);
        //frame->LoadString(ss.str(), failedUrl);
        return;
    }
    
    bool WebkitView::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
            CefProcessId source_process, CefRefPtr<CefProcessMessage> message) {
        amo::u8string strMessageName(message->GetName().ToString(), true);
        //int nBrowserID = browser->GetIdentifier();
        
        /*  if (strMessageName == MSG_IPC_READY) {
              if (browser->GetIdentifier() == m_pBrowser->GetIdentifier()) {
                  ClientHandler::RegisterBrowser(browser);
              }
        
              return true;
          }*/
        
        // 只处理这三种消息
        if (strMessageName == MSG_NATIVE_EXECUTE
                || strMessageName == MSG_NATIVE_SYNC_EXECUTE
                || strMessageName == MSG_NATIVE_ASYNC_EXECUTE) {
            IPCMessage::SmartType ipcMessage = createAnyProcessMessage(message);
            //将bigstr转换为string
            auto args = ipcMessage->getArgumentList();
            int len = args->getArgsSize();
            
            for (int i = 0; i < len; ++i) {
                if (!args->getValue(i).is<amo::bigstr>()) {
                    continue;
                }
                
                amo::bigstr bigstr = args->getValue(i).As<amo::bigstr>();
                args->setValue(i, bigstr.read());
            }
            
            auto manager = BrowserTransferMgr::getInstance();
            
            if (manager->onMessageTransfer(ipcMessage).isValid()) {
                return true;
            }
        }
        
        return false;
    }
    
    Any WebkitView::asyncExecuteResult(IPCMessage::SmartType msg) {
    
        int id = msg->getArgumentList()->getInt(IPCArgsPosInfo::AsyncCallback);
        auto item = AsyncFunctionManager<PID_BROWSER>::getInstance()->get(id);
        
        if (item) {
            item(msg->getArgumentList()->getValue(0));
        }
        
        using MGR = AsyncFunctionManager < PID_BROWSER > ;
        MGR::getInstance()->unRegisterCallbackFunction(id);
        return Undefined();
    }
    
    void WebkitView::registerExternalTransfer(int nBrowserID,
            std::shared_ptr<ClassTransfer> pTransfer) {
        // 注册外部模块到程序中
        BrowserTransferMgr::getInstance()->addTransfer(nBrowserID, pTransfer);
        // TODO: UI线程同时加载多个外部模块
    }
    
    
    
    void WebkitView::triggerEventOnUIThread(IPCMessage::SmartType msg) {
        CefPostTask(TID_UI,
                    base::Bind(&WebkitView::triggerEventOnUIThreadImpl,
                               this,
                               msg));
    }
    
    void WebkitView::triggerEventOnUIThreadImpl(IPCMessage::SmartType msg) {
        // 向页面发送消息
        std::shared_ptr<UIMessageEmitter> runner(new UIMessageEmitter());
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        int64_t nFrameID = args->getInt64(IPCArgsPosInfo::FrameID);
        CefRefPtr<CefFrame> pFrame = ClientHandler::GetFrameByID(nFrameID);
        runner->setIPCMessage(msg);
        runner->setFrame(pFrame);
        runner->execute();
    }
    
    
    void WebkitView::showDevTools() {
    
    
        auto appSettings = AppContext::getInstance()->getDefaultAppSettings();
        
        // 非调试模式下不能打开调试窗口
        if (!appSettings->debugMode) {
            return;
        }
        
        CefWindowInfo windowInfo;
        CefBrowserSettings settings;
        windowInfo.SetAsPopup(m_pBrowser->GetHost()->GetWindowHandle(),
                              "DevTools");
                              
#if CHROME_VERSION_BUILD >= 2272
        CefPoint pt;
        m_pBrowser->GetHost()->ShowDevTools(windowInfo,
                                            m_pClientHandler,
                                            settings,
                                            pt);
#else
        m_pBrowser->GetHost()->ShowDevTools(windowInfo,
                                            m_pClientHandler,
                                            settings);
#endif
        return ;
    }
    
    void WebkitView::closeDevTools() {
        m_pBrowser->GetHost()->CloseDevTools();
    }
    
    
    bool WebkitView::loadExternalTransfer(const std::string& u8DllName,
                                          CefRefPtr<CefBrowser> browser) {
        if (!browser) {
            return false;
        }
        
        AMO_TIMER_ELAPSED();
        // 从磁盘中加载与所给模块同名dll
        std::shared_ptr<amo::loader> pLoader;
        pLoader = DllManager<PID_BROWSER>::getInstance()->load(amo::u8string(
                      u8DllName, true));
                      
                      
        if (!pLoader) {
            return NULL;
        }
        
        // 外部模块必须提供registerTransfer函数
        int nBrowserID = browser->GetIdentifier();
        std::shared_ptr< TransferRegister> info(new TransferRegister());
        info->nBrowserID = nBrowserID;
        info->fnCallback = std::bind(&WebkitView::registerExternalTransfer,
                                     this,
                                     std::placeholders::_1,
                                     std::placeholders::_2);
        info->moduleName = u8DllName;
        
        auto options = pLoader->exec<bool, std::shared_ptr<TransferRegister>>(
                           "registerTransfer",
                           info);
                           
        // 判断外部模块是否注册成功
        if (!options || !*options) {
            return false;
        }
        
        AMO_TIMER_ELAPSED();
        return true;
    }
    
    Any WebkitView::onInclude(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        std::string strClass = args->getString(0);
        int nBrowserID = args->getInt(IPCArgsPosInfo::BrowserID);
        
        auto mananger = BrowserTransferMgr::getInstance();
        TransferMap& transferMap = mananger->getTransferMap(nBrowserID);
        Transfer* pTransfer = transferMap.findTransfer(strClass);
        FunctionWrapperMgr& mgr = pTransfer->getFuncMgr();
        return mgr.toJson();
        //
        //
        //// 从磁盘中加载与所给模块同名dll
        //std::shared_ptr<amo::loader> pLoader;
        //pLoader = DllManager<PID_BROWSER>::getInstance()->load(amo::u8string(strClass,
        //          true));
        //
        //if (!pLoader) {
        //    return Undefined();
        //}
        //
        //// 外部模块必须提供registerTransfer函数
        //int nBrowserID = args->getInt(IPCArgsPosInfo::BrowserID);
        //
        //int nBrowserID = context->GetBrowser()->GetIdentifier();
        //int64_t nFrameID = context->GetFrame()->GetIdentifier();
        //
        //if (m_oRegisteredSet.find(nBrowserID) == m_oRegisteredSet.end()) {
        //    m_oRegisteredSet.insert(nBrowserID);
        //    amo::u8directory dir(amo::u8path::getFullPathInExeDir("renderer_modules"));
        //    dir.transfer([&](amo::u8path & p) {
        //        if (p.is_directory()) {
        //            return;
        //        }
        //
        //        if (p.find_extension() != ".dll") {
        //            return;
        //        }
        //
        //
        //        amo::u8string module(p.strip_path().remove_extension(), true);
        //        loadExternalTransfer(module, pBrowser);
        //    }, false);
        //
        //}
        //
        //
        //std::shared_ptr< TransferRegister> info(new TransferRegister());
        //info->nBrowserID = nBrowserID;
        //info->fnCallback = std::bind(&WebkitView::registerExternalTransfer,
        //                             this,
        //                             std::placeholders::_1,
        //                             std::placeholders::_2);
        //
        //
        //
        //auto options = pLoader->exec<bool, std::shared_ptr<TransferRegister>>(
        //                   "registerTransfer",
        //                   info);
        //
        //
        //
        //// 判断外部模块是否注册成功
        //if (!options || !*options) {
        //    amo::u8string dllName(strClass, true);
        //    std::vector<amo::u8string> vec;
        //    // 从dll中导出c函数为JS函数
        //    std::shared_ptr<amo::loader> pLoader;
        //    pLoader = DllManager<PID_BROWSER>::getInstance()->load(dllName);
        //
        //    if (!pLoader) {
        //        return Undefined();
        //    }
        //
        //    vec = DllManager<PID_BROWSER>::getInstance()->exports(dllName);
        //
        //    amo::u8json jsonArr;
        //    jsonArr.set_array();
        //
        //    for (auto& p : vec) {
        //        jsonArr.push_back(p.to_utf8());
        //    }
        //
        //    return jsonArr;
        //
        //} else {
        //    auto mananger = BrowserTransferMgr::getInstance();
        //    TransferMap& transferMap = mananger->getTransferMap(nBrowserID);
        //    Transfer* pTransfer = transferMap.findTransfer(strClass);
        //    FunctionWrapperMgr& mgr = pTransfer->getFuncMgr();
        //    return mgr.toJson();
        //}
        
    }
    
    Any WebkitView::createPipeClient(IPCMessage::SmartType msg) {
        return ClientHandler::createPipeClientImpl(msg);
    }
    
    Any WebkitView::repaint(IPCMessage::SmartType msg) {
        bool killPaintTimer = !msg->getArgumentList()->getBool(0);
        int delay = msg->getArgumentList()->getInt(1);
        
        // 不管之前有没有计时器，先删掉再说
        m_pManager->KillTimer(this, REPAINT_TIMER_ID);
        
        if (delay > 0) {
            if (delay < 30) {
                delay = 30;
            }
            
            m_pManager->SetTimer(this, REPAINT_TIMER_ID, delay);
        }
        
        if (!killPaintTimer && m_pBrowser) {
            // 如果不是停止重绘，那么立即重绘一次,只在离屏下重绘
            if (m_pBrowserSettings->offscreen) {
                m_pBrowser->GetHost()->Invalidate(PET_VIEW);
            }
        }
        
        return true;
    }
    
    Any WebkitView::addOverlap(IPCMessage::SmartType msg) {
    
    
        amo::u8json settings = msg->getArgumentList()->getJson(0);
        std::string name = settings.getString("name");
        
        if (name.empty()) {
            return false;
        }
        
        std::shared_ptr<amo::file_mapping> map(new amo::file_mapping(amo::string(name,
                                               true), true));
        std::shared_ptr<Overlap> overlap(new Overlap(settings));
        std::shared_ptr<OverlapData> overlapData(new FilemappingOverlapData(map));
        overlapData->fill(map->address(), map->size());
        overlap->setOverlapData(overlapData);
        
        if (m_paintingRes.find(name) == m_paintingRes.end()) {
            m_paintingRes[name] = { overlap, map };
        }
        
        return true;
    }
    
    Any WebkitView::removeOverlap(IPCMessage::SmartType msg) {
        std::string name = amo::u8string(msg->getArgumentList()->getString(0),
                                         true).str();
                                         
        if (name.empty()) {
            return false;
        }
        
        m_paintingRes.erase(name);
        return true;
    }
    
    Any WebkitView::onMessageTransfer(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        // 还要处理调试窗口
        /*  int nBrowserID = args->getInt(IPCArgsPosInfo::BrowserID);
        
          if (m_nBrowserID != nBrowserID) {
              return false;
          }*/
        
        return Transfer::onMessageTransfer(msg);
    }
    
    std::string commandToAction(int commandID) {
        switch (commandID) {
        case 100:
            return "goBack";
            
        case 101:
            return "goForward";
            
        case 110:
            return "undo";
            
        case 111:
            return "redo";
            
        case 112:
            return "cut";
            
        case 113:
            return "copy";
            
        case 114:
            return "paste";
            
        case 115:
            return "delete";
            
        case 116:
            return "selectAll";
            
        case 131:
            return "print";
            
        case 132:
            return "viewResource";
            
        default:
            break;
        }
        
        return "";
    }
    
    bool commandIsEnabled(int commandID, int flag) {
    
    
    
        switch (commandID) {
        /*case 100:
        	return "goBack";
        
        case 101:
        	return "goForward";*/
        
        case 110:
            return (flag & 0x1) != 0;
            
        case 111:
            return (flag & (0x1 << 1)) != 0;
            
        case 112:
            return (flag & (0x1 << 2)) != 0;
            
        case 113:
            return (flag & (0x1 << 3)) != 0;
            
        case 114:
            return (flag & (0x1 << 4)) != 0;
            
        case 115:
            return (flag & (0x1 << 5)) != 0;
            
        case 116:
            return (flag & (0x1 << 6)) != 0;
            
            
        default:
            break;
        }
        
        return true;
    }
    
    void WebkitView::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                         CefRefPtr<CefFrame> frame,
                                         CefRefPtr<CefContextMenuParams> params,
                                         CefRefPtr<CefMenuModel> model) {
        if (m_nBrowserID != browser->GetIdentifier()) {
            return;
        }
        
        
        /*  type:1, value : 撤消(&U), keycode : 261360976, shift : true, ctrl : true, alt : true, command : 110
        	type : 1, value : 重做(&R), keycode : 261360976, shift : true, ctrl : true, alt : true, command : 111
        	type : 4, value : , keycode : 261360976, shift : true, ctrl : true, alt : true, command : -1
        	type : 1, value : 剪切(&T), keycode : 261360976, shift : true, ctrl : true, alt : true, command : 112
        	type : 1, value : 复制(&C), keycode : 261360976, shift : true, ctrl : true, alt : true, command : 113
        	type : 1, value : 粘贴(&P), keycode : 261360976, shift : true, ctrl : true, alt : true, command : 114
        	type : 1, value : 删除(&D), keycode : 261360976, shift : true, ctrl : true, alt : true, command : 115
        	type : 4, value : , keycode : 261360976, shift : true, ctrl : true, alt : true, command : -1
        	type : 1, value : 全选(&A), keycode : 261360976, shift : true, ctrl : true, alt : true, command : 116
        	type : 4, value : , keycode : 261360976, shift : true, ctrl : true, alt : true, command : -1
        	type : 1, value : radio, keycode : 261360976, shift : true, ctrl : true, alt : true, command : 200
        	type : 4, value : , keycode : 261360976, shift : true, ctrl : true, alt : true, command : -1
        	type : 1, value : 添加到字典中(&A), keycode : 261360976, shift : true, ctrl : true, alt : true, command : 206
        
        	type:1, value:返回(&B), keycode:259263824, shift:true, ctrl:true, alt:true, command:100
        	type:1, value:前进(&F), keycode:259263824, shift:true, ctrl:true, alt:true, command:101
        	type:4, value:, keycode:259263824, shift:true, ctrl:true, alt:true, command:-1
        	type:1, value:打印(&P)..., keycode:259263824, shift:true, ctrl:true, alt:true, command:131
        	type:1, value:查看源代码(&O), keycode:259263824, shift:true, ctrl:true, alt:true, command:132*/
        
        
        if ((params->GetTypeFlags() & (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_FRAME)) != 0) {
            //if (model->GetCount() > 0) model->AddSeparator();						//添加分割符
            // 不显示含有打印及查看源码的菜单, （只显示Edit类菜单）
            int k = 0;
            int count = model->GetCount();
            
            if (count == 0) {
                return;
            }
            
            amo::u8json json;
            amo::u8json menu;
            menu.set_array();
            //model->SetAcceleratorAt(0, 'C', true, true, true);
            
            
            for (int i = 0; i < count; i++) {
                int m = model->GetCommandIdAt(i);
                
                if (m == 200) { // 200 是一个radio?? ，BUG?
                    continue;
                }
                
                if (m == 131 || m == 132) { // 131-打印，132-查看源码
                    k++;
                }
                
                
                int key_code;
                bool shift;
                bool ctrl;
                bool alt;
                model->GetAcceleratorAt(i, key_code, shift, ctrl, alt);
                $cdevel("type:{}, value:{}, keycode:{}, shift:{}, ctrl:{}, alt:{},",
                        (int)model->GetTypeAt(i), amo::u8string(model->GetLabelAt(i).ToString(),
                                true).to_ansi(), key_code, shift, ctrl, alt);
                                
                /*MENUITEMTYPE_NONE,
                MENUITEMTYPE_COMMAND,
                MENUITEMTYPE_CHECK,
                MENUITEMTYPE_RADIO,
                MENUITEMTYPE_SEPARATOR,
                MENUITEMTYPE_SUBMENU,*/
                CefMenuModel::MenuItemType  type = model->GetTypeAt(i);
                
                switch (type) {
                case	MENUITEMTYPE_NONE:
                    break;
                    
                case	MENUITEMTYPE_COMMAND: {
                    std::string label = model->GetLabelAt(i);
                    
                    auto ssss = amo::string_utils::utf8_to_wide(label);
                    int commandid = model->GetCommandIdAt(i);
                    int editFlags = params->GetEditStateFlags();
                    std::string shortcut;
                    std::regex reg1("\\(&[a-zA-Z]\\)");
                    std::regex reg2("&[a-zA-Z]");
                    std::smatch m1;
                    std::smatch m2;
                    
                    if (std::regex_search(label, m1, reg1)) {
                        shortcut = m1[0].str();
                        amo::string_utils::trim_left(shortcut, "(&");
                        amo::string_utils::trim_right(shortcut, ")");
                        label = std::regex_replace(label, reg1, "");
                        
                    } else if (std::regex_search(label, m2, reg2)) {
                        shortcut = m2[0].str();
                        amo::string_utils::trim_left(shortcut, "&");
                        label = std::regex_replace(label, std::regex("&"), "");
                    }
                    
                    label = std::regex_replace(label, std::regex("&"), "");
                    
                    //while (std::regex_search(label, m1, reg1)) {
                    //    shortcut = m1[0].str();
                    //    amo::string_utils::trim_left(shortcut, "(");
                    //    amo::string_utils::trim_right(shortcut, ")");
                    //    shortcut = shortcut.substr(1);
                    //    //shortcut = shortcut.substr(0, shortcut.size() - 1);
                    //    break;
                    //}
                    //
                    //label = std::regex_replace(label, reg, "");
                    
                    amo::u8json item;
                    item.put("id", std::to_string(commandid));
                    item.put("text", label);
                    
                    if (shortcut.size() == 1) {
                        item.put("shortcut", shortcut);
                    }
                    
                    std::string action = commandToAction(commandid);
                    
                    
                    
                    if (i < count - 1 && (model->GetTypeAt(i + 1) == MENUITEMTYPE_SEPARATOR)) {
                        item.put("separator", true);
                    }
                    
                    if (!action.empty()) {
                        item.put("action", action);
                    }
                    
                    bool enabled = commandIsEnabled(commandid, editFlags);
                    item.put("enabled", enabled);
                    menu.push_back(item);
                }
                
                case	MENUITEMTYPE_CHECK:
                case	MENUITEMTYPE_RADIO:
                case	MENUITEMTYPE_SEPARATOR:
                case	MENUITEMTYPE_SUBMENU:
                default:
                    break;
                }
                
            }
            
            //return;
            model->Clear(); // 移除原有菜单
            
            if (k == 2) {
                return;
            }
            
            json.put("menu", menu);
            
            std::string ssss = json.to_string();
            std::wstring ws3 = amo::string_utils::utf8_to_wide(ssss);
            auto menuTransfer = ClassTransfer::getUniqueTransfer<MenuTransfer>();
            std::shared_ptr<IPCMessage> msg = IPCMessage::Empty();
            
            msg->getArgumentList()->setValue(0, json);
            
            menuTransfer->onCreateClass(msg);
        }
        
        return;
    }
    
    void WebkitView::OnBeforeDownload(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefDownloadItem> download_item,
                                      const CefString & suggested_name,
                                      CefRefPtr<CefBeforeDownloadCallback> callback) {
        //离屏模式下下载功能被FILECACHE接管了
        //离屏下不接管下载文件选择对话框
        if (m_nBrowserID != browser->GetIdentifier() || m_pBrowserSettings->offscreen) {
            return;
        }
        
        //获取默认下载文件夹，设置下载路径
        std::string path;
        TCHAR szFolderPath[MAX_PATH] = { 0 };
        
        //由用户选择目录保存文件
        if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE,
                                      NULL, 0, szFolderPath))) {
            path = amo::u8string(szFolderPath);
            path += std::string("\\") + amo::u8string(suggested_name.ToString(),
                    true).to_utf8();
        }
        
        callback->Continue(amo::u8string(path, true).to_utf8(), true);
        return;
    }
    
    
    bool WebkitView::OnFileDialog(CefRefPtr<CefBrowser> browser,
                                  CefDialogHandler::FileDialogMode mode,
                                  const CefString& title,
                                  const CefString& default_file_path,
                                  const std::vector<CefString>& accept_filters,
                                  int selected_accept_filter,
                                  CefRefPtr<CefFileDialogCallback> callback) {
        return false;
        std::vector<CefString> vec;
        vec.push_back("D:\\2222.jpg");
        callback->Continue(selected_accept_filter, vec);
        return true;
    }
    
#if CHROME_VERSION_BUILD >= 2704
    bool WebkitView::OnJSDialog(CefRefPtr<CefBrowser> browser,
                                const CefString & origin_url,
                                CefJSDialogHandler::JSDialogType dialog_type,
                                const CefString & message_text,
                                const CefString & default_prompt_text,
                                CefRefPtr<CefJSDialogCallback> callback,
                                bool & suppress_message) {
                                
        return OnJSDialog2(browser, origin_url, dialog_type, message_text,
                           default_prompt_text, callback, suppress_message);
    }
#else
    bool WebkitView::OnJSDialog(CefRefPtr<CefBrowser> browser,
                                const CefString & origin_url,
                                const CefString & accept_lang,
                                CefJSDialogHandler::JSDialogType dialog_type,
                                const CefString & message_text,
                                const CefString & default_prompt_text,
                                CefRefPtr<CefJSDialogCallback> callback,
                                bool & suppress_message) {
    
        return OnJSDialog2(browser, origin_url, dialog_type, message_text,
                           default_prompt_text, callback, suppress_message);
    }
    
    
    
    
    
#endif
    bool WebkitView::OnJSDialog2(CefRefPtr<CefBrowser> browser,
                                 const CefString & origin_url,
                                 CefJSDialogHandler::JSDialogType dialog_type,
                                 const CefString & message_text,
                                 const CefString & default_prompt_text,
                                 CefRefPtr<CefJSDialogCallback> callback,
                                 bool & suppress_message) {
        //接管JS 弹出框 ，显示自定义JS弹出框
        CEF_REQUIRE_UI_THREAD();
        amo::u8string strMessageText(message_text.ToString(), true);
        amo::u8string strOriginUrl(origin_url.ToString(), true);
        
        if (m_nBrowserID != browser->GetIdentifier()) {
            return false;
        }
        
        UINT uRet = 0;
        CDuiString strPromptText = default_prompt_text.c_str();
        amo::u8json settings = m_pBrowserSettings->getThemeJson();
        
        if (dialog_type == JSDIALOGTYPE_ALERT) { //alert
            strOriginUrl = amo::u8string(u8"提示", true);
            uRet = MessageWindow::Show(::GetParent(m_hBrowserWnd),
                                       settings,
                                       strMessageText.to_unicode().c_str(),
                                       strOriginUrl.to_unicode().c_str());
            callback->Continue(uRet == 1,
                               amo::u8string(strPromptText.GetData()).to_utf8());
            return true;
        } else if (dialog_type == JSDIALOGTYPE_CONFIRM) { //confirm
            strOriginUrl = amo::u8string(u8"询问", true);
            uRet = MessageWindow::Show(::GetParent(m_hBrowserWnd),
                                       settings,
                                       strMessageText.to_unicode().c_str(),
                                       strOriginUrl.to_unicode().c_str(),
                                       MB_OKCANCEL);
            callback->Continue(uRet == 1,
                               amo::u8string(strPromptText.GetData()).to_utf8());
            return true;
        } else if (dialog_type == JSDIALOGTYPE_PROMPT) { //prompt
            strOriginUrl = strMessageText;
            uRet = MessageWindow::ShowPrompt(::GetParent(m_hBrowserWnd),
                                             settings,
                                             strMessageText.to_unicode().c_str(),
                                             &strPromptText,
                                             strOriginUrl.to_unicode().c_str(),
                                             MB_OKCANCEL);
            callback->Continue(uRet == 1,
                               amo::u8string(strPromptText.GetData()).to_utf8());
            return true;
        }
        
        return false;
    }
    
    bool WebkitView::OnBeforeUnloadDialog(CefRefPtr<CefBrowser> browser,
                                          const CefString & message_text,
                                          bool is_reload,
                                          CefRefPtr<CefJSDialogCallback> callback) {
                                          
                                          
        // 如果页面回返了true of false,那么直接向cef返回结果
        if (message_text == "true"
                || message_text == "True"
                || message_text == "1"
                || message_text == "TRUE") {
            return true;
        } else if (message_text == "false"
                   || message_text == "False"
                   || message_text == "0"
                   || message_text == "FALSE") {
            m_bIsClosing = false;
            return true;
        }
        
        //接管JS 弹出框 ，显示自定义JS弹出框 页面退出时的询问框 unload
        if (m_nBrowserID != browser->GetIdentifier()) {
            return false;
        }
        
        CDuiString strCaption = _T("询问");
        
        amo::u8json settings = m_pBrowserSettings->getThemeJson();
        
        UINT uRet = MessageWindow::Show(::GetParent(m_hBrowserWnd),
                                        settings,
                                        message_text.c_str(),
                                        strCaption.GetData(),
                                        MB_OKCANCEL);
        callback->Continue(uRet == 1, message_text.c_str());
        
        // 没有能关闭页面
        if (uRet != 1) {
            m_bIsClosing = false;
        }
        
        return true;
    }
    
    
    void WebkitView::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
            CefRequestHandler::TerminationStatus status) {
        if (m_nBrowserID != browser->GetIdentifier()) {
            return;
        }
        
        if (status == TS_PROCESS_CRASHED) {
            amo::u8json settings = m_pBrowserSettings->getThemeJson();
            MessageWindow::Show(m_hBrowserWnd,  settings, _T("the page crashed."));
            
        }
        
        return;
    }
    
    
    
    CefRefPtr<CefBrowser> WebkitView::getBrowser() {
        return m_pBrowser;
    }
    
#if CEF_VERSION_REGION(3325, 10000)
    bool WebkitView::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefRequest> request,
                                    bool is_redirect) {
        // RenderProcessHandler::OnBeforeNavigation 无法使用后的替代方式，并不完美
        
        //CefRequest::TransitionType type = (int)request->GetTransitionType();
        int type = (int)request->GetTransitionType();
        
        
        if ((type & TT_RELOAD) != 0 && !m_pBrowserSettings->reload) {
            return true;
        }
        
        if ((type & TT_FORWARD_BACK_FLAG) != 0 && !m_pBrowserSettings->back_forword) {
            return true;
        }
        
        return false;
    }
#endif
    
    void WebkitView::OnLoadStart(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame) {
        AMO_TIMER_ELAPSED();
        // 加载页面时
        
        
        if (frame->IsMain()) {
            CefString url = frame->GetURL();
            
            if (browser->IsSame(m_pBrowser)) {
                m_paintingRes.clear();
            }
            
            if (url == "chrome-devtools://devtools/inspector.html") {
            
            } else {
            
            }
        }
        
    }
    
    
    void WebkitView::OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        const CefString& old_url,
                                        CefString& new_url) {
        // 死循环
        /* std::string url = old_url.ToString();
         std::string retval = UrlResourceHandlerFactory::getInstance()->redirectUrl(
        						  url);
        
         if (!retval.empty()) {
        	 new_url = retval;
         }*/
        
    }
    
    
    CefRefPtr<CefResourceHandler> WebkitView::GetResourceHandler(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefRequest> request) {
        
        std::string url = request->GetURL();
        
        return UrlResourceHandlerFactory::getInstance()->create(url);
        
        
        
    }
    
    
    bool WebkitView::OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
                                   const CefKeyEvent & event,
                                   CefEventHandle os_event,
                                   bool * is_keyboard_shortcut) {
        if (m_nBrowserID != browser->GetIdentifier()) {
            return false;
        }
        
        if (KEYEVENT_RAWKEYDOWN != event.type) {
            return false;
        }
        
        
        auto appSettings = AppContext::getInstance()->getDefaultAppSettings();
        
        if (appSettings->debugMode) {
            if (event.windows_key_code == VK_F12) {
                showDevTools();
                return true;
            }
            
            if (event.windows_key_code == VK_F5) {
                m_pBrowser->ReloadIgnoreCache();
                return true;
            }
            
        }
        
        if (event.windows_key_code == VK_ESCAPE) {
            ::PostMessage(m_hParentWnd, WM_KEYDOWN, VK_ESCAPE, NULL);
        }
        
        return false;
    }
    
    bool WebkitView::OnTooltip(CefRefPtr<CefBrowser> browser, CefString& text) {
        amo::u8string str(text.ToString(), true);
        //this->SetToolTip(str.to_unicode().c_str());
        
        if (m_pRenderWnd != NULL) {
            m_pRenderWnd->setTooltip(str.to_wide());
            return false;
        }
        
        return false;
    }
    
    CefRefPtr<amo::ClientHandler> WebkitView::getClientHandler() {
        return m_pClientHandler;
    }
    
    HWND WebkitView::GetNativeWindow() const {
        return m_hBrowserWnd;
    }
    
    Any WebkitView::focusedNodeChanged(IPCMessage::SmartType msg) {
        m_pRenderWnd->onFocusedNodeChanged(msg);
        return Undefined();
    }
    
    
    void WebkitView::OnPaint(CefRefPtr<CefBrowser> browser,
                             CefRenderHandler::PaintElementType type,
                             const CefRenderHandler::RectList & dirtyRects,
                             const void* buffer,
                             int width,
                             int height) {
        CEF_REQUIRE_UI_THREAD();
        
        
        amo::rect rc;
        rc.intersect(amo::rect());
        
        if (!m_pBrowserSettings->offscreen) {
            return;    //  只画透明窗口
        }
        
        
        if (m_pBrowserSettings->compute_cursor) {
            std::shared_ptr<Gdiplus::Bitmap> image;
            image.reset(new Gdiplus::Bitmap(width,
                                            height,
                                            width * 4,
                                            PixelFormat32bppARGB,
                                            (BYTE*)buffer));
            m_pRenderWnd->updateCaretPos(image);
            
        }
        
        /* if (m_pBrowserSettings->transparent) {
        	 std::shared_ptr<Gdiplus::Bitmap> image;
        	 image.reset(new Gdiplus::Bitmap(width,
        									 height,
        									 width * 4,
        									 PixelFormat32bppARGB,
        									 (BYTE*)buffer));
        
        	 amo::string path(amo::date_time().format("yyyyMMdd_hh_mm_ss_ms.png"), false);
        	 amo::SaveBitmapToFile(image.get(), path.to_unicode().c_str(), L"image/png");
        
         }*/
        
        std::shared_ptr<PaintResource> resource(new PaintResource());
        
        
        std::shared_ptr<  OverlapSettings > settings(new OverlapSettings());
        settings->width = width;
        settings->height = height;
        settings->name = "default";
        settings->length = width * height * 4;
        settings->type = 0;
        settings->index = 5;
        
        std::shared_ptr<OverlapData> overlapData(new DefalutOverlapData());
        overlapData->fill((char*)buffer, settings->length);
        
        
        std::shared_ptr<Overlap> overlap(new Overlap(settings));
        
        overlap->setOverlapData(overlapData);
        
        
        for (auto& item : m_paintingRes) {
            resource->addOverlap(item.second.first);
        }
        
        resource->addOverlap(overlap);
        //insertBitmap(resource);
        
        if (m_pBrowserSettings->transparent) {
            insertBitmap(resource);
        } else {
            m_pRenderWnd->insertBitmap(resource);
        }
        
    }
    
}

