// Created by amoylel on 31/05/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef LIBNCUI_DLLFILEHANDLER_67D9F884_71E9_4CA5_B095_7B4C242667D3_H__
#define LIBNCUI_DLLFILEHANDLER_67D9F884_71E9_4CA5_B095_7B4C242667D3_H__








#include <string>


#include "handler/CefHeader.hpp"
#include "scheme/LocalSchemeHandler.h"

namespace amo {
    class DllFileHandler : public LocalSchemeHandler {
    public:
        DllFileHandler(const std::string& url, const std::string& u8ZipPath,
                       const std::string& u8File);
                       
                       
        /*!
         * @fn	virtual bool DllFileHandler::ProcessRequest(
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
                                    
                                    
        IMPLEMENT_REFCOUNTING(DllFileHandler);
        
    private:
        amo::u8string m_strUrl; //
        amo::u8string m_strFile; //
        amo::u8string m_strZip; //
    };
}



#endif //LIBNCUI_DLLFILEHANDLER_67D9F884_71E9_4CA5_B095_7B4C242667D3_H__