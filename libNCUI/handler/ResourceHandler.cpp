#include "stdafx.h"  

#include "handler/ResourceHandler.h"

#include <algorithm>

#include "handler/CefHeader.hpp"

namespace amo {
    void ResourceHandler::Cancel() {
        return;
    }

    bool ResourceHandler::CanSetCookie(const CefCookie& cookie) {
        return true;
    }

    bool ResourceHandler::CanGetCookie(const CefCookie& cookie) {
        return true;
    }

    bool ResourceHandler::ReadResponse(void* data_out,
                                       int bytes_to_read,
                                       int& bytes_read,
                                       CefRefPtr<CefCallback> callback) {
        CEF_REQUIRE_IO_THREAD();

        bool bHasData = false;
        bytes_read = 0;

        if (m_nOffset < m_strData.length()) {
            int transferSize = (std::min)(bytes_to_read,
                                          static_cast<int>(m_strData.length() - m_nOffset));
            memcpy(data_out, m_strData.c_str() + m_nOffset, transferSize);
            m_nOffset += transferSize;

            bytes_read = transferSize;
            bHasData = true;
        }

        return bHasData;
    }

    void ResourceHandler::GetResponseHeaders(CefRefPtr<CefResponse> response,
            int64& response_length,
            CefString& redirectUrl) {
        CEF_REQUIRE_IO_THREAD();

        DCHECK(!m_strData.empty());

        response->SetMimeType(m_strMimeType);
        response->SetStatus(200);

        response_length = m_strData.length();
    }

    bool ResourceHandler::ProcessRequest(CefRefPtr<CefRequest> request,
                                         CefRefPtr<CefCallback> callback) {
        callback->Continue();
        return true;
    }

}