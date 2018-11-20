#include "stdafx.h"

#include "ui/win/BrowserWindow.h"

#include <commdlg.h>
#include <string>
#include <shellapi.h>
#include <windowsx.h>
#include <shlobj.h>

#include "handler/CefHeader.hpp"

#include <amo/logger.hpp>
#include <amo/string.hpp>


#include <amo/string.hpp>

#include <Windows.h>
#include <windowsx.h>
#include <iostream>

#include "context/AppContext.h"
#include "ui/win/WebkitView.h"
#include "transfer/BrowserWindowTransfer.h"
#include "transfer/TransferMgr.h"
#include "ui/win/BrowserWindowSkin.h"
#include "ui/win/tray/Tray.h"
#include "ipc/UIMessageBroadcaster.hpp"
#include "transfer/NativeWindowTransfer.h"
#include "transfer/GlobalShortcutTransfer.h"
#include "transfer/ShellTransfer.h"
#include "transfer/FrameTransfer.h"
#include "transfer/TransferMappingMgr.hpp"
#include "transfer/AppTransfer.h"
#include "transfer/BrowserTransfer.h"
#include "node/CefCallbackHandler.h"
#include "settings/BrowserWindowSettings.h"
#include "ui/win/clipboard/Clipboard.h"

#include "ui/win/Bitmap.hpp"




#ifndef SPI_GETWINARRANGING
#define SPI_GETWINARRANGING 0x0082
#endif

#ifndef SPI_SETWINARRANGING
#define SPI_SETWINARRANGING 0x0083
#endif

#ifndef SPI_GETSNAPSIZING
#define SPI_GETSNAPSIZING   0x008E
#endif

#ifndef SPI_SETSNAPSIZING
#define SPI_SETSNAPSIZING   0x008F
#endif

#include <zip.h>
#include "utility/libzippp.h"

#include <amo/filestream.hpp>
#include <amo/file.hpp>

#ifndef WM_GIF_RECORD_TIMER
#define WM_GIF_RECOFD_TIMER (WM_USER + 1232)
#endif

namespace amo {

    static LPCWSTR kParentWndProc = L"CefParentWndProc";
    static LPCWSTR kDraggableRegion = L"CefDraggableRegion";
    
    BrowserWindow::BrowserWindow(std::shared_ptr<BrowserWindowSettings>
                                 pBrowserSettings)
        : LocalWindow(pBrowserSettings)
        , m_pWebkit(NULL)
        , m_pBrowserSettings(pBrowserSettings) {
        AMO_TIMER_ELAPSED();
        m_pNativeSettings->id = amo::u8string::from_number(getObjectID()).to_utf8();
        m_pTitleBar = NULL;
        m_pBrowserLayout = NULL;
        m_gifRecordTimer = 0;
        m_pCefCallbackHandler.reset(new CefCallbackHandler(getObjectID()));
        setNativeWindow(false);
        draggable_region_ = ::CreateRectRgn(0, 0, 0, 0);
        
        auto pWindow = ClassTransfer::getUniqueTransfer<BrowserWindowTransfer>();
        int64_t nClassID = pWindow->getObjectID();
        setTransferClassID(nClassID);
        setTransferObjectID(getObjectID());
        
    }
    
