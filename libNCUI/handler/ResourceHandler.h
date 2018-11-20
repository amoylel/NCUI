// Created by amoylel on 06/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_RESOURCEHANDLER_H__
#define AMO_RESOURCEHANDLER_H__

#include "handler/CefHeader.hpp"
namespace amo {
    class ResourceHandler : public CefResourceHandler {
    public:
    
        /*!
         * @fn	virtual bool ResourceHandler::ProcessRequest(
         * 		CefRefPtr<CefRequest> request,
         * 		CefRefPtr<CefCallback> callback);
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
                                    
        /*!
         * @fn	virtual void ResourceHandler::GetResponseHeaders(
         * 		CefRefPtr<CefResponse> response,
         * 		int64& response_length,
         * 		CefString& redirectUrl);
         *
         * @brief	Gets response headers.
         *
         * @param	response			   	The response.
         * @param 	response_length	Length of the response.
         * @param 	redirectUrl	   	URL of the redirect.
         */
        
        virtual void GetResponseHeaders(CefRefPtr<CefResponse> response,
                                        int64& response_length,
                                        CefString& redirectUrl) override;
                                        
        /*!
         * @fn	virtual bool ResourceHandler::ReadResponse(
         * 		void* data_out,
         * 		int bytes_to_read,
         * 		int& bytes_read,
         * 		CefRefPtr<CefCallback> callback);
         *
         * @brief	Reads a response.
         *
         * @param 	data_out  	If non-null, the data out.
         * @param	bytes_to_read	  	The bytes to read.
         * @param 	bytes_read	The bytes read.
         * @param	callback		  	The callback.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool ReadResponse(void* data_out,
                                  int bytes_to_read,
                                  int& bytes_read,
                                  CefRefPtr<CefCallback> callback) override;
                                  
        /*!
         * @fn	virtual bool ResourceHandler::CanGetCookie(const CefCookie& cookie);
         *
         * @brief	Determine if we can get cookie.
         *
         * @param	cookie	The cookie.
         *
         * @return	true if we can get cookie, false if not.
         */
        
        virtual bool CanGetCookie(const CefCookie& cookie) override;
        
        /*!
         * @fn	virtual bool ResourceHandler::CanSetCookie(const CefCookie& cookie);
         *
         * @brief	Determine if we can set cookie.
         *
         * @param	cookie	The cookie.
         *
         * @return	true if we can set cookie, false if not.
         */
        
        virtual bool CanSetCookie(const CefCookie& cookie) override;
        
        /*!
         * @fn	virtual void ResourceHandler::Cancel();
         *
         * @brief	Cancels this object.
         */
        
        virtual void Cancel() override;
        
        /*!
         * @fn	ResourceHandler::IMPLEMENT_REFCOUNTING(ResourceHandler);
         *
         * @brief	Copy constructor.
         *
         * @param	parameter1	The first parameter.
         */
        
        IMPLEMENT_REFCOUNTING(ResourceHandler);
    private:
        /*! @brief	The data. */
        std::string m_strData;
        /*! @brief	Type of the mime. */
        std::string m_strMimeType;
        /*! @brief	The offset. */
        size_t m_nOffset;
    };
}

#endif // AMO_RESOURCEHANDLER_H__
