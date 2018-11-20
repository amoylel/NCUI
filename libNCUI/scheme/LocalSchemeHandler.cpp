#include "stdAfx.h"

#include "scheme/LocalSchemeHandler.h"

#include <algorithm>
#include <fstream>
#include <streambuf>
#include <fstream>
#include <sstream>
#include <shlwapi.h>
#include <string>

#include <amo/string.hpp>
#include <amo/logger.hpp>
#include "utility/utility.hpp"
#include <amo/path.hpp>


namespace amo {


    struct MimeInfo {
        const char* const mime_type;
        const char* const extensions;  // comma separated list
    };
    
    
    
    static const MimeInfo primary_mappings[] = {
        { "text/html", "html,htm,shtml,shtm,md" },
        { "text/css", "css" },
        { "text/xml", "xml" },
        { "image/gif", "gif" },
        { "image/jpeg", "jpeg,jpg" },
        { "image/webp", "webp" },
        { "image/png", "png" },
        { "video/mp4", "mp4,m4v" },
        { "audio/x-m4a", "m4a" },
        { "audio/mp3", "mp3" },
        { "video/ogg", "ogv,ogm" },
        { "audio/ogg", "ogg,oga,opus" },
        { "video/webm", "webm" },
        { "audio/webm", "webm" },
        { "audio/wav", "wav" },
        { "audio/flac", "flac" },
        { "application/xhtml+xml", "xhtml,xht,xhtm" },
        { "application/x-chrome-extension", "crx" },
        { "multipart/related", "mhtml,mht" }
    };
    
    static const MimeInfo secondary_mappings[] = {
        { "application/octet-stream", "exe,com,bin" },
        { "application/gzip", "gz" },
        { "application/pdf", "pdf" },
        { "application/postscript", "ps,eps,ai" },
        { "application/javascript", "js" },
        { "application/font-woff", "woff" },
        { "image/bmp", "bmp" },
        { "image/x-icon", "ico" },
        { "image/vnd.microsoft.icon", "ico" },
        { "image/jpeg", "jfif,pjpeg,pjp" },
        { "image/tiff", "tiff,tif" },
        { "image/x-xbitmap", "xbm" },
        { "image/svg+xml", "svg,svgz" },
        { "image/x-png", "png" },
        { "message/rfc822", "eml" },
        { "text/plain", "txt,text" },
        { "text/html", "ehtml" },
        { "application/rss+xml", "rss" },
        { "application/rdf+xml", "rdf" },
        { "text/xml", "xsl,xbl,xslt" },
        { "application/vnd.mozilla.xul+xml", "xul" },
        { "application/x-shockwave-flash", "swf,swl" },
        { "application/pkcs7-mime", "p7m,p7c,p7z" },
        { "application/pkcs7-signature", "p7s" },
        { "application/x-mpegurl", "m3u8" },
        { "application/json", "json" }
        
    };
    
    template <typename T>
    int getArrayLen(T& array) {
        //使用模板定义一 个函数getArrayLen,该函数将返回数组array的长度
        return (sizeof(array) / sizeof(array[0]));
        
    }
    
    // Finds mime type of |ext| from |mappings|.
    std::string FindMimeType(const MimeInfo* mappings,
                             size_t mappings_len,
                             const std::string& ext) {
        amo::u8string str(ext, true);
        str.to_lower();
        
        for (size_t i = 0; i < mappings_len; ++i) {
            const char* extensions = mappings[i].extensions;
            
            for (;;) {
                size_t end_pos = strcspn(extensions, ",");
                
                // The length check is required to prevent the StringPiece below from
                // including uninitialized memory if ext is longer than extensions.
                if (end_pos == ext.size() &&
                        (memcmp(extensions, str.c_str(), str.size()) == 0)
                   ) {
                    return mappings[i].mime_type;
                }
                
                extensions += end_pos;
                
                if (!*extensions) {
                    break;
                }
                
                extensions += 1;  // skip over comma
            }
        }
        
        
        return "";
    }
    