    BrowserWindow::~BrowserWindow() {
        ::DeleteObject(draggable_region_);
        removeTransfer(getObjectID());
        amo::cdevel << func_orient << amo::endl;
    }
    
    
    LRESULT BrowserWindow::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                         BOOL& bHandled) {
                                         
        return LocalWindow::OnLButtonDown(uMsg, wParam, lParam, bHandled);
    }
    
    LRESULT BrowserWindow::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                       BOOL& bHandled) {
                                       
        return LocalWindow::OnLButtonUp(uMsg, wParam, lParam, bHandled);
    }
    
    LRESULT BrowserWindow::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                       BOOL& bHandled) {
                                       
        return LocalWindow::OnMouseMove(uMsg, wParam, lParam, bHandled);
    }
    
    LRESULT BrowserWindow::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL& bHandled) {
#if CHROME_VERSION_BUILD >= 2704
                                  
        if (m_pBrowser) {
            OnDraggableRegionsChanged(m_pBrowser, m_lastDragRegions);
        }
        
#endif
        
        return LocalWindow::OnSize(uMsg, wParam, lParam, bHandled);
    }
    
    
    
    LRESULT BrowserWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                   BOOL& bHandled) {
        return LocalWindow::OnPaint(uMsg, wParam, lParam, bHandled);
    }
    
    LRESULT BrowserWindow::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                   BOOL& bHandled) {
        if (!m_gifEncoder) {
            return FALSE;
        }
        
        if (wParam == m_gifEncoder->m_gifRecordTimer) {
            writeGif();
            return 0;
        }
        
        bHandled = FALSE;
        return 0;
    }
    
    void BrowserWindow::InitWindow() {
    
    
    
        AMO_TIMER_ELAPSED();
        //获取图标，第二个参数为要获取第几个图标
        HICON hIcon = ExtractIconW(m_PaintManager.GetInstance(),
                                   amo::u8path::fullAppName().generic_wstring().c_str(), 0);
        ::SendMessage(m_hWnd, WM_SETICON, (WPARAM)false, (LPARAM)hIcon);
        // 添加到Transfer管理,如果不是用JS创建的，则没有添加进
        addTransfer(getDerivedClass<ClassTransfer>());
        
        LocalWindow::InitWindow();
        
        // 使用skin图标
        /*    const TImageInfo* data = m_PaintManager.GetImageEx(_T("logo.ico"), NULL, 0);
        	HICON m_hIcon = (HICON)LoadImage(NULL, _T("skin\\logo.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
        	::SendMessage(m_hWnd, WM_SETICON, (WPARAM)false, (LPARAM)m_hIcon);*/
        auto clientApp = amo::AppContext::getInstance()->getClientApp();
        clientApp->RegisterBrowserProcessHandlerDelegate(this);
        m_pTitleBar = static_cast<CControlUI*>(m_PaintManager.FindControl(
                _T("titleLayout")));
        assert(m_pTitleBar != NULL);
        
        
        if (m_pBrowserSettings->titleBar) {
            m_pTitleBar->SetVisible(true);
            RECT rcCaption = { 0, 0, 0, 40 };
            m_PaintManager.SetCaptionRect(rcCaption);
        } else {
            m_pTitleBar->SetVisible(false);
        }
        
        RECT rect;
        ::GetWindowRect(m_hWnd, &rect);
        
        //初始化浏览器控件
        m_pBrowserLayout
            = static_cast<CHorizontalLayoutUI*>(
                  m_PaintManager.FindControl(_T("browserLayout")));
                  
        if (m_pBrowserLayout == NULL) {
            return;
        }
        
        if (m_pWebkit == NULL) {
            m_pWebkit = new WebkitView(m_pBrowserSettings);
        }
        
        auto ss = m_pWebkit->GetName();
        
        m_pWebkit->SetName(_T("Webkit"));
        m_pWebkit->getClientHandler()->RegisterRenderHandlerDelegate(this);
        m_pWebkit->getClientHandler()->RegisterLifeSpanHandlerDelegate(this);
        m_pWebkit->getClientHandler()->RegisterDragHandlerDelegate(this);
        m_pWebkit->getClientHandler()->RegisterDisplayHandlerDelegate(this);
        m_pWebkit->getClientHandler()->RegisterLoadHandlerDelegate(this);
        m_pWebkit->getClientHandler()->RegisterDialogHandlerDelegate(this);
        m_pWebkit->SetBkColor(m_pBrowserSettings->windowColor);
        
        if (!isLayered()) {
            m_pWebkit->SetBkColor(m_pBrowserSettings->windowColor);
            m_PaintManager.GetRoot()->SetAttribute(_T("bkcolor"), _T("0xffffffff"));
            m_pBrowserLayout->SetAttribute(_T("bkcolor"), _T("0xffffffff"));
            m_pWebkit->SetAttribute(_T("bkcolor"), _T("0xffffffff"));
            
        } else {
            m_PaintManager.GetRoot()->SetAttribute(_T("bkcolor"), _T("0x00ffffff"));
            m_pBrowserLayout->SetAttribute(_T("bkcolor"), _T("0x00ffffff"));
            m_pWebkit->SetAttribute(_T("bkcolor"), _T("0x00ffffff"));
        }
        
        
        m_pCefCallbackHandler->registerHandlerDelegate(m_pWebkit->getClientHandler());
        m_pBrowserLayout->Add(m_pWebkit);//将浏览器控件加入到窗口中
        this->registerFunction();
        
        
        AMO_TIMER_ELAPSED();
    }
    
    void BrowserWindow::OnFinalMessage(HWND hWnd) {
        LocalWindow::OnFinalMessage(hWnd);
        
        
        
        //delete m_pWebkit;
        //delete this;
    }
    
    LRESULT BrowserWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                   BOOL& bHandled) {
        if (!m_pBrowserSettings->closable && wParam != 255) {
            bHandled = TRUE;
            return TRUE;
        }
        
        //如果浏览器控件状态，且没有被关闭，那么先关闭浏览器
        if ((m_pWebkit != NULL)
                && m_pWebkit->getBrowser()
                && !m_pWebkit->isClosing()) {
            CefRefPtr<CefBrowserHost> pHost = m_pWebkit->getBrowser()->GetHost();
            pHost->CloseBrowser(wParam == 255 ? true : false);
            bHandled = TRUE;
            return 0;
        }
        
        // 反注册事件代理
        auto clientApp = AppContext::getInstance()->getClientApp();
        clientApp->UnregisterBrowserProcessHandlerDelegate(this);
        //关闭窗口
        
        return LocalWindow::OnClose(uMsg, wParam, lParam, bHandled);
    }
    
    LRESULT BrowserWindow::HandleCustomMessage(UINT uMsg, WPARAM wParam,
            LPARAM lParam, BOOL& bHandled) {
            
        LRESULT lRes = FALSE;
        
        switch (uMsg) {
        
        case WM_NCLBUTTONDOWN:
            lRes = OnNcLButtonDown(uMsg, wParam, lParam, bHandled);
            break;
            
        case WM_NCLBUTTONUP:
            lRes = OnNcLButtonUp(uMsg, wParam, lParam, bHandled);
            break;
            
        case WM_NCLBUTTONDBLCLK:
            lRes = OnNcLButtonDbClick(uMsg, wParam, lParam, bHandled);
            break;
            
        case  WM_TIMER:
            lRes = OnTimer(uMsg, wParam, lParam, bHandled);
            
        default:
            break;
        }
        
        if (bHandled) {
            return lRes;
        }
        
        return LocalWindow::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
    }
    
    
    Any BrowserWindow::enableDrag(IPCMessage::SmartType msg) {
        m_isDragable = true;
        return Undefined();
    }
    
    Any BrowserWindow::disableDrag(IPCMessage::SmartType msg) {
    
        m_isDragable = false;
        return Undefined();
    }
    
    
    Any BrowserWindow::loadURL(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        std::string hasShadow = args->getString(0);
        m_pWebkit->getBrowser()->GetMainFrame()->LoadURL(hasShadow);
        return Undefined();
    }
    
    Any BrowserWindow::reload(IPCMessage::SmartType msg) {
        m_pWebkit->getBrowser()->Reload();
        return Undefined();
    }
    
    Any BrowserWindow::getBrowser(IPCMessage::SmartType msg) {
        if (!m_pBrowserTransfer) {
            return Undefined();
        }
        
        return m_pBrowserTransfer->getFuncMgr().toSimplifiedJson();
    }
    
    
    
    Any BrowserWindow::showDevTools(IPCMessage::SmartType msg) {
        m_pWebkit->showDevTools();
        return Undefined();
    }
    
    Any BrowserWindow::closeDevTools(IPCMessage::SmartType msg) {
        m_pWebkit->closeDevTools();
        return Undefined();
    }
    
    
    std::shared_ptr<BrowserWindow> BrowserWindow::toBrowserWindow() {
        return std::dynamic_pointer_cast<BrowserWindow>(shared_from_this());
    }
    
    Any BrowserWindow::isMainWindow(IPCMessage::SmartType msg) {
        return m_pBrowserSettings->main;
    }
    
    Any BrowserWindow::repaint(IPCMessage::SmartType msg) {
        if (m_pWebkit) {
            m_pWebkit->repaint(msg);
        }
        
        return Undefined();
    }
    
    Any BrowserWindow::addOverlap(IPCMessage::SmartType msg) {
        if (m_pWebkit) {
            return m_pWebkit->addOverlap(msg);
        }
        
        return false;
    }
    
    
    Any BrowserWindow::removeOverlap(IPCMessage::SmartType msg) {
        if (m_pWebkit) {
            return	m_pWebkit->removeOverlap(msg);
        }
        
        return false;
    }
    
    
    Any BrowserWindow::setDragBlackList(IPCMessage::SmartType msg) {
        m_pBrowserSettings->dragBlacklist = msg->getArgumentList()->getInt(0);
        return Undefined();
    }
    
    Any BrowserWindow::getDragBlackList(IPCMessage::SmartType msg) {
        return   m_pBrowserSettings->dragBlacklist;
    }
    
    
    Any BrowserWindow::showTitleBar(IPCMessage::SmartType msg) {
    
        if (m_pTitleBar == NULL) {
            return Undefined();
        }
        
        bool titleBar = msg->getArgumentList()->getBool(0);
        
        if (m_pBrowserSettings->titleBar == titleBar) {
            return Undefined();
            
        }
        
        m_pBrowserSettings->titleBar = titleBar;
        
        
        if (m_pBrowserSettings->titleBar) {
            m_pTitleBar->SetVisible(true);
            RECT rcCaption = { 0, 0, 0, 40 };
            m_PaintManager.SetCaptionRect(rcCaption);
        } else {
            m_pTitleBar->SetVisible(false);
        }
        
        return Undefined();
    }
    
    Any BrowserWindow::setNextDialogFiles(IPCMessage::SmartType msg) {
        auto args = msg->getArgumentList();
        Any& val = args->getValue(0);
        m_dropFiles.clear();
        std::vector<Any> vec;
        
        if (val.is<std::string>()) {
            vec.push_back(val);
        } else if (val.is<std::vector<Any> >()) {
            vec = val.As<std::vector<Any> >();
        }
        
        for (auto& p : vec) {
            if (p.is<std::string>()) {
                amo::u8string str(p.As<std::string>(), true);
                auto appSettings = AppSettings::getInstance();
                str = amo::u8string(appSettings->toAbsolutePath(str), true);
                m_dropFiles.push_back(str.to_wide());
            }
        }
        
        return Undefined();
    }
    
    Any BrowserWindow::getNextDialogFiles(IPCMessage::SmartType msg) {
        std::vector<Any> retval;
        
        for (auto& p : m_dropFiles) {
            retval.push_back(amo::u8string(p.ToWString()).str());
        }
        
        return retval;
    }
    
    Any BrowserWindow::dropFiles(IPCMessage::SmartType msg) {
        auto args = msg->getArgumentList();
        std::vector<Any> files;
        Any& val = args->getValue(0);
        
        if (val.is<std::string>()) {
            files.push_back(val);
        } else if (val.is<std::vector<Any> >()) {
            files = val.As<std::vector<Any> >();
        }
        
        CefRefPtr<CefDragData> drag_data = CefDragData::Create();
        auto appSettings = AppSettings::getInstance();
        
        for (auto& p : files) {
            if (!p.is<std::string>()) {
                continue;
            }
            
            amo::u8string str(p.As<std::string>(), true);
            str = amo::u8string(appSettings->toAbsolutePath(str), true);
            amo::u8path path(str);
            
            if (!path.exists()) {
                continue;
            }
            
            drag_data->AddFile(str.to_wide(), path.filename().generic_wstring());
        }
        
        CefMouseEvent ev;
        ev.x = args->getInt(1);
        ev.y = args->getInt(2);
        ev.modifiers = 272;
        
        CefBrowserHost::DragOperationsMask effect = CefBrowserHost::DragOperationsMask(
                    DRAG_OPERATION_COPY |
                    DRAG_OPERATION_LINK | DRAG_OPERATION_MOVE | DRAG_OPERATION_EVERY);
                    
        auto host = this->GetWebkitView()->getBrowser()->GetHost();
        host->DragTargetDragEnter(drag_data, ev, effect);
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        effect = DRAG_OPERATION_MOVE;
        host->DragTargetDragOver(ev, effect);
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        effect = DRAG_OPERATION_COPY;
        host->DragTargetDrop(ev);
        return Undefined();
    }
    
    Any BrowserWindow::stopRecordGif(IPCMessage::SmartType msg) {
        if (!m_gifEncoder) {
            return Undefined();
        }
        
        m_gifEncoder->End();
        KillTimer(m_hWnd, m_gifEncoder->m_gifRecordTimer);
        m_gifEncoder.reset();
        
        return Undefined();
    }
    
    
    
    void BrowserWindow::writeGif() {
    
    
        if (!m_gifEncoder) {
            return;
        }
        
        
        RECT rc = { 0 };
        ::GetClientRect(m_hWnd, &rc);
        
        createBitmapFromDC([&](HBITMAP hBitmap) {
            std::vector<uint8_t> vec;
            amo::BitmapToBytes(hBitmap, vec);
            
            for (size_t i = 0; i < vec.size();) {
                std::swap(vec[i], vec[i + 2]);
                i += 4;
            }
            
            
            m_gifEncoder->Write(vec.data(), m_gifEncoder->width, m_gifEncoder->height,
                                m_gifEncoder->delay);
                                
        }, true);
        
        
        m_gifEncoder->count++;
        
        if (m_gifEncoder->count >= m_gifEncoder->total) {
            stopRecordGif(IPCMessage::Empty());
        }
        
    }
    
    void BrowserWindow::createBitmapFromDC(std::function<void(HBITMAP)> fn,
                                           bool containsTitleBar /*= false*/) {
                                           
        amo::timer t;
        PAINTSTRUCT ps = { 0 };
        ::BeginPaint(m_hWnd, &ps);
        
        CControlUI* pRoot = m_PaintManager.GetRoot();
        
        if (!containsTitleBar) {
            pRoot = m_PaintManager.FindControl(_T("Webkit"));
        }
        
        if (pRoot == NULL) {
            return;
        }
        
        
        RECT rcClient = pRoot->GetPos();
        int nWidth = rcClient.right - rcClient.left;
        int nHeight = rcClient.bottom - rcClient.top;
        SIZE wndSize = { rcClient.right - rcClient.left, rcClient.bottom - rcClient.top };
        HDC hDC = ::GetDC(m_hWnd);
        
        HDC memDC = NULL;
        
        if (memDC == NULL) {
            memDC = ::CreateCompatibleDC(hDC);
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
        
        $cdevel("createBitmapFromDC used1: {}", t.elapsed());
        t.restart();
        pRoot->DoPaint(memDC, pRoot->GetPos(), NULL);
        
        if (fn) {
            fn(hBitmap);
        }
        
        $cdevel("createBitmapFromDC used2: {}", t.elapsed());
        t.restart();
        ::SelectObject(memDC, hOldBitmap);
        
        DeleteObject(hBitmap);
        
        
        ::ReleaseDC(m_hWnd, memDC);
        
        ::ReleaseDC(m_hWnd, hDC);
        ::EndPaint(m_hWnd, &ps);
        
        $cdevel("createBitmapFromDC used3: {}", t.elapsed());
        t.restart();
    }
    
    
    std::shared_ptr<amo::BrowserWindowSettings>
    BrowserWindow::getBrowserSettings()  const {
        return m_pBrowserSettings;
    }
    
    std::vector<HWND> BrowserWindow::getParents(HWND hWnd) {
        std::vector<HWND> vec;
        
        while (hWnd != NULL) {
            vec.push_back(hWnd);
            hWnd = ::GetParent(hWnd);
        }
        
        return vec;
    }
    
    
    Any BrowserWindow::saveImageToFile(IPCMessage::SmartType msg) {
    
        auto args = msg->getArgumentList();
        
        amo::u8string filename(args->getString(0), true);
        
        bool containsTitleBar = args->getBool(1);
        
        if (filename.empty()) {
            return false;
        }
        
        
        amo::u8path p(filename);
        std::string ext = p.find_extension();
        
        std::string fileformat = GetFormatByExt(ext);
        
        
        
        
        PAINTSTRUCT ps = { 0 };
        ::BeginPaint(m_hWnd, &ps);
        
        CControlUI* pRoot = m_PaintManager.GetRoot();
        
        if (!containsTitleBar) {
            pRoot = m_PaintManager.FindControl(_T("Webkit"));
        }
        
        if (pRoot == NULL) {
            return false;
        }
        
        
        RECT rcClient = pRoot->GetPos();
        int nWidth = rcClient.right - rcClient.left;
        int nHeight = rcClient.bottom - rcClient.top;
        SIZE wndSize = { rcClient.right - rcClient.left, rcClient.bottom - rcClient.top };
        HDC hDC = ::GetDC(m_hWnd);
        
        HDC memDC = NULL;
        
        if (memDC == NULL) {
            memDC = ::CreateCompatibleDC(hDC);
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
        
        HBITMAP hOldBitmap = (HBITMAP)::SelectObject(memDC, hBitmap);
        
        
        pRoot->DoPaint(memDC, pRoot->GetPos(), NULL);
        
        
        SaveHBitmapToFile(hBitmap,
                          filename.to_unicode().c_str(),
                          amo::u8string(fileformat, true).to_unicode().c_str());
                          
                          
        ::SelectObject(memDC, hOldBitmap);
        DeleteObject(hBitmap);
        
        
        ::ReleaseDC(m_hWnd, memDC);
        
        ::ReleaseDC(m_hWnd, hDC);
        ::EndPaint(m_hWnd, &ps);
        return Undefined();
    }
    
    
    
    Any BrowserWindow::recordGifToFile(IPCMessage::SmartType msg) {
        if (m_gifEncoder) {
            return Undefined();
        }
        
        amo::u8json json = msg->getArgumentList()->getJson(0);
        
        m_gifEncoder = GifEncoder::fromJson(json);
        
        
        
        RECT rc = { 0 };
        GetClientRect(m_hWnd, &rc);
        amo::rect rect(rc);
        
        if (m_gifEncoder->width == 0 || m_gifEncoder->height == 0) {
            m_gifEncoder->width = rect.width();
            m_gifEncoder->height = rect.height();
        }
        
        if (!m_gifEncoder->Begin(m_gifEncoder->filename.c_str(), m_gifEncoder->width,
                                 m_gifEncoder->height, m_gifEncoder->delay)) {
            return Undefined();
        }
        
        m_gifEncoder->m_gifRecordTimer =  SetTimer(m_hWnd, WM_GIF_RECOFD_TIMER,
                                          m_gifEncoder->delay,
                                          NULL);
        return Undefined();
        
    }
    
    LRESULT CALLBACK BrowserWindow::SubclassedWindowProc(HWND hWnd, UINT message,
            WPARAM wParam, LPARAM lParam) {
        ::WNDPROC hParentWndProc = reinterpret_cast<::WNDPROC>(::GetPropW(hWnd,
                                   kParentWndProc));
        HRGN hRegion = reinterpret_cast<HRGN>(::GetPropW(hWnd, kDraggableRegion));
        //$cdevel("message:{}, WPARAM:{}, LPARAM{}", message, wParam, lParam);
        
        if (message == WM_NCHITTEST) {
            LRESULT hit = CallWindowProc(hParentWndProc, hWnd, message, wParam, lParam);
            //return hit;
            //return HTTRANSPARENT;
            
            if (hit == HTCLIENT) {
                POINTS points = MAKEPOINTS(lParam);
                POINT point = { points.x, points.y };
                ::ScreenToClient(hWnd, &point);
                
                /* RECT rect;
                GetClientRect(hWnd, &rect);
                
                if (::PtInRect(&rect, point)) {
                return HTTRANSPARENT;
                }
                */
                if (::PtInRegion(hRegion, point.x, point.y)) {
                    // Let the parent window handle WM_NCHITTEST by returning HTTRANSPARENT
                    // in child windows.
                    return HTTRANSPARENT;
                }
            }
            
            
            return hit;
        }
        
        return CallWindowProc(hParentWndProc, hWnd, message, wParam, lParam);
    }
    
    void BrowserWindow::SubclassWindow2(HWND hWnd, HRGN hRegion) {
        HANDLE hParentWndProc = ::GetPropW(hWnd, kParentWndProc);
        
        if (hParentWndProc) {
            return;
        }
        
        SetLastError(0);
        LONG_PTR hOldWndProc = SetWindowLongPtr(
                                   hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(SubclassedWindowProc));
                                   
        if (hOldWndProc == 0 && GetLastError() != ERROR_SUCCESS) {
            return;
        }
        
        ::SetPropW(hWnd, kParentWndProc, reinterpret_cast<HANDLE>(hOldWndProc));
        ::SetPropW(hWnd, kDraggableRegion, reinterpret_cast<HANDLE>(hRegion));
    }
    
    void BrowserWindow::UnSubclassWindow(HWND hWnd) {
        LONG_PTR hParentWndProc =
            reinterpret_cast<LONG_PTR>(::GetPropW(hWnd, kParentWndProc));
            
        if (hParentWndProc) {
            LONG_PTR hPreviousWndProc =
                SetWindowLongPtr(hWnd, GWLP_WNDPROC, hParentWndProc);
            ALLOW_UNUSED_LOCAL(hPreviousWndProc);
            DCHECK_EQ(hPreviousWndProc,
                      reinterpret_cast<LONG_PTR>(SubclassedWindowProc));
        }
        
        ::RemovePropW(hWnd, kParentWndProc);
        ::RemovePropW(hWnd, kDraggableRegion);
    }
    
    BOOL CALLBACK BrowserWindow::SubclassWindowsProc(HWND hwnd, LPARAM lParam) {
        SubclassWindow2(hwnd, reinterpret_cast<HRGN>(lParam));
        return TRUE;
    }
    
    BOOL CALLBACK BrowserWindow::UnSubclassWindowsProc(HWND hwnd, LPARAM lParam) {
        UnSubclassWindow(hwnd);
        return TRUE;
    }
    
    WebkitView* BrowserWindow::GetWebkitView() {
        return m_pWebkit;
    }
    
    DuiLib::CDuiString BrowserWindow::GetSkinFolder() {
        return _T("");
    }
    
    DuiLib::CDuiString BrowserWindow::GetSkinFile() {
    
        return amo::u8string(skinBrowserWindow, true).format(
                   m_pBrowserSettings->settings).to_unicode().c_str();
    }
    
    LPCTSTR BrowserWindow::GetWindowClassName() const {
    
        return _T("BrowserWindow");
        return amo::u8string(m_pBrowserSettings->windowClass,
                             true).to_unicode().c_str();
    }
    
    
    void BrowserWindow::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
    
        // 这个函数可能会被多次调用，因为调试窗口和浏览器窗口共用一个ClientHandler
        if (m_pBrowser) {
            return;
        }
        
        
        
        m_pBrowser = browser;
        
        auto manager = amo::BrowserTransferMgr::getInstance();
        
        manager->setDefaultTriggerEventFunc(
            UIMessageBroadcaster::triggerEventToBorwser);
            
        int nBrowserID = browser->GetIdentifier();
        
        manager->registerClass(nBrowserID);
        auto transferMapMgr = TransferMappingMgr<BrowserTransfer>::getInstance();
        
        if (m_pBrowserTransfer) {
            return;
        }
        
        m_pBrowserTransfer = transferMapMgr->toTransfer(browser);
        // 为什么在没有webkit-app-region的时候不增加一下空白regin用来改变窗口大小？
        // 1. 如果有标题栏可以通过标题栏来改变窗口大小，
        // 2. 如果没有标题栏，那么认为是没有改变窗口大小的需求？？
        // 3.
#if CHROME_VERSION_BUILD >= 2704
        OnDraggableRegionsChanged(browser, {});
#endif
        
        
    }
    void BrowserWindow::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
        if (!browser->IsSame(m_pBrowser)) {
            return ;
        }
        
        m_pBrowser = NULL;
        removeTransfer(this->getObjectID());
        m_pBrowserTransfer.reset();
        auto manager = amo::BrowserTransferMgr::getInstance();
        manager->removeTransfer(browser->GetIdentifier(), transferName());
        auto transferMapMgr = TransferMappingMgr<BrowserTransfer>::getInstance();
        transferMapMgr->removeMapping(browser);
        return ;
    }
    
    bool BrowserWindow::DoClose(CefRefPtr<CefBrowser> browser) {
    
        return false;
        
    }
    
    bool BrowserWindow::OnDragEnter(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefDragData> dragData, CefDragHandler::DragOperationsMask mask) {
        if (m_pBrowserSettings->dragBlacklist != 0) {
            if ((m_pBrowserSettings->dragBlacklist & mask) != 0) {
                return true;
            }
        }
        
        return false;
    }
    
    
    void BrowserWindow::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                      const CefString& title) {
                                      
        amo::u8string str(title.ToString(), true);
        m_pBrowserSettings->title = str.to_utf8();
        SetWindowText(m_hWnd, str.to_unicode().c_str());
    }
    
    void BrowserWindow::OnLoadStart(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame) {
        if (!frame->IsMain()) {
            return;
        }
        
#if CHROME_VERSION_BUILD >= 2704
        // 清除所有拖拽区域，用于修复当前页面跳转到其他页面时，如果该页面未设置，拖拽区域；
        // 那么将不会触发OnDraggableRegionsChanged回调函数，导致原有的拖拽区域仍然可用。
        OnDraggableRegionsChanged(browser, {});
#endif
    }
    
