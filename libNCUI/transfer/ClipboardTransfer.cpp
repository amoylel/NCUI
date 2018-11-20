#include "stdafx.h"
#include "transfer/ClipboardTransfer.h"
#include <amo/filestream.hpp>
#include "ui/win/Bitmap.hpp"
#include "ipc/BlobManager.hpp"
#include "ui/win/clipboard/ClipboardMonitor.h"


namespace amo {

    ClipboardTransfer::ClipboardTransfer() : ClassTransfer("clipboard") {
        addModule("EventEmitter");
    }
    
    
    Any ClipboardTransfer::startWatch(IPCMessage::SmartType msg) {
        return ClipboardMonitor::getInstance()->startWatch();
    }
    
    Any ClipboardTransfer::stopWatch(IPCMessage::SmartType msg) {
        return ClipboardMonitor::getInstance()->stopWatch();
    }
    
    Any ClipboardTransfer::isWatching(IPCMessage::SmartType msg) {
        return ClipboardMonitor::getInstance()->isWatching();
    }
    
    Any ClipboardTransfer::readText(IPCMessage::SmartType msg) {
        try {
            int nBrowserID = msg->getArgumentList()->getInt(IPCArgsPosInfo::BrowserID);
            auto manager = BrowserWindowManager::getInstance();
            HWND hWnd = manager->getHWNDByBrowserID(nBrowserID);
            
            Clipboard clipboard;
            
            std::wstring wstr = clipboard.readUnicodeText();
            
            if (wstr.empty()) {
                return"";
            }
            
            return amo::u8string(wstr).to_utf8();
        } catch (const std::exception& e) {
            $cerr("read clipboard data failed: {}", e.what());
            return "";
        }
        
        
        
    }
    
    
    Any ClipboardTransfer::readImage(IPCMessage::SmartType msg) {
        FUNC_CALL_TIME();
        
        try {
        
            Clipboard clipboard;
            amo::u8string fmt(msg->getArgumentList()->getString(0), true);
            
            if (fmt.empty()) {
                fmt = L".jpeg";
            }
            
            std::string data =  clipboard.readImageBase64(fmt);
            auto blob =  BigStrManager<PID_BROWSER>::getInstance()->create(data.size(),
                         10000);
            blob->mime("image/jpeg");
            blob->write(data);
            return *blob;
        } catch (const std::exception& e) {
            $cerr("write clipboard data failed: {}", e.what());
            return false;
        }
    }
    
    Any ClipboardTransfer::readFiles(IPCMessage::SmartType msg) {
        std::vector<Any> retval;
        
        try {
            Clipboard clipboard;
            bool bContainsDir = msg->getArgumentList()->getBool(0);
            std::vector<std::wstring> vec = clipboard.readFiles();
            
            for (auto& p : vec) {
                if (!bContainsDir && amo::u8path(p).is_directory()) {
                    continue;
                }
                
                retval.push_back(amo::u8string(p).to_utf8());
            }
            
            return retval;
        } catch (const std::exception& e) {
            $cerr("read clipboard data failed: {}", e.what());
            return retval;
        }
    }
    
    
    Any ClipboardTransfer::saveText(IPCMessage::SmartType msg) {
    
        try {
            Clipboard clipboard;
            std::wstring str = clipboard.readUnicodeText();
            amo::u8string u8str(str);
            amo::u8string savePath(msg->getArgumentList()->getString(0), true);
            
            if (savePath.empty()) {
                return false;
            }
            
            if (str.empty()) {
                return false;
            }
            
            savePath = amo::u8string(amo::AppSettings::getInstance()->toAbsolutePath(
                                         savePath), true);
                                         
            amo::u8path path(savePath);
            
            if (path.is_directory()) {
                return false;
            }
            
            path.parent().create_directory();
            
            amo::u8filestream ofs(path, std::ios::out | std::ios::binary);
            
            if (!ofs.is_open()) {
                return false;
            }
            
            ofs.write(u8str.c_str(), u8str.size());
            ofs.close();
            
            
            return true;
        } catch (const std::exception& e) {
            $cerr("write clipboard data failed: {}", e.what());
            return false;
        }
        
        return false;
    }
    
