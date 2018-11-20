#include "stdafx.h"
#include "scheme/ZipFileHandler.h"
#include "utility/libzippp.h"
#include "scheme/ZipFileManager.h"



namespace amo {


    ZipFileHandler::ZipFileHandler(const std::string& url,
                                   const std::string& u8ZipPath,
                                   const std::string& u8File)
        : m_strUrl(url, true)
        , m_strFile(u8File, true)
        , m_strZip(u8ZipPath, true) {
        
        
        m_strUrl = amo::util().getUrlFromUtf8(url);
        m_strFile = amo::util().getUrlFromUtf8(u8File);
        m_strZip = amo::util().getUrlFromUtf8(u8ZipPath);
    }
    
    bool ZipFileHandler::ProcessRequest(CefRefPtr<CefRequest> request,
                                        CefRefPtr<CefCallback> callback) {
        amo::u8string url = amo::util().getUrlFromUtf8(request->GetURL());
        
        if (url != m_strUrl) {
            return false;
        }
        
        amo::u8string zipPath = m_strZip;
        zipPath.replace(amo::u8string("\\", true), amo::u8string("/", true));
        
        std::shared_ptr<libzippp::ZipArchive> zf;
        zf = ZipFileManager::getInstance()->get(zipPath);
        
        if (!zf) {
            return false;
        }
        
        amo::u8path p(m_strFile);
        p.remove_front_backslash();
        amo::u8string strFile(p.c_str(), true);
        strFile.replace(amo::u8string("\\", true), amo::u8string("/", true));
        
      /*  std::vector<libzippp::ZipEntry>  vec = zf->getEntries();
        
        for (auto& p : vec) {
            amo::cdevel << p.getName() << amo::endl;
        }*/
        
        auto entry =  zf->getEntry(strFile.to_utf8(), false, false);
        
        if (entry.isNull()) {
            amo::cerr << "failed to find resource file:" << strFile.to_ansi() << amo::endl;
            return false;
        }
        
        m_strData = entry.readAsText();
		if (m_strData.empty()) {
			$cdevel("empty data.");
			return false;
		}
        bool bHandled = readMimeType(p.find_extension());
        
        if (bHandled) {
            callback->Continue();
            return true;
        }
        
        return false;
        
    }
    
}