#include "stdafx.h"
#include "ui/win/ActionPreprocessor.h"

#include <memory>

#include "handler/CefHeader.hpp"
#include "ui/win/BrowserWindowManager.h"
#include "transfer/AppTransfer.h"
#include "transfer/ClassTransfer.hpp"
#include "ui/win/BrowserWindow.h"

namespace amo {

    ActionPreprocessor::ActionPreprocessor(int32_t nBrowserID, int64_t nFrameID)
        : m_nBrowserID(nBrowserID)
        , m_nFrameID(nFrameID) {
        
        
    }
    
    bool ActionPreprocessor::process(const std::string& action) {
        if (action.empty()) {
            return false;
        }
        
        // 如果所给FrameID小于0，查找当前所获得焦点的窗口
        auto manager = BrowserWindowManager::getInstance();
        
        
        std::shared_ptr<BrowserWindow> pWindow;
        
        // 先通过ID查找
        auto pLocalWindow = manager->findValidWindow(m_nBrowserID);
        
        // 不存在，使用当前焦点窗口
        if (!pLocalWindow) {
            return false;
        }
        
        pWindow = pLocalWindow->toBrowserWindow();
        
        if (!pWindow) {
            return false;
        }
        
        CefRefPtr<CefBrowser> pBrowser = pWindow->GetWebkitView()->getBrowser();
        
        if (!pBrowser) {
            return false;
        }
        
        CefRefPtr<CefFrame> pFrame = pBrowser->GetFrame(m_nFrameID);
        
        if (!pFrame) {
            pFrame = pBrowser->GetMainFrame();
        }
        
        CefRefPtr<CefBrowserHost> pHost = pBrowser->GetHost();
        
        IPCMessage::SmartType msg(new IPCMessage());
        
        // 对已有Action的直接执行操作
        
        // Frame 操作
        if (action == "undo") {
            pFrame->Undo();
        } else if (action == "redo") {
            pFrame->Redo();
        } else if (action == "cut") {
            pFrame->Cut();
        } else if (action == "copy") {
            pFrame->Copy();
        } else if (action == "paste") {
            pFrame->Paste();
        } else if (action == "selectAll") {
            pFrame->SelectAll();
        } else if (action == "delete") {
            pFrame->Delete();
        }
        
        // 浏览器操作
        else if (action == "goBack") {
            // 后退
            pBrowser->GoBack();
        } else if (action == "goForward") {
            // 前进
            pBrowser->GoForward();
        } else if (action == "reload") {
            // reload - 正常重新加载当前窗口
            pBrowser->Reload();
        } else if (action == "forceReload") {
            // 忽略缓存并重新加载当前窗口
            pBrowser->ReloadIgnoreCache();
        } else if (action == "stopLoad") {
            // 停止加载
            pBrowser->StopLoad();
        }
        // HOST 操作
        else if (action == "print") {
            //TODO: 打印有BUG
            pHost->Print();
        } else if (action == "printToPDF") {
            //pHost->PrintToPDF()
        } else if (action == "resetZoom") {
            pHost->SetZoomLevel(0.0);
        } else if (action == "zoomIn") {
            pHost->SetZoomLevel(pHost->GetZoomLevel() - 0.1);
        } else if (action == "zoomOut") {
            pHost->SetZoomLevel(pHost->GetZoomLevel() + 0.1);
        }
        
        // 窗口操作
        else if (action == "show") {
            pWindow->show(msg);
        } else if (action == "hide") {
            pWindow->hide(msg);
        } else if (action == "toggleVisible") {
            pWindow->toggleVisible(msg);
        } else if (action == "minimize") {
            pWindow->minimize(msg);
        } else if (action == "maximize") {
            pWindow->maximize(msg);
        } else if (action == "close") {
            pWindow->close(msg);
        }  else if (action == "showDevtools") {
            pWindow->showDevTools(msg);
        } else if (action == "closeDevtools") {
            pWindow->closeDevTools(msg);
        } else if (action == "fullscreen") {
            pWindow->setFullscreen(true);
        } else if (action == "nonFullscreen") {
            pWindow->setFullscreen(false);
        } else if (action == "toggleFullScreen") {
            pWindow->toggleFullScreen(msg);
        }
        
        // App
        else if (action == "quit") {
            std::shared_ptr<AppTransfer> pTransfer;
            pTransfer = ClassTransfer::getUniqueTransfer<AppTransfer>();
            pTransfer->quit(msg);
        } else if (action == "exit") {
            std::shared_ptr<AppTransfer> pTransfer;
            pTransfer = ClassTransfer::getUniqueTransfer<AppTransfer>();
            pTransfer->exit(msg);
        } else  {
            return false;
        }
        
        return true;
    }
    
}