    Any ClipboardTransfer::saveImage(IPCMessage::SmartType msg) {
        try {
        
            Clipboard clipboard;
            
            if (!clipboard.hasFormat(CF_BITMAP)) {
                return false;
            }
            
            amo::u8string savePath(msg->getArgumentList()->getString(0), true);
            
            if (savePath.empty()) {
                return false;
            }
            
            if (savePath.empty()) {
                return false;
            }
            
            savePath = amo::u8string(amo::AppSettings::getInstance()->toAbsolutePath(
                                         savePath), true);
                                         
            amo::u8path path(savePath);
            
            if (path.is_directory()) {
                return false;
            }
            
            path.parent().create_directory();
            
            std::wstring wpath =  path.normalize().canonicalize(true).raw_wstring();
            
            return clipboard.saveImage(wpath);
            
        } catch (const std::exception& e) {
            $cerr("write clipboard data failed: {}", e.what());
            return false;
        }
        
        return false;
    }
    
    Any ClipboardTransfer::saveFiles(IPCMessage::SmartType msg) {
        try {
        
            Clipboard clipboard;
            std::vector<std::wstring> files = clipboard.readFiles();
            
            if (files.empty()) {
                return false;
            }
            
            Any& val = msg->getArgumentList()->getValue(0);
            
            std::vector<std::pair<std::wstring, std::wstring> >vec;
            
            auto appSettings = amo::AppSettings::getInstance();
            
            if (val.is<std::string>()) {
                amo::u8string path(val.As<std::string>(), true);
                path = amo::u8string(appSettings->toAbsolutePath(path), true);
                
                if (path.empty()) {
                    return false;
                }
                
                vec.push_back({ files[0], path.to_wide() });
            } else if (val.is<std::vector<Any> >()) {
                std::vector<Any> paths = val.As<std::vector<Any> >();
                
                for (auto& p : paths) {
                    if (!p.is<amo::u8json>()) {
                        return false;
                    }
                    
                    amo::u8json json = p.As<amo::u8json>();
                    
                    if (!json.is_object()) {
                        return false;
                    }
                    
                    std::wstring src = amo::u8string(json.getString("src"), true).to_wide();
                    
                    bool found = false;
                    
                    for (auto& p : files) {
                        if (amo::path(p).equal_to(amo::path(src))) {
                            found = true;
                            src = p;
                            break;
                        }
                    }
                    
                    if (!found) {
                        return false;
                    }
                    
                    amo::u8string dst(json.getString("dst"), true);
                    dst = amo::u8string(appSettings->toAbsolutePath(dst), true);
                    
                    if (dst.empty()) {
                        return false;
                    }
                    
                    vec.push_back({ src, dst.to_wide() });
                }
                
            }
            
            if (vec.empty()) {
                return false;
            }
            
            bool bOk = true;
            
            for (auto& p : vec) {
                amo::u8path path(p.first);
                
                if (!path.copy_to(p.second)) {
                    bOk = false;
                }
            }
            
            return bOk;
            
            
        } catch (const std::exception& e) {
            $cerr("write clipboard data failed: {}", e.what());
            return false;
        }
        
        
        return false;
    }
    
    Any ClipboardTransfer::hasFormat(IPCMessage::SmartType msg) {
        try {
            Clipboard clipboard;
            uint32_t nFmt = msg->getArgumentList()->getInt(0);
            return clipboard.hasFormat(nFmt);
        } catch (const std::exception& e) {
            $cerr("read clipboard data failed: {}", e.what());
            return false;
        }
    }
    
