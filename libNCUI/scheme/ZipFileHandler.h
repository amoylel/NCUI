// Created by amoylel on 12/11/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_ZIPFILEHANDLER_H__
#define AMO_ZIPFILEHANDLER_H__



#include <string>


#include "handler/CefHeader.hpp"
#include "scheme/LocalSchemeHandler.h"

namespace amo {
    class ZipFileHandler : public LocalSchemeHandler {
    public:
        ZipFileHandler(const std::string& url, const std::string& u8ZipPath,
                       const std::string& u8File);
                       
                       
        /*!
         * @fn	virtual bool ZipFileHandler::ProcessRequest(
         * 		CefRefPtr<CefRequest> request,
         * 		CefRefPtr<CefCallback> callback) override;
         *
         * @brief	Process the request.
         *
         * @param	request 	The request.
         * @param	callback	The callback.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool ProcessRequest(CefRefPtr<CefRequest> request,
                                    CefRefPtr<CefCallback> callback) override;
                                    
                                    
        IMPLEMENT_REFCOUNTING(ZipFileHandler);
        
    private:
        amo::u8string m_strUrl; //
        amo::u8string m_strFile; //
        amo::u8string m_strZip; //
    };
}



#endif // AMO_ZIPFILEHANDLER_H__