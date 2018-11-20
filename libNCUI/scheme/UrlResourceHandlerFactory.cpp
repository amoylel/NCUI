#include "stdafx.h"
#include "scheme/UrlResourceHandlerFactory.h"
#include "scheme/ZipFileHandler.h"
#include "scheme/DllFileHandler.h"


namespace amo {

    UrlResourceHandlerFactory::UrlResourceHandlerFactory() {
    
    }
    
    CefRefPtr<CefResourceHandler> UrlResourceHandlerFactory::create(
        const std::string& url) {
        IPCMessage::SmartType msg(new IPCMessage());
        msg->getArgumentList()->setValue(0, url);
        
        std::shared_ptr<AppTransfer> pTransfer;
        pTransfer = ClassTransfer::getUniqueTransfer<AppTransfer>();
        Any ret = pTransfer->urlToNativePath(msg);
        
        std::string u8File = ret.As<std::string>();
        
        if (u8File.empty()) {
            return NULL;
        }
        
        CefRefPtr<CefResourceHandler> pHandler = NULL;
        
        pHandler = getZipResourceHandler(url, u8File);
        
        if (pHandler) {
            return pHandler;
        }
        
        pHandler = getZResResourceHandler(url, u8File);
        
        if (pHandler) {
            return pHandler;
        }
        
        pHandler = getDBResourceHandler(url, u8File);
        
        if (pHandler) {
            return pHandler;
        }
        
        pHandler = getDllResourceHandler(url, u8File);
        
        if (pHandler) {
            return pHandler;
        }
        
        amo::u8string ansiPath(u8File, true);
        
        if (amo::u8path(ansiPath).file_exists()) {
            return new NativeFileHandler(url, u8File);
        }
        
        if (url != u8File) {
        
        }
        
        return NULL;
    }
    
    
    std::string UrlResourceHandlerFactory::redirectUrl(const std::string& url) {
        IPCMessage::SmartType msg(new IPCMessage());
        msg->getArgumentList()->setValue(0, url);
        
        std::shared_ptr<AppTransfer> pTransfer;
        pTransfer = ClassTransfer::getUniqueTransfer<AppTransfer>();
        Any ret = pTransfer->urlToNativePath(msg);
        
        std::string u8File = ret.As<std::string>();
        
        if (u8File.empty()) {
            return "";
        }
        
        if (isZipPath(u8File)) {
            return "";
        } else if (isResPath(u8File)) {
            return "";
        } else if (isDBPath(u8File)) {
            return "";
        } else if (isDllPath(u8File)) {
            return "";
        } else if (url != u8File) {
        
            amo::u8string ansiPath(u8File, true);
            
            if (amo::u8path(ansiPath).file_exists()) {
                return "";
            } else  {
                return u8File;
            }
        }
        
        return "";
    }
    
    CefRefPtr<CefResourceHandler> UrlResourceHandlerFactory::getDBResourceHandler(
        const std::string& url, const std::string& u8Path) {
        
        return NULL;
    }
    
    CefRefPtr<CefResourceHandler> UrlResourceHandlerFactory::getZipResourceHandler(
        const std::string& url, const std::string& u8Path) {
        if (!isZipPath(u8Path)) {
            return NULL;
        }
        
        
        std::string dbFile = u8Path.substr(7);
        
        int  nIndex = dbFile.find(".zip");
        
        if (nIndex == -1) {
            return NULL;
        }
        
        std::string u8DBPath = dbFile.substr(0, nIndex + 4);
        std::string u8File = dbFile.substr(nIndex + 4);
        
        return new ZipFileHandler(url, u8DBPath, u8File);
        
        
    }
    
    CefRefPtr<CefResourceHandler> UrlResourceHandlerFactory::getZResResourceHandler(
        const std::string& url, const std::string& u8Path) {
        if (!isResPath(u8Path)) {
            return NULL;
        }
        
        
        std::string dbFile = u8Path.substr(7);
        
        int  nIndex = dbFile.find(".res");
        
        if (nIndex == -1) {
            return NULL;
        }
        
        std::string u8DBPath = dbFile.substr(0, nIndex + 4);
        std::string u8File = dbFile.substr(nIndex + 4);
        
        return new ZipFileHandler(url, u8DBPath, u8File);
        
        
    }
    
    CefRefPtr<CefResourceHandler> UrlResourceHandlerFactory::getDllResourceHandler(
        const std::string& url, const std::string& u8Path) {
        
        if (!isDllPath(u8Path)) {
            return NULL;
        }
        
        
        std::string dbFile = u8Path.substr(7);
        
        int  nIndex = dbFile.find(".dll");
        
        if (nIndex == -1) {
            return NULL;
        }
        
        std::string u8DBPath = dbFile.substr(0, nIndex + 4);
        std::string u8File = dbFile.substr(nIndex + 4);
        
        return new DllFileHandler(url, u8DBPath, u8File);
        
    }
    
    std::string UrlResourceHandlerFactory::getAbsolutePath(const std::string&
            u8Path) {
            
        auto appSettings = AppContext::getInstance()->getDefaultAppSettings();
        
        if (isZipPath(u8Path)) {
            std::string retval = "zip:///";
            std::string u8SubPath = u8Path.substr(retval.size());
            return retval + appSettings->toAbsolutePath(u8SubPath);
        } else if (isResPath(u8Path)) {
            std::string retval = "res:///";
            std::string u8SubPath = u8Path.substr(retval.size());
            return retval + appSettings->toAbsolutePath(u8SubPath);
        } else if (isDBPath(u8Path)) {
            std::string retval = "db:///";
            std::string u8SubPath = u8Path.substr(retval.size());
            return retval + appSettings->toAbsolutePath(u8SubPath);
        } else if (isDllPath(u8Path)) {
            std::string retval = "dll:///";
            std::string u8SubPath = u8Path.substr(retval.size());
            return retval + appSettings->toAbsolutePath(u8SubPath);
        }
        
        return appSettings->toAbsolutePath(u8Path);
    }
    
    bool UrlResourceHandlerFactory::isZipPath(const std::string& u8Path) {
        int nIndex = u8Path.find("zip:///");
        
        if (nIndex == 0) {
            return true;
        }
        
        nIndex = u8Path.find("zip:\\\\\\");
        return nIndex == 0;
    }
    
    bool UrlResourceHandlerFactory::isResPath(const std::string& u8Path) {
        int nIndex = u8Path.find("res:///");
        
        if (nIndex == 0) {
            return true;
        }
        
        nIndex = u8Path.find("res:\\\\\\");
        return nIndex == 0;
    }
    
    bool UrlResourceHandlerFactory::isDBPath(const std::string& u8Path) {
        int nIndex = u8Path.find("db:///");
        
        if (nIndex == 0) {
            return true;
        }
        
        nIndex = u8Path.find("db:\\\\\\");
        return nIndex == 0;
    }
    
    bool UrlResourceHandlerFactory::isDllPath(const std::string& u8Path) {
        int nIndex = u8Path.find("dll:///");
        
        if (nIndex == 0) {
            return true;
        }
        
        nIndex = u8Path.find("dll:\\\\\\");
        return nIndex == 0;
    }
    
}

