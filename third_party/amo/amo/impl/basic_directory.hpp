// Created by amoylel on 30/09/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_BASIC_DIRECTORY_C2143BE7_0DE6_4DAE_8C1D_D1D355B0AB7A_HPP__
#define AMO_BASIC_DIRECTORY_C2143BE7_0DE6_4DAE_8C1D_D1D355B0AB7A_HPP__


#include <amo/string.hpp>
#include <amo/path.hpp>
#include <shellapi.h>
namespace amo {
    template<typename CodingType>
    class basic_directory {
    public:
        typedef amo::basic_path<CodingType> path_type;
    public:
        basic_directory(const path_type& p)
            : m_path(p) {
            
        }
        
        std::string name() {
            return path_type(m_path).strip_path();
        }
        
        std::string full_name() {
            return m_path.c_str();
        }
        // 判断目录是否存在
        bool is_exists() const {
            return m_path.is_directory();
        }
        
        // 创建目录
        bool create() {
            return m_path.create_directory();
        }
        
        path_type get_path() const {
            return m_path;
        }
        // 清空目录下的所有子目录及文件，只留当前目录
        bool empty() {
            std::vector<path_type> vec;
            for_each([&](path_type & p) {
                vec.push_back(p);
                return true;
            }, false);
            
            bool bOk = true;
            
            // 只要有一个删除失败都认为删除失败
            for (auto& p : vec) {
                if (!deletePath(p)) {
                    bOk = false;
                }
            }
            
            return bOk;
        }
        
        // 删除文件夹
        bool remove() {
            return deletePath(m_path);
        }
        
        // 移动文件夹
        bool move_to(const path_type& to) {
            return movePath(to, m_path);
        }
        
        //复制文件夹
        bool copy_to(const path_type& to) {
            return copyPath(to, m_path);
        }
        
        // 重命名文件夹
        bool rename(const path_type& to) {
            return renamePath(to, m_path);
        }
        
        /**
        * @fn	int directory::count(bool recursion = false, bool includeDir = false)
        *
        * @brief	获取当前文件夹下的文件个数.
        *
        * @param	recursion 	(Optional) true to recursion.
        * @param	includeDir	(Optional) true to include, false to exclude the dir.
        *
        * @return	An int.
        */
        
        int count(bool recursion = false, bool includeDir = false) {
            int nCount = 0;
            transfer([&](path_type & p) {
                if (p.is_directory() && includeDir) {
                    ++nCount;
                } else {
                    ++nCount;
                }
            }, recursion);
            return nCount;
        }
        
        /**
        * @fn	uint64_t directory::size()
        *
        * @brief	获取当前文件夹下文件大小.
        *
        * @return	An uint64_t.
        */
        
        uint64_t size() {
        
            uint64_t nSize = 0;
            transfer([&](path_type & p) {
                if (p.is_directory()) {
                    return;
                } else {
                    WIN32_FILE_ATTRIBUTE_DATA fad;
                    auto wvec = p.wbytes();
                    
                    if (GetFileAttributesExW(wvec.data(), GetFileExInfoStandard, &fad)) {
                        nSize += ((ULONG64)fad.nFileSizeHigh & 0xffffffff) << 32 |
                                 (ULONG64)fad.nFileSizeLow;
                    }
                }
            }, true);
            return nSize;
            
        }
        
        void transfer(std::function<void(path_type&)> fn_cb, bool recursion = false) {
            // 判断目录是否存在
            if (!this->is_exists()) {
                return;
            }
            
            WCHAR szFind[MAX_PATH] = { (L"\0") };
            WIN32_FIND_DATAW findFileData;
            auto wvec = m_path.wbytes();
            wcscpy(szFind, wvec.data());
            //这里一定要指明通配符，不然不会读取所有文件和目录
            wcscat(szFind, (L"\\*.*"));
            
            HANDLE hFind = ::FindFirstFileW(szFind, &findFileData);
            
            if (INVALID_HANDLE_VALUE == hFind) {
                return;
            }
            
            do {
            
                std::wstring pStr(findFileData.cFileName);
                
                if (pStr != L"." && pStr != L"..") {
                    path_type p = m_path;
                    p.append(path_type(findFileData.cFileName));
                    
                    
                    if (fn_cb) {
                        fn_cb(p);
                    }
                    
                    //printf("%s\\%s\n", lpPath, findFileData.cFileName);
                    // 如题是一个目录，且recursion=true时才进去
                    if (p.is_directory() && recursion) {
                        p.transfer(fn_cb, recursion);
                        //return false;
                    }
                    
                    
                }
                
                
            } while (::FindNextFileW(hFind, &findFileData));
            
            ::FindClose(hFind);
            return;
        }
        
        bool for_each(amo::function<bool(path_type&)> fn_cb, bool recursion = false) {
            // 判断目录是否存在
            if (!this->is_exists()) {
                return false;
            }
            
            WCHAR szFind[MAX_PATH] = { (L"\0") };
            WIN32_FIND_DATAW findFileData;
            auto wvec = m_path.wbytes();
            wcscpy(szFind, MAX_PATH, wvec.data());
            //这里一定要指明通配符，不然不会读取所有文件和目录
            wcscat(szFind, (L"\\*.*"));
            
            HANDLE hFind = ::FindFirstFileW(szFind, &findFileData);
            
            if (INVALID_HANDLE_VALUE == hFind) {
                return false;
            }
            
            do {
            
                std::wstring pStr(findFileData.cFileName);
                
                if (pStr != "." && pStr != "..") {
                    path_type p = m_path;
                    p.append(path_type(findFileData.cFileName));
                    bool bContinue = true;
                    
                    if (fn_cb) {
                        bContinue = fn_cb(p);
                    }
                    
                    //printf("%s\\%s\n", lpPath, findFileData.cFileName);
                    // 如题是一个目录，且recursion=true时才进去
                    if (p.is_directory() && recursion) {
                        bContinue = p.transfer(fn_cb, recursion);
                        //return false;
                    }
                    
                    if (!bContinue) {
                        break;
                    }
                }
                
                
            } while (::FindNextFileW(hFind, &findFileData));
            
            ::FindClose(hFind);
            return true;
        }
        
        
        
