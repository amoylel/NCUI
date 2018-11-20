// Created by amoylel on 08/05/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NATIVEFILEHANDLER_H__
#define AMO_NATIVEFILEHANDLER_H__

#include <string>


#include "handler/CefHeader.hpp"
#include "scheme/LocalSchemeHandler.h"

namespace amo {
    class NativeFileHandler : public LocalSchemeHandler {
    public:
        NativeFileHandler(const std::string& url, const std::string& file);
        IMPLEMENT_REFCOUNTING(NativeFileHandler);
        
        /*!
         * @fn	virtual bool NativeFileHandler::ProcessRequest(
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
                                    
                                    
                                    
                                    
    private:
        std::string m_strUrl; // utf8
        std::string m_strFile; // utf8
    };
}

#endif // AMO_NATIVEFILEHANDLER_H__