#if CHROME_VERSION_BUILD >= 2704
    void BrowserWindow::OnDraggableRegionsChanged(CefRefPtr<CefBrowser> browser,
            const std::vector<CefDraggableRegion>& regions) {
            
        m_lastDragRegions = regions;
        
        ::SetRectRgn(draggable_region_, 0, 0, 0, 0);
        
        RECT rect;
        GetClientRect(m_hWnd, &rect);
        
        // left
        HRGN leftRgn = ::CreateRectRgn(rect.left, rect.top,
                                       rect.left + 4,
                                       rect.bottom);
        ::CombineRgn(draggable_region_, draggable_region_, leftRgn, RGN_OR);
        
        if (!m_pBrowserSettings->titleBar) {
            HRGN topRgn = ::CreateRectRgn(rect.left, rect.top,
                                          rect.right,
                                          rect.top + 4);
            ::CombineRgn(draggable_region_, draggable_region_, topRgn, RGN_OR);
        }
        
        
        HRGN rightRgn = ::CreateRectRgn(rect.right - 4, rect.top,
                                        rect.right,
                                        rect.bottom);
        ::CombineRgn(draggable_region_, draggable_region_, rightRgn, RGN_OR);
        
        HRGN bottomRgn = ::CreateRectRgn(rect.left, rect.bottom - 4,
                                         rect.right,
                                         rect.bottom);
        ::CombineRgn(draggable_region_, draggable_region_, bottomRgn, RGN_OR);
        
        // Determine new draggable region.
        std::vector<CefDraggableRegion>::const_iterator it = regions.begin();
        
        for (; it != regions.end(); ++it) {
            HRGN region = ::CreateRectRgn(it->bounds.x, it->bounds.y,
                                          it->bounds.x + it->bounds.width,
                                          it->bounds.y + it->bounds.height);
            ::CombineRgn(draggable_region_, draggable_region_, region,
                         it->draggable ? RGN_OR : RGN_DIFF);
            ::DeleteObject(region);
        }
        
        // Subclass child window procedures in order to do hit-testing.
        // This will be a no-op, if it is already subclassed.
        if (m_hWnd) {
            WNDENUMPROC proc =
                !regions.empty() ? SubclassWindowsProc : UnSubclassWindowsProc;
            proc = SubclassWindowsProc;
            ::EnumChildWindows(m_hWnd, proc,
                               reinterpret_cast<LPARAM>(draggable_region_));
        }
    }