    Any ClipboardTransfer::empty(IPCMessage::SmartType msg) {
        try {
            Clipboard clipboard;
            return clipboard.empty();
        } catch (const std::exception& e) {
            $cerr("read clipboard data failed: {}", e.what());
            return false;
        }
    }
    
    Any ClipboardTransfer::isEmpty(IPCMessage::SmartType msg) {
        try {
            Clipboard clipboard;
            return clipboard.isEmpty();
        } catch (const std::exception& e) {
            $cerr("read clipboard data failed: {}", e.what());
            return false;
        }
    }
    
    Any ClipboardTransfer::getFormats(IPCMessage::SmartType msg) {
        std::vector<Any> retval;
        
        try {
            Clipboard clipboard;
            auto vec =  clipboard.getFormats();
            
            for (auto& p : vec) {
                retval.push_back((int32_t)p);
            }
            
            return retval;
        } catch (const std::exception& e) {
            $cerr("read clipboard data failed: {}", e.what());
            return retval;
        }
    }
    
    Any ClipboardTransfer::effect(IPCMessage::SmartType msg) {
    
        try {
            Clipboard clipboard;
            return clipboard.dropEffect();
        } catch (const std::exception& e) {
            $cerr("read clipboard data failed: {}", e.what());
            return 0;
        }
    }
    