    private:
    
        /*	fFlags:    是在进行文件操作时的过程和状态控制标识。它主要有如下一些标识及其组合:
        FOF_FILESONLY:不执行通配符，只执行文件。
        FOF_ALLOWUNDO:保存 UNDO信息，以便恢复。
        FOF_NOCONFIRMATION : 在出现目标文件已存在的时候，如果不设置此项，则它会出现确认是否覆盖的对话框，设置此项则自动确认，进行覆盖，不出现对话框。
        FOF_NOERRORUI : 设置此项后，当文件处理过程中出现错误时，不出现错误提示，否则会进行错误提示。
        FOF_RENAMEONCOLLISION : 当已存在文件名时，对其进行更换文件名提示。
        FOF_SILENT : 不显示进度对话框。
        FOF_WANTMAPPINGHANDLE : 要求SHFileOperation()函数返回正处于操作状态的实际文件列表，文件列表名柄保存在hNameMappings成员中。
        SHFILEOPSTRUCT将包含一个SHNAMEMAPPING结构的数组，此数组保存由SHELL计算的每个处于操作状态的文件的新旧路径。*/
        
        // 复制文件或文件夹
        bool copyPath(const path_type& to, const path_type& from) {
        
            std::wstring src = from.generic_wstring();
            std::wstring dst = to.generic_wstring();
            amo::string_utils::replace(src, L"/", L"\\");
            amo::string_utils::replace(dst, L"/", L"\\");
            
            SHFILEOPSTRUCTW FileOp = { 0 };
            FileOp.hwnd = HWND_DESKTOP;
            FileOp.fFlags = FOF_NO_UI |   //不出现确认对话框
                            FOF_SILENT | // 不显示进度
                            FOF_NOCONFIRMMKDIR; //需要时直接创建一个文件夹,不需用户确定
            FileOp.pFrom = src.c_str();
            FileOp.pTo = dst.c_str();
            FileOp.wFunc = FO_COPY;
            return (SHFileOperationW(&FileOp) == 0);
        }
        
        // 删除一个目录
        bool deletePath(const path& p) {
        
            std::wstring src = p.generic_wstring();
            amo::string_utils::replace(src, L"/", L"\\");
            
            SHFILEOPSTRUCTW FileOp = { 0 };
            FileOp.hwnd = HWND_DESKTOP;
            FileOp.fFlags = /*FOF_SILENT |*/ // 不显示进度
                FOF_NOCONFIRMATION //不出现确认对话框
                | FOF_NOERRORUI; //设置此项后，当文件处理过程中出现错误时，不出现错误提示，否则会进行错误提示。
            FileOp.pFrom = src.c_str();
            FileOp.pTo = NULL;      //一定要是NULL
            FileOp.wFunc = FO_DELETE;    //删除操作
            return SHFileOperationW(&FileOp) == 0;
            
        }
        
        //移动文件或文件夹
        bool movePath(const path& to, const path& from) {
        
            std::wstring src = from.generic_wstring();
            std::wstring dst = to.generic_wstring();
            amo::string_utils::replace(src, L"/", L"\\");
            amo::string_utils::replace(dst, L"/", L"\\");
            
            SHFILEOPSTRUCTW FileOp = { 0 };
            FileOp.hwnd = HWND_DESKTOP;
            FileOp.fFlags = FOF_NO_UI |   //不出现确认对话框
                            FOF_SILENT | // 不显示进度
                            FOF_NOCONFIRMMKDIR; //需要时直接创建一个文件夹,不需用户确定
            FileOp.pFrom = src.c_str();
            FileOp.pTo = dst.c_str();
            FileOp.wFunc = FO_MOVE;
            return SHFileOperationW(&FileOp) == 0;
        }
        
        
        //重命名文件或文件夹
        bool renamePath(const path& to, const path& from) {
        
            std::wstring src = from.generic_wstring();
            std::wstring dst = to.generic_wstring();
            amo::string_utils::replace(src, L"/", L"\\");
            amo::string_utils::replace(dst, L"/", L"\\");
            SHFILEOPSTRUCTW FileOp = { 0 };
            FileOp.hwnd = HWND_DESKTOP;
            FileOp.fFlags = FOF_NO_UI     //不出现确认对话框
                            | FOF_SILENT // 不显示进度
                            | FOF_NOCONFIRMATION; //如果设置，确认对话框在任何情况下都不出现
            FileOp.pFrom = src.c_str();
            FileOp.pTo = dst.c_str();
            FileOp.wFunc = FO_RENAME;
            
            
            return SHFileOperationW(&FileOp) == 0;
        }
        
    private:
        path_type m_path;
    };
    
    
}



#endif //AMO_BASIC_DIRECTORY_C2143BE7_0DE6_4DAE_8C1D_D1D355B0AB7A_HPP__