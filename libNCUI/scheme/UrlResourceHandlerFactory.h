// Created by amoylel on 12/11/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_URLRESOURCEHANDLERFACTORY_H__
#define AMO_URLRESOURCEHANDLERFACTORY_H__


#include <amo/singleton.hpp>
#include "handler/CefHeader.hpp"

namespace amo {
    class UrlResourceHandlerFactory : public
        amo::singleton<UrlResourceHandlerFactory> {
    public:
        UrlResourceHandlerFactory();
        
        CefRefPtr<CefResourceHandler> create(const std::string& url);
        
        std::string redirectUrl(const std::string& url);
        
        CefRefPtr<CefResourceHandler> getDBResourceHandler(const std::string& url,
                const std::string& u8Path);
                
        CefRefPtr<CefResourceHandler> getZipResourceHandler(const std::string& url,
                const std::string& u8Path);
                
        CefRefPtr<CefResourceHandler> getZResResourceHandler(const std::string& url,
                const std::string& u8Path);
                
        CefRefPtr<CefResourceHandler> getDllResourceHandler(const std::string& url,
                const std::string& u8Path);
        std::string getAbsolutePath(const std::string& u8Path);
        
        bool isZipPath(const std::string& u8Path);
        
        bool isResPath(const std::string& u8Path);
        
        bool isDBPath(const std::string& u8Path);
        
        bool isDllPath(const std::string& u8Path);
    private:
    };
}


#endif // AMO_SCHEMEMANAGER_H__