    Any ClipboardTransfer::screenshot(IPCMessage::SmartType msg) {
        auto args = msg->getArgumentList();
        Any& val = args->getValue(0);
        
        int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);//屏幕横向分辨率
        int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);//屏幕纵向分辨率
        amo::rect rect = { 0, 0, nScreenWidth, nScreenHeight };
        
        if (val.is<std::vector<Any> >()) {
            std::vector<Any> vec = val;
            
            if (vec.size() != 4) {
                return "";
            }
            
            for (auto& p : vec) {
                if (!p.is<int>()) {
                    return "";
                }
            }
            
            rect.left(vec[0].As<int>());
            rect.top(vec[1].As<int>());
            rect.width(vec[2].As<int>());
            rect.height(vec[3].As<int>());
            
        }
        
        RECT rc = rect;
        HBITMAP hBitmap = amo::ScreenCapture(&rc);
        
        if (hBitmap == NULL) {
            return false;
        }
        
        Clipboard clipboard;
        return	clipboard.writeBitmap(hBitmap);
        
    }
    
    Any ClipboardTransfer::test(IPCMessage::SmartType msg) {
    
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
        
        
        auto manager = BrowserWindowManager::getInstance();
        int nBrowserID = msg->getArgumentList()->getInt(IPCArgsPosInfo::BrowserID);
        auto pLocalWindow = manager->findValidWindow(nBrowserID);
        std::shared_ptr<BrowserWindow> pWindow;
        
        if (pLocalWindow) {
            pWindow = pLocalWindow->toBrowserWindow();
        }
        
        if (!pWindow) {
            return Undefined();
        }
        
        
        CefMouseEvent ev;
        ev.x = 100;
        ev.y = 100;
        ev.modifiers = 272;
        CefBrowserHost::DragOperationsMask effect = CefBrowserHost::DragOperationsMask(
                    DRAG_OPERATION_COPY |
                    DRAG_OPERATION_LINK | DRAG_OPERATION_MOVE | DRAG_OPERATION_EVERY);
        auto host = pWindow->GetWebkitView()->getBrowser()->GetHost();
        host->DragTargetDragEnter(drag_data, ev, effect);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        effect = DRAG_OPERATION_MOVE;
        host->DragTargetDragOver(ev, effect);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        effect = DRAG_OPERATION_COPY;
        host->DragTargetDrop(ev);
        return Undefined();
    }
    
    
    Any ClipboardTransfer::writeText(IPCMessage::SmartType msg) {
        try {
        
            // 必须同步执行，不然readText时会出错，也不知道为啥
            int nBrowserID = msg->getArgumentList()->getInt(IPCArgsPosInfo::BrowserID);
            // 如果所给FrameID小于0，查找当前所获得焦点的窗口
            auto manager = BrowserWindowManager::getInstance();
            HWND hWnd = manager->getHWNDByBrowserID(nBrowserID);
            
            Clipboard clipboard;
            std::string str = msg->getArgumentList()->getString(0);
            
            if (str.empty()) {
                return false;
            }
            
            amo::u8string s(str, true);
            bool bOk = clipboard.writeText(s.to_wide());
            return bOk;
        } catch (const std::exception& e)  {
            $cerr("write clipboard data failed: {}", e.what());
            return false;
        }
        
    }
    
    Any ClipboardTransfer::writeImage(IPCMessage::SmartType msg) {
        try {
            Any& val = msg->getArgumentList()->getValue(0);
            std::string data;
            
            if (val.is<std::string>()) {
                data = val.As<std::string>();
            }
            
            // else if (val.is<amo::blob>()) {
            //    amo::blob blob = val.As<amo::blob>();
            //
            //    while (true) {
            //        blob.update_header();
            //        int state = blob.state();
            //
            //        if (amo::timer::now() < blob.create_timestamp() + blob.delay_destroy()) {
            //            // 当前数据已超过其存活时间，丢弃不要
            //            return false;
            //        }
            //
            //        if (state == amo::blob::BLOB_DELETE) {
            //            return false;
            //        }
            //
            //        else if (state == amo::blob::BLOB_READY) {
            //            data = blob.read();
            //            break;
            //        } else {
            //            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            //        }
            //    }
            //}
            //
            
            if (data.empty()) {
                return false;
            }
            
            /*     amo::u8filestream ifs(L"E:\\test.txt", std::ios::in | std::ios::binary);
                 data = ifs.read_all();
                 ifs.close();*/
            Clipboard clipboard;
            bool bOk = 	clipboard.writeImageBase64(data);
            return bOk;
        } catch (const std::exception& e) {
            $cerr("write clipboard data failed: {}", e.what());
            return false;
        }
    }
    
    Any ClipboardTransfer::writeImageFromFile(IPCMessage::SmartType msg) {
        try {
        
        
            Any& val = msg->getArgumentList()->getValue(0);
            
            if (!val.is<std::string>()) {
                return false;
            }
            
            amo::u8string filename(val.As<std::string>(), true);
            auto appSettings = amo::AppSettings::getInstance();
            filename = amo::u8string(appSettings->toAbsolutePath(filename), true);
            
            if (!amo::u8path(filename).exists()) {
                return false;
            }
            
            Clipboard clipboard;
            return clipboard.writeImageFromFile(filename.to_wide());
        } catch (const std::exception& e) {
            $cerr("read clipboard data failed: {}", e.what());
            return false;
        }
    }
    
    Any ClipboardTransfer::writeFiles(IPCMessage::SmartType msg) {
    
        try {
        
            std::vector<std::wstring> vec;
            std::vector<Any> files;
            Any& val = msg->getArgumentList()->getValue(0);
            
            if (val.is<std::string>()) {
                files.push_back(val);
                
            } else if (val.is<std::vector<Any> >()) {
                files = val.As<std::vector<Any> >();
            }
            
            for (auto& p : files) {
                if (!p.is<std::string>()) {
                    continue;
                }
                
                std::wstring path = amo::u8string(p.As<std::string>(), true).to_wide();
                std::string u8str = amo::u8path(path)
                                    .absolute()
                                    .normalize()
                                    .canonicalize(true).raw_string();
                                    
                vec.push_back(amo::u8string(u8str, true).to_wide());
            }
            
            
            Clipboard clipboard;
            return clipboard.writeFiles(vec);
        } catch (const std::exception& e) {
            $cerr("read clipboard data failed: {}", e.what());
            return false;
        }
    }
    
}