    bool LocalSchemeHandler::ReadResponse(void* data_out,
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
    
    
    void LocalSchemeHandler::Cancel() {
        CEF_REQUIRE_IO_THREAD();
    }
    
    void LocalSchemeHandler::GetResponseHeaders(CefRefPtr<CefResponse> response,
            int64& response_length,
            CefString& redirectUrl) {
        CEF_REQUIRE_IO_THREAD();
        DCHECK(!m_strData.empty());
        
        response->SetMimeType(m_strMimeType);
        response->SetStatus(200);
        
        response_length = m_strData.length();
    }
    bool LocalSchemeHandler::ProcessRequest(CefRefPtr<CefRequest> request,
                                            CefRefPtr<CefCallback> callback) {
                                            
        CEF_REQUIRE_IO_THREAD();
        
        bool bHandled = false;
        
        amo::u8string strUrl(request->GetURL(), true);
        strUrl.replace(amo::u8string("\\", true), amo::u8string("/", true));
        
        
        int nIndex = strUrl.find(amo::u8string("@file:///", true));
        
        if (nIndex != -1) {
            strUrl = strUrl.substr(nIndex + 9);
        } else {
            nIndex = strUrl.find(amo::u8string("local://file/", true));
            
            if (nIndex != -1) {
                strUrl = strUrl.substr(nIndex + 13);
            }
        }
        
        amo::u8string tmp(AppSettings::getInstance()->toAbsolutePath(
                              strUrl.to_utf8()), true);
                              
        strUrl = util::getUrlFromUtf8(tmp);
        
        
        if (!amo::u8path(strUrl).is_absolute()) {
            strUrl = amo::u8string(AppSettings::getInstance()->toAbsolutePath(
                                       std::string("%webDir%") + strUrl.to_utf8()), true);
        }
        
        return ReadNativeFile(strUrl, callback);
        
    }
    /* bool LocalSchemeHandler::ProcessRequest(CefRefPtr<CefRequest> request,
                                             CefRefPtr<CefCallback> callback) {
    
         CEF_REQUIRE_IO_THREAD();
    
         bool bHandled = false;
    
         std::string url = request->GetURL();
         url = AppSettings::getInstance()->toAbsolutePath(url);
    
         amo::u8string strUrl = util::getUrlFromUtf8(request->GetURL());
         strUrl.replace(amo::u8string("\\", true), amo::u8string("/", true));
         int nIndex = strUrl.find(amo::u8string("local://file/", true));
    
         if (nIndex != -1) {
             strUrl = strUrl.substr(nIndex + 13);
         }
    
         if (!amo::u8path(strUrl).is_absolute()) {
             std::string nativePath = AppSettings::getInstance()->toAbsolutePath(
                                          std::string("%webDir%") + strUrl.to_utf8());
             strUrl = amo::u8string(nativePath, true);
         }
    
         strUrl.replace(amo::u8string("\\", true), amo::u8string("/", true));
    
         nIndex = strUrl.find(amo::u8string("@file:///", true));
    
         if (nIndex != -1) {
             strUrl = strUrl.substr(nIndex + 9);
         }
    
         if (!amo::u8path(strUrl).is_absolute()) {
             strUrl = amo::u8string(AppSettings::getInstance()->toAbsolutePath(
                                        std::string("%webDir%") + strUrl.to_utf8()), true);
         }
    
         return ReadNativeFile(strUrl, callback);
    
     }*/
    
    
    bool LocalSchemeHandler::ReadNativeFile(const amo::u8string& strPath,
                                            CefRefPtr<CefCallback> callback) {
                                            
        //获取文件扩展名
        std::string ext = amo::u8path(strPath.c_str()).find_extension();
        
        //读取文件
        std::ifstream tail_ifs(strPath.to_wide(),
                               std::ios::ios_base::binary | std::ios::ios_base::in);
                               
        if (!tail_ifs.is_open()) {
        
            return false;	//文件不存在
        }
        
        std::stringstream tail_buffer;
        tail_buffer << tail_ifs.rdbuf();
        std::string str(tail_buffer.str());
        m_strData = str;
        
        if (m_strData.empty()) {
        
            return false;    //没有读取到数据，返回false
        }
        
        bool bHandled = readMimeType(ext);
        
        
        if (bHandled) {
            callback->Continue();
            return true;
        }
        
        
        return false;
    }
    
    bool LocalSchemeHandler::readMimeType(const std::string& str) {
        bool bHandled = true;
        
        std::string ext = str;
        
        if (ext.find(".") == 0) {
            ext = ext.substr(1);
        }
        
        m_strMimeType = FindMimeType(primary_mappings,
                                     getArrayLen(primary_mappings),
                                     ext);
                                     
        if (m_strMimeType.empty()) {
            m_strMimeType = FindMimeType(secondary_mappings,
                                         getArrayLen(secondary_mappings),
                                         ext);
        }
        
        if (m_strMimeType.empty()) {
            m_strMimeType = "text/html";
            bHandled = false;
        }
        
        return bHandled;
    }
    
    LocalSchemeHandler::LocalSchemeHandler() : m_nOffset(0) {
    
    }
    
    CefRefPtr<CefResourceHandler> LocalSchemeHandlerFactory::Create(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        const CefString& scheme_name,
        CefRefPtr<CefRequest> request) {
        CEF_REQUIRE_IO_THREAD();
        return new LocalSchemeHandler();
    }
}