#include "stdafx.h"
#include "scheme/DllFileHandler.h"
#include "scheme/DllFileManager.h"




namespace amo {


    DllFileHandler::DllFileHandler(const std::string& url,
                                   const std::string& u8ZipPath,
                                   const std::string& u8File)
        : m_strUrl(url, true)
        , m_strFile(u8File, true)
        , m_strZip(u8ZipPath, true) {
        
        
        m_strUrl = amo::util().getUrlFromUtf8(m_strUrl);
        m_strFile = amo::util().getUrlFromUtf8(m_strFile);
        m_strZip = amo::util().getUrlFromUtf8(m_strZip);
    }
    
    bool DllFileHandler::ProcessRequest(CefRefPtr<CefRequest> request,
                                        CefRefPtr<CefCallback> callback) {
        //AMO_TIMER_ELAPSED();
        amo::u8string url = amo::util().getUrlFromUtf8(request->GetURL());
        
        if (url != m_strUrl) {
            return false;
        }
        
        amo::u8string dllPath = m_strZip;
        dllPath.replace(amo::u8string("\\", true), amo::u8string("/", true));
        //zipPath = "web/web.zip";
        
        std::shared_ptr<amo::loader> zf = DllFileManager::getInstance()->get(
                                              dllPath);
                                              
        if (!zf) {
            return false;
        }
        
        amo::u8path p(m_strFile);
        p.remove_front_backslash();
        amo::u8string strFile(p.c_str(), false);
        strFile.replace(amo::u8string("\\", true), amo::u8string("/", true));
        
        std::string filePath = strFile;
        char* val = NULL;
        amo::optional<int> retval  = zf->exec<int, const char*, char**>("readAsText",
                                     filePath.c_str(), &val);
                                     
        if (!retval || *retval == 0 || val == NULL) {
            amo::cerr << "failed to find resource file:" << strFile.to_ansi() << amo::endl;
            return false;
        }
        
        std::string ssss(val, *retval);
        m_strData = ssss;
        
        bool bHandled = readMimeType(p.find_extension());
        //AMO_TIMER_ELAPSED();
        /*    OutputDebugStringA(strFile.c_str());
          OutputDebugStringA("\n");*/
        
        if (bHandled) {
            callback->Continue();
            return true;
        }
        
        return false;
        
    }
    
}