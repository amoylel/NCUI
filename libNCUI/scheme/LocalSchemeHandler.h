// Created by amoylel on 6/22/2017.
// Copyright (c) 2017 amoylel. All rights reserved.

#ifndef AMO_SCHEMEHANDLER_H__
#define AMO_SCHEMEHANDLER_H__



#include <amo/string.hpp>
#include "handler/CefHeader.hpp"


namespace amo {

    /*!
     * @class	LocalSchemeHandler
     *
     * @brief	自定义协议local://.
     */
    
    class LocalSchemeHandler : public CefResourceHandler {
    public:
        LocalSchemeHandler();
        
        virtual ~LocalSchemeHandler() {}
        
        virtual bool ProcessRequest(CefRefPtr<CefRequest> request,
                                    CefRefPtr<CefCallback> callback) override;
                                    
        virtual void GetResponseHeaders(CefRefPtr<CefResponse> response,
                                        int64& response_length,
                                        CefString& redirectUrl) override;
                                        
        virtual void Cancel() override;
        
        
        virtual bool ReadResponse(void* data_out,
                                  int bytes_to_read,
                                  int& bytes_read,
                                  CefRefPtr<CefCallback> callback) override;
                                  
                                  
                                  
        IMPLEMENT_REFCOUNTING(LocalSchemeHandler);
    protected:
        bool ReadNativeFile(const amo::u8string& strPath,
                            CefRefPtr<CefCallback> callback);
                            
        bool readMimeType(const std::string& str);
    protected:
        std::string m_strData;
        std::string m_strMimeType;
        size_t m_nOffset;
        
    };
    
    /*!
     * @class	LocalSchemeHandlerFactory
     *
     * @brief	A local scheme handler factory.
     */
    
    class LocalSchemeHandlerFactory : public CefSchemeHandlerFactory {
    public:
        LocalSchemeHandlerFactory() {}
        
        virtual ~LocalSchemeHandlerFactory() {}
        
        /*!
         * @fn	virtual CefRefPtr<CefResourceHandler> LocalSchemeHandlerFactory::Create(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefFrame> frame,
         * 		const CefString& scheme_name,
         * 		CefRefPtr<CefRequest> request) override;
         *
         * @brief	创建一个新的 scheme handler instance to handle the request.
         *
         * @param	browser	   	The browser.
         * @param	frame	   	The frame.
         * @param	scheme_name	Name of the scheme.
         * @param	request	   	The request.
         *
         * @return	A CefRefPtr&lt;CefResourceHandler&gt;
         */
        
        virtual CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser,
                CefRefPtr<CefFrame> frame,
                const CefString& scheme_name,
                CefRefPtr<CefRequest> request) override;
                
        IMPLEMENT_REFCOUNTING(LocalSchemeHandlerFactory);
    };
    
}

#endif // AMO_SCHEMEHANDLER_H__

