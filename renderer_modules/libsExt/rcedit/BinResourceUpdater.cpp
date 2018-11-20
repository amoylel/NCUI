#include "stdafx.h"
#include "BinResourceUpdater.h"

namespace amo {




    BinResourceUpdater::BinResourceUpdater(const std::string& exeName)
        : m_filename(exeName) {
        
    }
    
    bool BinResourceUpdater::updateZipResource(const std::string& zipname,
            int resourceID /*= 131*/,
            int lang_id /*= LANG_CHINESE*/) {
        amo::file file(zipname);
        
        if (!file.is_exists()) {
            return false;
        }
        
        int fileSize = file.size();
        
        if (fileSize == 0) {
            return false;
        }
        
        std::vector<char> vec(fileSize, 0);
        char *pData = vec.data();
        
        if (fileSize != file.get_filestream(std::ios::in | std::ios::binary)->read_some(pData, fileSize)) {
            return false;
        }
        
        std::wstring wstr = amo::u8string(m_filename, true).to_wide();
        const wchar_t *wstrName = wstr.c_str();
        
        
        HANDLE upResHandle = BeginUpdateResourceW(wstrName, FALSE);
        
        if (upResHandle == NULL) {
        
            return false;
        }
        
        
        BOOL ret = UpdateResourceW(upResHandle, L"ZIPRES",
                                   MAKEINTRESOURCEW(resourceID),
                                   MAKELANGID(LANG_CHINESE, 0x2),
                                   pData,
                                   fileSize);
                                   
        if (ret == FALSE) {
            return false;
        }
        
        ret = EndUpdateResourceW(upResHandle, FALSE);
        
        if (ret == FALSE) {
        
            return false;
        }
        
        return true;
    }
    
}