// Created by amoylel on 07/04/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_TRANSFERMAPPING_HPP__
#define AMO_TRANSFERMAPPING_HPP__

#include <memory>
#include "handler/CefHeader.hpp"
/*!
 * @namespace	amo
 *
 * @brief	Transfer 与实际的Cef类型映射.
 */
namespace amo {
    class BrowserTransfer;
    class FrameTransfer;
    class BrowserHostTransfer;
    class RequestTransfer;
    class URLRequestTransfer;
    class URLRequestClientTransfer;
    /*!
     * @struct	TransferMapping
     *
     * @brief	通用类型，所映射的类型与自身相同.
     *
     * @tparam	T	Generic type parameter.
     */
    template<typename T> struct TransferMapping {
        typedef T value_type;
    };
    
    // CefBrowser
    template<> struct TransferMapping< CefRefPtr<CefBrowser> > {
        typedef std::shared_ptr<BrowserTransfer> value_type;
    };
    template<> struct TransferMapping < std::shared_ptr<BrowserTransfer> > {
        typedef CefRefPtr<CefBrowser> value_type;
    };
    
    // CefFrame
    template<> struct TransferMapping< CefRefPtr<CefFrame> > {
        typedef std::shared_ptr<FrameTransfer> value_type;
    };
    
    template<> struct TransferMapping < std::shared_ptr<FrameTransfer> > {
        typedef CefRefPtr<CefFrame> value_type;
    };
    
    // CefBrowserHost
    template<> struct TransferMapping < CefRefPtr<CefBrowserHost> > {
        typedef std::shared_ptr<BrowserHostTransfer> value_type;
    };
    
    template<> struct TransferMapping < std::shared_ptr<BrowserHostTransfer> > {
        typedef CefRefPtr<CefBrowserHost> value_type;
    };
    
    // CefRequest
    template<> struct TransferMapping < CefRefPtr<CefRequest> > {
        typedef std::shared_ptr<RequestTransfer> value_type;
    };
    
    template<> struct TransferMapping < std::shared_ptr<RequestTransfer> > {
        typedef CefRefPtr<CefRequest> value_type;
    };
    
    // CefURLRequest
    template<> struct TransferMapping < CefRefPtr<CefURLRequest> > {
        typedef std::shared_ptr<URLRequestTransfer> value_type;
    };
    
    template<> struct TransferMapping < std::shared_ptr<URLRequestTransfer> > {
        typedef CefRefPtr<CefURLRequest> value_type;
    };
    
    // CefURLRequestClient
    template<> struct TransferMapping < CefRefPtr<CefURLRequestClient> > {
        typedef std::shared_ptr<URLRequestClientTransfer> value_type;
    };
    
    template<> struct TransferMapping<std::shared_ptr<URLRequestClientTransfer> > {
        typedef CefRefPtr<CefURLRequestClient> value_type;
    };
    
    
    
}

#endif // AMO_TRANSFERMAPPING_HPP__