#endif
    
    
    
    void BrowserWindow::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame, int httpStatusCode) {
                                  
    }
    
    bool BrowserWindow::OnFileDialog(CefRefPtr<CefBrowser> browser,
                                     CefDialogHandler::FileDialogMode mode,
                                     const CefString& title,
                                     const CefString& default_file_path,
                                     const std::vector<CefString>& accept_filters,
                                     int selected_accept_filter,
                                     CefRefPtr<CefFileDialogCallback> callback) {
                                     
        if (m_dropFiles.empty()) {
            return false;
        }
        
        callback->Continue(selected_accept_filter, m_dropFiles);
        m_dropFiles.clear();
        return true;
    }
    
    bool BrowserWindow::preTranslateMessage(CefEventHandle os_event) {
    
        if (m_pWebkit == NULL || m_pWebkit->getBrowser().get() == NULL) {
            return false;
        }
        
        HWND hWnd = os_event->hwnd;
        std::vector<HWND> parents = getParents(hWnd);
        
        auto iter = std::find(parents.begin(), parents.end(), m_hWnd);
        
        if (iter == parents.end()) {
            return false;
        }
        
        // 如果 == 1, 那么就是 hWnd == m_hWnd
        if (parents.size() > 1) {
            iter = std::find(parents.begin(), parents.end(), m_pWebkit->GetNativeWindow());
            
            if (iter == parents.end()) {
                return false;
            }
        }
        
        POINT point;
        ::GetCursorPos(&point);
        HWND hParent = ::GetAncestor(m_hWnd, GA_ROOT);
        LRESULT  nHitTest = ::SendMessage(hParent,
                                          WM_NCHITTEST,
                                          NULL,
                                          MAKELPARAM(point.x, point.y));
                                          
        if (os_event->message == WM_LBUTTONDOWN) {
            if (nHitTest == HTCAPTION
                    || nHitTest == HTTOP
                    || nHitTest == HTBOTTOM
                    || nHitTest == HTLEFT
                    || nHitTest == HTRIGHT
                    || nHitTest == HTTOPLEFT
                    || nHitTest == HTTOPRIGHT
                    || nHitTest == HTBOTTOMLEFT
                    || nHitTest == HTBOTTOMRIGHT
               ) {
               
                LRESULT lRes = ::SendMessage(hParent,
                                             WM_NCLBUTTONDOWN,
                                             nHitTest,
                                             MAKELPARAM(point.x, point.y));
                ::SendMessage(hParent,
                              WM_NCLBUTTONUP,
                              NULL,
                              MAKELPARAM(point.x, point.y));
                              
                return true;
            }
            
            return false;
        } else if (os_event->message == WM_LBUTTONUP) {
            ::SendMessage(hParent,
                          WM_NCRBUTTONUP,
                          HTCAPTION,
                          MAKELPARAM(point.x, point.y));
        } else if (os_event->message == WM_LBUTTONDBLCLK) {
            if (nHitTest == HTCAPTION
                    || nHitTest == HTTOP
                    || nHitTest == HTBOTTOM
                    || nHitTest == HTLEFT
                    || nHitTest == HTRIGHT
                    || nHitTest == HTTOPLEFT
                    || nHitTest == HTTOPRIGHT
                    || nHitTest == HTBOTTOMLEFT
                    || nHitTest == HTBOTTOMRIGHT
               ) {
               
                ::SendMessage(hParent,
                              WM_NCLBUTTONDBLCLK,
                              nHitTest,
                              MAKELPARAM(point.x, point.y));
                ::SendMessage(hParent,
                              WM_NCLBUTTONUP,
                              NULL,
                              MAKELPARAM(point.x, point.y));
                              
                return true;
            }
            
            return false;
        } else if (os_event->message == WM_MOUSEMOVE) {
        
            switch (nHitTest) {
            case HTTOP:
            case HTBOTTOM:
                ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
                return true;
                
            case HTLEFT:
            case HTRIGHT:
                ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
                return true;
                
            case HTTOPLEFT:
            case HTBOTTOMRIGHT:
                ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
                
                return true;
                
            case HTTOPRIGHT:
            case HTBOTTOMLEFT:
                ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));
                return true;
                
            default:
            
            
                break;
            }
            
            return false;
        } else if (os_event->message == WM_RBUTTONUP) {
            return false;
        }
        
        return false;
    }
    
    LRESULT BrowserWindow::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                       BOOL & bHandled) {
                                       
                                       
        if (!ptInWindow()) {
        
            return HTCLIENT;
        }
        
        
        if (!m_pBrowserSettings->moveable) {
            return HTCLIENT;
        }
        
        if (m_isFullScreen) {
            return HTCLIENT;
        }
        
        POINT pt;
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        ::ScreenToClient(*this, &pt);
        
        RECT rcClient;
        ::GetClientRect(*this, &rcClient);
        
        if (!::IsZoomed(*this)) {
            RECT rcSizeBox = m_PaintManager.GetSizeBox();
            
            if (pt.y < rcClient.top + rcSizeBox.top) {
                if (pt.x < rcClient.left + rcSizeBox.left) {
                    return HTTOPLEFT;
                }
                
                if (pt.x > rcClient.right - rcSizeBox.right) {
                    return HTTOPRIGHT;
                }
                
                return HTTOP;
            } else if (pt.y > rcClient.bottom - rcSizeBox.bottom) {
                if (pt.x < rcClient.left + rcSizeBox.left) {
                    return HTBOTTOMLEFT;
                }
                
                if (pt.x > rcClient.right - rcSizeBox.right) {
                    return HTBOTTOMRIGHT;
                }
                
                return HTBOTTOM;
            }
            
            if (pt.x < rcClient.left + rcSizeBox.left) {
                return HTLEFT;
            }
            
            if (pt.x > rcClient.right - rcSizeBox.right) {
                return HTRIGHT;
            }
        }
        
        CControlUI* pControl = static_cast<CControlUI*>(
                                   m_PaintManager.FindControl(pt));
        CDuiString webkitView = _T("WebkitView");
        
        if (pControl && (pControl->GetName() == _T("browserLayout")
                         || webkitView == pControl->GetClass())) {
            // 2704 以上版本不在使用钩子获取鼠标消息
#if CHROME_VERSION_BUILD >= 2704
            return HTCAPTION;
#else
            
            if (m_isDragable) {
                return HTCAPTION;
            } else {
                return HTCLIENT;
            }
            
#endif
            
            
            
        }
        
        return LocalWindow::OnNcHitTest(uMsg, wParam, lParam, bHandled);
    }
    
    int BrowserWindow::GetIdentifier() const {
        return m_pWebkit->getBrowser()->GetIdentifier();
    }
    
    LRESULT BrowserWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                     BOOL & bHandled) {
                                     
        if (m_pWebkit != NULL && isLayered()) {
            m_pWebkit->insertBitmap(std::shared_ptr<PaintResource>());
        }
        
        
        if (m_pWebkit) {
        
            CefRefPtr<amo::ClientHandler> pHandler = NULL;
            pHandler = m_pWebkit->getClientHandler();
            pHandler->UnregisterRenderHandlerDelegate(this);
            pHandler->UnregisterLifeSpanHandlerDelegate(this);
            pHandler->UnregisterDragHandlerDelegate(this);
            pHandler->UnregisterDisplayHandlerDelegate(this);
            pHandler->UnregisterLoadHandlerDelegate(this);
            /*   if (m_pBrowserLayout) {
                   m_pBrowserLayout->Remove(m_pWebkit);
            
               }
            
               delete m_pWebkit;
               m_pWebkit = NULL;*/
            
        }
        
        return LocalWindow::OnDestroy(uMsg, wParam, lParam, bHandled);
    }
    
    void foo123(Any & any) {
    
    }
    
    
    LRESULT BrowserWindow::OnNcLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                           BOOL & bHandled) {
                                           
        UINT nHitTest = wParam;
        POINT pt;
        POINTSTOPOINT(pt, lParam);
        LRESULT lRes = 0;
        
        if (nHitTest == HTTOP) {
            lRes = SendMessage(WM_SYSCOMMAND,
                               SC_SIZE | WMSZ_TOP, MAKELPARAM(pt.x, pt.y));
        } else if (nHitTest == HTBOTTOM) {
            lRes = SendMessage(WM_SYSCOMMAND,
                               SC_SIZE | WMSZ_BOTTOM, MAKELPARAM(pt.x, pt.y));
        } else if (nHitTest == HTLEFT) {
            lRes = SendMessage(WM_SYSCOMMAND,
                               SC_SIZE | WMSZ_LEFT, MAKELPARAM(pt.x, pt.y));
        } else if (nHitTest == HTRIGHT) {
            lRes = SendMessage(WM_SYSCOMMAND,
                               SC_SIZE | WMSZ_RIGHT, MAKELPARAM(pt.x, pt.y));
        } else if (nHitTest == HTTOPLEFT) {
            lRes = SendMessage(WM_SYSCOMMAND,
                               SC_SIZE | WMSZ_TOPLEFT, MAKELPARAM(pt.x, pt.y));
        } else if (nHitTest == HTTOPRIGHT) {
            lRes = SendMessage(WM_SYSCOMMAND,
                               SC_SIZE | WMSZ_TOPRIGHT, MAKELPARAM(pt.x, pt.y));
        } else if (nHitTest == HTBOTTOMLEFT) {
            lRes = SendMessage(WM_SYSCOMMAND,
                               SC_SIZE | WMSZ_BOTTOMLEFT, MAKELPARAM(pt.x, pt.y));
        } else if (nHitTest == HTBOTTOMRIGHT) {
            lRes = SendMessage(WM_SYSCOMMAND,
                               SC_SIZE | WMSZ_BOTTOMRIGHT, MAKELPARAM(pt.x, pt.y));
        } else if (nHitTest == HTCAPTION) {
            lRes = SendMessage(WM_SYSCOMMAND,
                               SC_MOVE | 4, MAKELPARAM(pt.x, pt.y));
        }
        
        bHandled = true;
        return lRes;
    }
    
    LRESULT BrowserWindow::OnNcLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                         BOOL & bHandled) {
                                         
        LRESULT lRes = 0;
        
        bHandled = true;
        return lRes;
    }
    
    LRESULT BrowserWindow::OnNcLButtonDbClick(UINT uMsg, WPARAM wParam,
            LPARAM lParam, BOOL & bHandled) {
        if (!m_pBrowserSettings->resizable) {
            bHandled = TRUE;
            return TRUE;
        }
        
        return FALSE;
    }
    
    
}
