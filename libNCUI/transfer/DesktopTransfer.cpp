#include "stdafx.h"
#include "transfer/DesktopTransfer.h"
#include "ui/win/Bitmap.hpp"
#include "ipc/BlobManager.hpp"

namespace amo {

    DesktopTransfer::DesktopTransfer()
        : ClassTransfer("desktop") {
        
    }
    
    Any DesktopTransfer::getScreenInfo(IPCMessage::SmartType msg) {
        int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);//屏幕横向分辨率
        int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);//屏幕纵向分辨率
        amo::u8json json;
        json.put("width", nScreenWidth);
        json.put("height", nScreenHeight);
        
        MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        
        ::GetMonitorInfo(::MonitorFromWindow(::GetDesktopWindow(),
                                             MONITOR_DEFAULTTOPRIMARY),
                         &oMonitor);
                         
                         
        amo::u8json jsonWork;
        jsonWork.put("left", oMonitor.rcMonitor.left);
        jsonWork.put("top", oMonitor.rcWork.top);
        jsonWork.put("right", oMonitor.rcWork.right);
        jsonWork.put("bottom", oMonitor.rcWork.bottom);
        jsonWork.put("width", oMonitor.rcWork.right - oMonitor.rcWork.left);
        jsonWork.put("height", oMonitor.rcWork.bottom - oMonitor.rcWork.top);
        json.put_child("rcWork", jsonWork);
        return json;
    }
    
    
    
    Any DesktopTransfer::screenshot(IPCMessage::SmartType msg) {
        auto args = msg->getArgumentList();
        Any& val = args->getValue(0);
        bool copyToClipboard = false;
        
        int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);//屏幕横向分辨率
        int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);//屏幕纵向分辨率
        amo::rect rect = { 0, 0, nScreenWidth, nScreenHeight };
        
        if (val.is<bool>()) {
            copyToClipboard = args->getBool(0);
        } else if (val.is<std::vector<Any> >()) {
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
            
            copyToClipboard = args->getBool(1);
        }
        
        RECT rc = rect;
        HBITMAP hBitmap = amo::ScreenCapture(&rc);
        
        if (hBitmap == NULL) {
            return "";
        }
        
        copyToClipboard = false;
        
        if (copyToClipboard) {
            Clipboard clipboard;
            clipboard.writeBitmap(hBitmap);
            
        }
        
        std::wstring imageFormat = amo::GetFormatByExt(L".jpg");
        
        std::vector<uint8_t> vec;
        
        bool bOk = amo::SaveHBitmapToStream(hBitmap, vec, imageFormat.c_str());
        
        if (!bOk) {
            return "";
        }
        
        std::string encoded = "data:image/jpeg;base64,";
        encoded += amo::base64::encode(vec.data(), vec.size());
        DeleteObject(hBitmap);
        //return encoded;
        
        auto blob = BigStrManager<PID_BROWSER>::getInstance()->create(encoded.size(),
                    10000);
        blob->mime("image/jpeg");
        blob->write(encoded);
        return *blob;
        
        
        
    }
    
}

