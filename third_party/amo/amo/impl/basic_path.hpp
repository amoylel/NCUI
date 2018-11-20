// Created by amoylel on 29/09/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_BASIC_PATH_D025EE85_6284_4EB1_AB33_6A15CF1692B2_HPP__
#define AMO_BASIC_PATH_D025EE85_6284_4EB1_AB33_6A15CF1692B2_HPP__

#include <amo/config.hpp>
#include <memory.h>
#include <string>
#include <stdio.h>

#include <amo/type_traits.hpp>
#include <amo/windows.hpp>
#include <amo/type_traits.hpp>
#include <amo/functional.hpp>
#include <amo/filesystem.hpp>
#include <amo/string/string_utils.hpp>
#include <amo/string/tiny_string.hpp>
#include <amo/string/coding_type.hpp>



#ifdef OS_WIN

#include <shlwapi.h>
#include <shlobj.h>
#include <shellapi.h>
#include <direct.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Shell32.lib")

#endif // WIN32
#include <algorithm>

#ifdef OS_LINUX
#include <unistd.h>
#endif // OS_LINUX


namespace amo {
    template<typename CodingType>
    class basic_path {
    public:
        typedef std::string string_type;
    public:
        static   StringCodingType get_coding_type()  {
            return CodingType::coding_type;
        }
        
        static bool is_utf8()  {
            return CodingType::coding_type == StringCodingType::CODING_TYPE_UTF8;
        }
        
    public:
        basic_path() {
            memset(m_path, 0, 1000);
        }
        
        basic_path(const basic_path& rhs) {
            memset(m_path, 0, 1000);
            memcpy(m_path, rhs.m_path, 1000);
        }
        
        // 认为是CodingType
        basic_path(const std::string& str_path) {
            memset(m_path, 0, 1000);
            memcpy(m_path, str_path.c_str(), str_path.size());
        }
        
        //// 认为是CodingType
        basic_path(const amo::tiny_string& str_path) {
            memset(m_path, 0, 1000);
            memcpy(m_path, str_path.c_str(), str_path.size());
        }
        
        basic_path(const char* str_path) {
            memset(m_path, 0, 1000);
            strcpy(m_path, str_path);
        }
        
        basic_path(const std::wstring& wstr_path) {
            std::string str_path = amo::string_utils::wide_to_string(wstr_path,
                                   get_coding_type());
            memset(m_path, 0, 1000);
            strcpy(m_path, str_path.c_str());
        }
        
        basic_path(const wchar_t* wstr_path) {
            std::string str_path = amo::string_utils::wide_to_string(wstr_path,
                                   get_coding_type());
            memset(m_path, 0, 1000);
            strcpy(m_path, str_path.c_str());
        }
        
        
        basic_path(const amo::filesystem::path& p) {
            filesystem_path(p);
        }
        
        /**
        * @fn	path& path::reset_path(const std::string& str)
        *
        * @brief	重新设置当前路径.
        *
        * @param	str	.
        *
        * @return	#path& 当前对象引用.
        */
        
        basic_path& reset_path(const std::string& str) {
            memset(m_path, 0, 1000);
            strcpy(m_path, str.c_str());
            return *this;
        }
        
        basic_path& reset_path(const std::vector<char>& vec) {
            return reset_path(vec.data());
        }
        
        basic_path& reset_path(const std::wstring& wstr) {
            std::string str = amo::string_utils::wide_to_string(wstr, get_coding_type());
            memset(m_path, 0, 1000);
            strcpy(m_path, str.c_str());
            return *this;
        }
        
        basic_path& reset_path(const std::vector<wchar_t>& wvec) {
            return reset_path(wvec.data());
        }
        
        //////////////////////////////////////////////////////////////////////////
        //
        //		判断函数
        //////////////////////////////////////////////////////////////////////////
        
        /**
        * @fn	bool path::has_blanks()
        *
        * @brief	判断当前路径是否存在空格， “\t”也算空格.
        *
        * @return	#bool true  有空格, false 无空格.
        */
        
        bool has_blanks() const {
            std::wstring str = generic_wstring();
            
            if (str.find(L" ") != -1) {
                return true;
            }
            
            if (str.find(L"\t") != -1) {
                return true;
            }
            
            return false;
        }
        
        /**
        * @fn	bool path::is_complete() const
        *
        * @brief	判断当前路径是否为完整路径.
        *
        * @return	#bool true / false.
        */
        
        bool is_complete() const {
        
            return filesystem_path().is_complete();
        }
        
        
        
        /**
        * @fn	bool path::empty() const
        *
        * @brief	当前路径 是否为空路径.
        *
        * @return	true if it succeeds, false if it fails.
        */
        
        bool empty() const {
            return filesystem_path().empty();
        }
        /**
        * @fn	bool path::filename_is_dot() const
        *
        * @brief	判断是否为".".
        *
        * @return	#bool true / false.
        */
        bool filename_is_dot() const {
            return filesystem_path().filename_is_dot();
        }
        
        /**
        * @fn	bool path::filename_is_dot_dot() const
        *
        * @brief	判断是否为"..".
        *
        * @return	true if it succeeds, false if it fails.
        */
        
        bool filename_is_dot_dot() const {
            return filesystem_path().filename_is_dot_dot();
        }
        
        /**
        * @fn	bool path::has_root_path() const
        *
        * @brief	当前路径是否具有根目录.
        *
        * @return	#bool true / false.
        */
        
        bool has_root_path() const {
            return filesystem_path().has_root_path();
        }
        
        /**
        * @fn	bool path::has_root_name() const
        *
        * @brief	当前路径是否具有根名称.
        *
        * @return	#bool true / false.
        */
        
        bool has_root_name() const {
            return filesystem_path().has_root_name();
        }
        
        /**
        * @fn	bool path::has_root_directory() const
        *
        * @brief	是否有根目录.
        *
        * @return	#bool true / false.
        */
        
        bool has_root_directory() const {
            return filesystem_path().has_root_directory();
        }
        
        /**
        * @fn	bool path::has_relative_path() const
        *
        * @brief	是否存在相对路径.
        *
        * @return	#bool true / false.
        */
        
        bool has_relative_path() const {
            return filesystem_path().has_relative_path();
        }
        
        /**
        * @fn	bool path::has_parent_path() const
        *
        * @brief	是否存在父目录.
        *
        * @return	#bool true / false.
        */
        
        bool has_parent_path() const {
            return filesystem_path().has_parent_path();
        }
        
        /**
        * @fn	bool path::has_filename() const
        *
        * @brief	是否存在文件名
        *
        * @return	#bool true / false.
        */
        
        bool has_filename() const {
            return filesystem_path().has_filename();
        }
        
        /**
        * @fn	bool path::has_stem() const
        *
        * @brief	Query if this object has stem.
        *
        * @return	true if stem, false if not.
        */
        
        bool has_stem() const {
            return filesystem_path().has_stem();
        }
        
        /**
        * @fn	bool path::has_extension() const
        *
        * @brief	是否存在扩展名.
        *
        * @return	#bool true / false.
        */
        
        bool has_extension() const {
            return filesystem_path().has_extension();
        }
        
        /**
        * @fn	bool path::is_relative() const
        *
        * @brief	是否为相对路径.
        *
        * @return	#bool true / false.
        */
        
        bool is_relative() const {
            return filesystem_path().is_relative();
        }
        
        /**
        * @fn	bool path::is_absolute() const
        *
        * @brief	是否为绝对路径.
        *
        * @return	#bool true / false.
        */
        
        bool is_absolute() const {
            return filesystem_path().is_absolute();
        }
        
        /**
        * @fn	bool path::has_leaf() const
        *
        * @brief	Query if this object has leaf.
        *
        * @return	#bool true / false.
        */
        
        bool   has_leaf() const {
            return filesystem_path().has_leaf();
        }
        
        /**
        * @fn	bool path::has_branch_path() const
        *
        * @brief	Query if this object has branch path.
        *
        * @return	#bool true / false.
        */
        
        bool   has_branch_path() const {
            return filesystem_path().has_branch_path();
        }
        
        
        
        //////////////////////////////////////////////////////////////////////////
        //
        //		路径截断与合并函数
        //////////////////////////////////////////////////////////////////////////
        
        /*!
        * @fn	path& path::remove_args()
        *
        * @brief	去除路径的参数.
        *
        * @return	#path& 当前对象引用.
        */
        basic_path& remove_args() {
        
            std::string str = generic_string();
            
            if (str.empty()) {
                return *this;
            }
            
            if (str[str.size() - 1] == '/' || str[str.size() - 1] == '\\') {
                return *this;
            }
            
            
            
            basic_path parent = this->parent();
            int start_pos  = parent.string().size() + 1;
            
            int index = str.find(' ', start_pos);
            
            if (index != -1) {
                str = str.substr(0, index);
            }
            
            reset_path(str);
            return *this;
            
            /*$windows(
                auto wvec = wbytes();
                ::PathRemoveArgsW(wvec.data());
                reset_path(wvec);
            )
            $welse(
                auto ext = find_extension();
                change_extension(ext);
            )
            
            return *this;*/
        }
        
        /*!
        * @fn	path& path::remove_backslash()
        *
        * @brief	去除路径最后的反斜杠"\"或斜杠"/".
        *
        * @return	#path& 当前对象引用.
        */
        basic_path& remove_backslash() {
            std::string str = m_path;
            amo::string_utils::trim_right(str, "/\\");
            reset_path(str);
            return *this;
        }
        
        /*!
        * @fn	path& path::remove_front_backslash()
        *
        * @brief	移除路径前的斜杠或反斜杠.
        *
        * @return	#path& 返回当前对象.
        */
        
        basic_path& remove_front_backslash() {
        
        
            *this = this->remove_front_backslash_copy();
            return *this;
            /* std::string str = m_path;
             {
                 int nIndex = str.find("./");
            
                 if (nIndex == 0) {
                     str = str.substr(2);
                 }
             }
             {
                 int nIndex = str.find(".\\");
            
                 if (nIndex == 0) {
                     str = str.substr(2);
                 }
             }
             {
                 int nIndex = str.find("/");
            
                 if (nIndex == 0) {
                     str = str.substr(1);
                 }
             }
             {
                 int nIndex = str.find("\\");
            
                 if (nIndex == 0) {
                     str = str.substr(1);
                 }
             }
             reset_path(str);
             return *this;*/
        }
        
        /**
        * @fn	path path::remove_front_backslash_copy() const
        *
        * @brief	移除路径前面的斜杠或反斜杠.
        *
        * @return	#path 返回一个新的对象 .
        */
        
        basic_path remove_front_backslash_copy() const {
            std::string str = m_path;
            {
            
                int nIndex = str.find("./");
                
                if (nIndex == 0) {
                    str = str.substr(2);
                }
            }
            {
                int nIndex = str.find(".\\");
                
                if (nIndex == 0) {
                    str = str.substr(2);
                }
            }
            {
                int nIndex = str.find("/");
                
                if (nIndex == 0) {
                    str = str.substr(1);
                }
            }
            {
                int nIndex = str.find("\\");
                
                if (nIndex == 0) {
                    str = str.substr(1);
                }
            }
            
            if (str == m_path) {
                return basic_path(str);
            }
            
            return basic_path(str).remove_front_backslash_copy();
        }
        
        /*!
        * @fn	path& path::add_backslash()
        *
        * @brief	在路径前面加上斜杠"/", 如果路径本身存在反斜杠，那么不会添加新的斜杠
        *
        * @return	#path& 当前对象引用.
        */
        basic_path& add_backslash() {
            remove_backslash();
            std::string str = m_path;
            
            if (!str.empty()) {
                str += "/";
            }
            
            /*$windows({
            
            	if (!str.empty()) {
            		str += "\\";
            	}
            })
            $welse({
            
            	if (!str.empty()) {
            		str += "/";
            	}
            })*/
            
            reset_path(str);
            return *this;
        }
        
        
        basic_path& add_front_backslash(std::string slash = "./") {
            remove_front_backslash();
            std::string str = m_path;
            str = slash + str;
            reset_path(str);
            
            return *this;
        }
        
        
        
        /*!
        * @fn	path& path::remove_blanks()
        *
        * @brief	去除路径前后的空格.
        *
        * @return	#path& 当前对象引用.
        */
        basic_path& remove_blanks() {
            std::string str = m_path;
            amo::string_utils::trim_left(str, "\t ");
            amo::string_utils::trim_right(str, "\t ");
            reset_path(str);
            return *this;
        }
        
        /*!
        * @fn	path& path::add_extension(const string_type & ext)
        *
        * @brief	在文件路径后面加上扩展名.
        *
        * @param	#std::string 扩展名.
        *
        * @return	#path& 当前对象引用.
        */
        basic_path& add_extension(const string_type & ext) {
            if (ext.empty()) {
                return *this;
            }
            
            std::string str = m_path;
            
            if (ext[0] != '.') {
                str += '.';
            }
            
            str += ext;
            
            reset_path(str);
            return *this;
            //return change_extension(ext);
        }
        
        /*!
        * @fn	path& path::remove_file_spec()
        *
        * @brief	去除文件名，得到目录.
        *
        * @return	#path& 当前对象引用.
        */
        basic_path& remove_file_spec() {
            basic_path p = parent_path();
            reset_path(p.string());
            return *this;
        }
        
        /**
        * @fn	path path::remove_file_spec_copy() const
        *
        * @brief	去除文件名，得到目录.
        *
        * @return #path 返回一个新的对象.
        */
        
        basic_path remove_file_spec_copy() const {
            basic_path p = *this;
            p.remove_file_spec();
            return p;
        }
        
        /*!
        * @fn	path& path::unquote_spaces()
        *
        * @brief	去除路径中的首尾空格.
        *
        * @return	A reference to a path.
        */
        basic_path& unquote_spaces() {
            return remove_blanks();
        }
        /*!
        * @fn	path& path::quote_spaces()
        *
        * @brief	判断路径中是否有空格，有的话，就是用“”引号把整个路径包含起来.
        *
        * @return	A reference to a path.
        */
        basic_path& quote_spaces() {
            if (has_blanks()) {
                return quote();
            }
            
            return *this;
        }
        
        /**
        * @fn	path& path::quote()
        *
        * @brief	就是用“”引号把整个路径包含起来.
        *
        * @return	一个包含引号的路径.
        */
        
        basic_path& quote() {
            std::string str = m_path;
            amo::string_utils::trim_left(str, "\" ");
            amo::string_utils::trim_right(str, "\" \t");
            str = "\"" + str;
            str += "\"";
            reset_path(str);
            return *this;
        }
        
        /**
        * @fn	path path::quote_c()
        *
        * @brief	将整个路径用引号包起来，并返回一个新的路径.
        *
        * @return	#path 新路径.
        */
        
        basic_path quote_copy() {
            std::string str = m_path;
            amo::string_utils::trim_left(str, "\" ");
            amo::string_utils::trim_right(str, "\" \t");
            str = "\"" + str;
            str += "\"";
            return basic_path(str);
        }
        
        
        /*!
        * @fn	path& path::append(const path& other)
        *
        * @brief	将一个路径追加到另一个路径后面.
        *
        * @param	需要追加的路径.
        *
        * @return	#path& 当前对象引用.
        */
        basic_path& append(const basic_path& other) {
            if (!other.empty()) {
                this->add_backslash();
            }
            
            auto p = filesystem_path().append(other.filesystem_path().generic_wstring());
            
            this->filesystem_path(p);
            /*path p = other.remove_front_backslash_copy();
            ::PathAppendA(m_path, p.c_str());*/
            return *this;
        }
        
        
        
        /*!
        * @fn	path path::append_c(const path& other)
        *
        * @brief	追加路径.
        *
        * @param	需要追加的路径.
        *
        * @return	#path 一个新的对象.
        */
        basic_path append_c(const basic_path& other) const {
            auto p = filesystem_path().append(other.filesystem_path().c_str());
            return p;
            
        }
        
        
        bool operator==(const basic_path& rhs) const {
            basic_path p1(*this);
            basic_path p2(rhs);
            p1.absolute();
            p2.absolute();
            p1.normalize();
            p2.normalize();
            return p1.filesystem_path().compare(p2.filesystem_path()) == 0;
        }
        
        /**
        * @fn	path path::append_copy(const path& other) const
        *
        * @brief	追加路径.
        *
        * @param	需要追加的路径.
        *
        * @return	#path 一个新的对象.
        */
        
        basic_path append_copy(const basic_path& other) const {
            return append_c(other);
        }
        
        /**
        * @fn	path path::trim_left() const
        *
        * @brief	去掉左边的 ./\ 等字符.
        *
        * @return	#path 一个新的对象.
        */
        
        basic_path trim_left() const {
            string_type str(m_path);
            amo::string_utils::trim_left(str, " \t");
            return basic_path(str).remove_front_backslash();
            remove_front_backslash_copy();
            amo::string_utils::trim_left(str, "\\/.");
            return basic_path(str);
            
        }
        
        /*!
        * @fn	path& path::combine(const path& other)
        *
        * @brief	合并两个路径.
        *
        * @param	other	The other.
        *
        * @return	A reference to a path.
        */
        basic_path& combine(const basic_path& other) {
            return append(other);
        }
        
        /*!
        * @fn	path& path::skip_root()
        *
        * @brief	去掉路径中的磁盘符或UNC部分.
        *
        * @return	#path& 当前对象引用.
        */
        basic_path& skip_root() {
            basic_path p = relative_path();
            this->filesystem_path(p.filesystem_path());
            return *this;
        }
        
        /*!
        * @fn	path& path::strip_path()
        *
        * @brief	去掉路径中的目录部分，得到文件名.
        *
        * @return	A reference to a path.
        */
        basic_path& strip_path() {
            this->filesystem_path(this->filename().filesystem_path());
            return *this;
        }
        
        /**
        * @fn	path path::strip_path_copy() const
        *
        * @brief	去掉路径中的目录部分，得到文件名.
        *
        * @return	#path.
        */
        
        basic_path strip_path_copy() const {
            return this->filename();
        }
        
        /*!
        * @fn	path& path::strip_to_root()
        *
        * @brief	去掉路径的文件部分，得到根目录.
        *
        * @return	A reference to a path.
        */
        basic_path& strip_to_root() {
            this->filesystem_path(root_path().filesystem_path());
            return *this;
        }
        /*!
        * @fn	path& path::compact_path(int dx = 120)
        *
        * @brief	根据字符个数来生成符合长度的路径.
        *
        * @param	dx	(Optional) the dx.
        *
        * @return	A reference to a path.
        */
        basic_path& compact_path(int dx = 120) {
        
            $windows({
                auto wvec = wbytes();
                std::vector<wchar_t> vec(1000, 0);
                ::PathCompactPathExW(vec.data(), wvec.data(), dx, 0);
                reset_path(vec);
            })
            
            return *this;
        }
        /*!
        * @fn	path& path::compact_path_ex(int dx = 120)
        *
        * @brief	根据字符个数来生成符合长度的路径.
        *
        * @param	dx	(Optional) the dx.
        *
        * @return	A reference to a path.
        */
        basic_path& compact_path_ex(int dx = 120) {
        
            $windows({ auto wvec = wbytes();
                       std::vector<wchar_t> vec(1000, 0);
                       ::PathCompactPathExW(vec.data(), wvec.data(), dx, 0);
                       reset_path(vec);
                     });
            return *this;
        }
        /*!
        * @fn	path& path::undecorate()
        *
        * @brief	去除路径中的修饰——具体还没看明白，MSDN的例子只是去掉了括号.
        *
        * @return	A reference to a path.
        */
        basic_path& undecorate() {
            $windows({
                auto wvec = wbytes();
                ::PathUndecorateA(wvec.data());
                reset_path(wvec);
            })
            return *this;
        }
        /*!
        * @fn	path& path::unexpand_env_strings()
        *
        * @brief	将路径中部分数据替换为系统环境变量格式.
        *
        * @return	A reference to a path.
        */
        basic_path& unexpand_env_strings() {
            $windows({ char a[1000] = { 0 };
                       ::PathUnExpandEnvStringsA(m_path, a, 1000);
                       reset_path(a);
                     })
                     
            return *this;
        }
        
        basic_path parent(bool remove_backslash_ = false) const {
        
            std::wstring wstr = generic_wstring();
            basic_path p(wstr);
            p.canonicalize(false);
            p.remove_backslash();
            string_type str = p.raw_string();
            int index = str.find_last_of('\\');
            
            if (index == -1) {
                index = str.find_last_of('/');
            }
            
            if (index == -1) {
                return basic_path();
            }
            
            if (!remove_backslash_) {
                str = str.substr(0, index + 1);
            } else {
                str = str.substr(0, index);
            }
            
            return basic_path(str);
        }
        
        //////////////////////////////////////////////////////////////////////////
        // 路径查找比较函数
        //////////////////////////////////////////////////////////////////////////
        
        /*!
        * @fn	path& path::find_on_path(const path& other)
        *
        * @brief	Searches for the first path.
        *
        * @param	other	The other.
        *
        * @return	The found path.
        */
        bool find_on_path(const basic_path& other) {
            $windows({
                auto wvec = wbytes();
                auto rvec = other.wbytes();
                BOOL bOk = ::PathFindOnPathW(wvec.data(),
                                             (PZPCWSTR)rvec.data());
                return bOk != FALSE;
            })
            return false;
        }
        
        
        bool find(const basic_path& other) const {
            basic_path p1(*this);
            basic_path p2(other);
            p1.canonicalize(false);
            p2.canonicalize(false);
            
            p1.remove_front_backslash();
            p2.remove_front_backslash();
            
            std::wstring wstr = p1.generic_wstring();
            std::wstring sub_wstr = p2.generic_wstring();
            return wstr.find(sub_wstr) != -1;
            
        }
        /*!
        * @fn	string_type path::find_extension()
        *
        * @brief	Searches for the first extension.
        *
        * @return	The found extension.
        */
        string_type find_extension() {
            return this->extension().c_str();
        }
        
        /*!
        * @fn	path& path::find_file_name()
        *
        * @brief	Searches for the first file name.
        *
        * @return	The found file name.
        */
        string_type find_file_name() {
            return this->filename();
        }
        /*!
        * @fn	string_type path::find_next_component()
        *
        * @brief	Searches for the next component.
        *
        * @return	The found component.
        */
        string_type find_next_component() {
            $windows({
                auto wvec = wbytes();
                std::wstring wstr = (::PathFindNextComponentW(
                                         wvec.data()));
                return amo::string_utils::wide_to_string(wstr, get_coding_type());
            });
            return "";
            
        }
        /*!
        * @fn	bool path::find_suffix_array(std::vector<string_type > suffix)
        *
        * @brief	查找给定的文件名是否有给定的后缀.
        *
        * @param	suffix	The suffix.
        *
        * @return	true if it succeeds, false if it fails.
        */
        bool find_suffix_array(std::vector<string_type> suffix) {
        
            string_type  ext = this->extension().string();
            
            for (std::size_t i = 0; i < suffix.size(); ++i) {
                if (suffix[i] == ext) {
                    return true;
                }
                
            }
            
            return false;
        }
        
        /*!
        * @fn	string_type path::get_args()
        *
        * @brief	获取路径参数.
        *
        * @return	The arguments.
        */
        string_type get_args() const {
            basic_path tmp = *this;
            tmp.remove_args();
            std::string src = tmp.string();
            std::string str = this->string();
            amo::string_utils::replace_first(str, src, "");
            amo::string_utils::trim_left(str);
            amo::string_utils::trim_right(str);
            return str;
        }
        
        /*!
        * @fn	path& path::get_char_type()
        *
        * @brief		获取路径字符类型.
        *
        * @return	The character type.
        */
        basic_path& get_char_type() {
            //::PathGetCharTypeA()
            return *this;
        }
        /*!
        * @fn	path& path::get_drive_number()
        *
        * @brief		根据逻辑盘符返回驱动器序号.
        *
        * @return	The drive number.
        */
        int get_drive_number() const {
            $windows({
                auto wvec = wbytes();
                return ::PathGetDriveNumberW(wvec.data());
            });
            return 0;
        }
        
        std::vector<std::string> split(bool absolute_ = true) const {
        
            basic_path p = *this;
            
            if (absolute_) {
                p.absolute();
            }
            
            p.canonicalize(false);
            
            if (!absolute_) {
                p.remove_front_backslash();
            }
            
            p.remove_backslash();
            p.normalize();
            
            std::string str = p.to_linux_string();
            
            return amo::string_utils::split(str, "/");
        }
        
        /**
        * @fn	path path::common_path(const amo::path& other)
        *
        * @brief	返回当前路径与所给路径相同的部分，必须是从根目录开始相同.
        *
        * @param	other	The other.
        *
        * @return	A path.
        */
        
        basic_path common_path(const basic_path& other, bool absolute_ = true) {
            std::vector<std::string> vec1 = this->split(absolute_);
            std::vector<std::string> vec2 = other.split(absolute_);
            basic_path p;
            size_t len = (std::min)(vec1.size(), vec2.size());
            
            for (size_t i = 0; i < len; ++i) {
                if (vec1[i] == vec2[i]) {
                    p.append(vec1[i]);
                } else {
                    break;
                }
            }
            
            basic_path tmp(*this);
            tmp.absolute();
            std::string str = tmp.normalize().to_linux_string();
            
            if (!str.empty() && str[0] == '/') {
                p.add_front_backslash("/");
            }
            
            return p;
            
        }
        
        /*!
        * @fn	path path::relative_path_to(const path& to)
        * @deprecated
        * @brief	命令有歧义，不要使用该函数
        * 			从to出发到this的路径,创建一个路径到另一个路径的相对路径.
        * 			当前"c:\\a\\b\\path\\file"; 目标to: "c:\\a\\b\\path";
        * 			返回 ".\\file"
        *
        *
        * @param	to	to.
        *
        * @return	A reference to a path.
        */
        basic_path& relative_path_to(const basic_path& to) {
        
            if (is_absolute() != to.is_absolute()) {
                reset_path("");
                return *this;
            }
            
            
            $windows({
                if (get_drive_number() != to.get_drive_number()) {
                    reset_path("");
                    return *this;
                }
            })
            
            std::vector<std::string> vec1 = this->split();
            std::vector<std::string> vec2 = to.split();
            
            
            size_t len = (std::min)(vec1.size(), vec2.size());
            size_t index = 0;
            
            for (size_t i = 0; i < len; ++i) {
                if (vec1[i] == vec2[i]) {
                    ++index;
                } else {
                    break;
                }
            }
            
            basic_path p = ".";
            
            for (size_t i = index; i < vec2.size(); ++i) {
                p.append("..");
            }
            
            for (size_t i = index; i < vec1.size(); ++i) {
                p.append(vec1[i]);
            }
            
            *this = p;
            return *this;
            
        }
        
        /*!
        * @fn	path path::relative_path_to_c(const path& to)
        * @deprecated
        * @brief	创建一个路径到另一个路径的相对路径.
        * 			当前"c:\\a\\b\\path\\file"; 目标to: "c:\\a\\b\\path";
        * 			返回 ".\\file"
        *
        * @param	to	to.
        *
        * @return	A path.
        */
        basic_path relative_path_to_copy(const basic_path& to) const {
            basic_path tmp = *this;
            tmp.relative_path_to(to);
            return tmp;
        }
        
        /**
         * @fn	basic_path sub_path(const basic_path& to)
         * @deprecated
         * @brief	子路径.
         *
         * @param	to	to.
         *
         * @return	A basic_path.
         */
        
        basic_path sub_path(const basic_path& to) {
            return relative_path_from(to);
        }
        
        /**
         * @fn	basic_path sub_path_c(const basic_path& to)
         * @deprecated
         *
         * @brief	子路径.
         *
         * @param	to	to.
         *
         * @return	A basic_path.
         */
        
        basic_path sub_path_copy(const basic_path& to) {
            return relative_path_from_copy(to);
        }
        
        
        /**
        * @fn	basic_path& relative_path_from(const basic_path& from)
        *
        * @brief	从from到当前路径的相对路径.
        *
        * @param	from	Source for the.
        *
        * @return	A reference to a basic_path.
        */
        
        basic_path& relative_path_from(const basic_path& from) {
            return relative_path_to(from);
            
        }
        
        /**
         * @fn	basic_path relative_path_from_copy(const basic_path& from)
         *
         * @brief	从from到当前路径的相对路径.
         *
         * @param	from	Source for the.
         *
         * @return	A basic_path.
         */
        
        basic_path relative_path_from_copy(const basic_path& from) {
            return relative_path_to_copy(from);
            
        }
        
        
        basic_path sub_path_from(const basic_path& to) {
            return relative_path_from(to);
        }
        
        
        // 子路径
        basic_path sub_path_from_copy(const basic_path& to) {
            return relative_path_from_copy(to);
        }
        
        basic_path& replace(const basic_path& from, const basic_path& to) {
            *this = to.append_copy(sub_path_from_copy(from));
        }
        
        basic_path replace_c(const basic_path& from, const basic_path& to) {
            return to.append_copy(sub_path_from_copy(from));
        }
        
        // 去除子路径
        basic_path& trim_right(const basic_path& right) {
            basic_path rhs(right);
            rhs.remove_front_backslash();
            
            std::string strRight = rhs.c_str();
            
            while (true) {
                if (strRight.empty()) {
                    return *this;
                }
                
                if (strRight[0] == '\\' || strRight[0] == '/') {
                    strRight = strRight.substr(1);
                    
                } else {
                    break;
                }
            }
            
            if (strRight.empty()) {
                return *this;
            }
            
            std::string m_str = m_path;
            int nIndex = m_str.find(strRight);
            
            if (nIndex == -1) {
                return *this;
            }
            
            m_str = m_str.substr(0, nIndex);
            *this = basic_path(m_str);
            return *this;
        }
        
        basic_path trim_right_copy(const basic_path& right) {
            basic_path p(*this);
            return p.trim_right(right);
        }
        
        // 去除父路径
        basic_path& trim_left(const basic_path& left) {
            return relative_path_from(left);
        }
        
        basic_path trim_left_copy(const basic_path& left) {
            return relative_path_to_copy(left);
        }
        /*!
        * @fn	path& path::resolve()
        *
        * @brief		将一个相对路径或绝对路径转换为一个合格的路径，这个理解起来比较拗口.
        *
        * @return	A reference to a path.
        */
        basic_path& resolve() {
            //::PathResolve()
            return *this;
        }
        /*!
        * @fn	path& path::canonicalize()
        *
        * @brief		规范化路径。将格式比较乱的路径整理成规范的路径格式.
        *
        * @return	A reference to a path.
        */
        basic_path& canonicalize(bool windows = true) {
            std::wstring str = generic_wstring();
            
            amo::string_utils::replace(str, L"\\", L"/");
            
            if (str.empty()) {
                return *this;
            }
            
            int offset = 0;
            
            while (true) {
                int index = str.find(L"./", offset);
                offset += 2;
                
                if (index == -1) {
                    break;
                    
                }
                
                if (index > 0) {
                    offset = index + 2;
                    
                    if (str[index - 1] != '.') {
                        str[index] = '/';
                    }
                }
                
            }
            
            offset = 0;
            
            while (true) {
                int index = str.find(L".\\", offset);
                offset += 2;
                
                if (index == -1) {
                    break;
                    
                }
                
                if (index > 0) {
                    offset = index + 2;
                    
                    if (str[index - 1] != '.') {
                        str[index] = '\\';
                    }
                }
                
            }
            
            
            // 将多个斜杠变为一个
            std::wregex reg(L"(\\\\{2,})|(/{2,})");
            str = std::regex_replace(str, reg, L"/");
            
            if (windows) {
                amo::string_utils::replace(str, L"/", L"\\");
            } else {
                amo::string_utils::replace(str, L"\\", L"/");
            }
            
            reset_path(str);
            return *this;
        }
        
        /*basic_path& normalize() {
            canonicalize(false);
            return remove_dot_dot();
        }*/
        
        // 移除路径中的".."
        basic_path& remove_dot_dot() {
        
        
            std::regex reg("\\/\\.\\.");
            
            std::smatch m;
            std::string str = raw_string();
            std::string retval;
            
            while (std::regex_search(str, m, reg)) {
            
                std::string mstr = m[0];
                std::string suffix_str = m.suffix().str();
                
                if (!suffix_str.empty() && suffix_str[0] != '/') {
                    str = suffix_str;
                    continue;
                }
                
                int suffix_len = suffix_str.size();
                int body_len = mstr.size();
                int prefix_len = str.size() - suffix_len - body_len;
                std::string prefix_str = str.substr(0, prefix_len);
                
                retval += prefix_str;
                
                if (retval.empty()
                        || retval == "."
                        || retval == "./"
                        || amo::string_utils::end_with(retval, "/..")) {
                    retval += str.substr(prefix_len,  body_len);
                } else {
                
                    int index = retval.find_last_of('/');
                    
                    if (index == -1) {
                        retval += mstr;
                    } else {
                        retval = retval.substr(0, index);
                    }
                }
                
                str = suffix_str;
                
            }
            
            retval += str;
            reset_path(retval);
            return *this;
        }
        
        /*!
        * @fn	path& path::get_short_path_name()
        *
        * @brief	将长路径转为8.3格式的短路径格式.
        *
        * @return	The short path name.
        */
        basic_path& get_short_path_name() {
            $windows({
            
                std::vector<wchar_t> a(1000, 0);
                auto wvec = wbytes();
                BOOL bRet = ::GetShortPathNameW(wvec.data(), a.data(), 1000);
                
                if (bRet != FALSE) {
                    reset_path(a);
                }
            })
            
            return*this;
            
            
        }
        /*!
        * @fn	path& path::get_long_path_name()
        *
        * @brief		将短路径格式转为长路径.
        *
        * @return	The long path name.
        */
        basic_path& get_long_path_name() {
            $windows({
                std::vector<wchar_t> a(1000, 0);
                auto wvec = wbytes();
                BOOL bRet = ::GetLongPathNameW(wvec.data(), a.data(), 1000);
                
                if (bRet != FALSE) {
                    reset_path(a);
                }
            })
            
            
            return*this;
        }
        /*!
        * @fn	path& path::get_short_path()
        *
        * @brief		将长路径转为短路径格式（8.3格式）.
        *
        * @return	The short path.
        */
        basic_path& get_short_path() {
            return get_short_path_name();
        }
        /*!
        * @fn	path& path::make_pretty()
        *
        * @brief	把路径全部转为小写，增加可读性.
        *
        * @return	A reference to a path.
        */
        basic_path& make_pretty() {
            std::string str = this->raw_string();
            amo::string_utils::to_lower(str);
            reset_path(str);
            return *this;
        }
        
        /*path& PathProcessCommand()
        {
        ::PathProcessCommand
        }*/
        /*!
        * @fn	path& cleanup_spec()
        *
        * @brief	Cleanup specifier. 因为 ' \' 被认为是无效的字符，将删除
        *
        * @return	A reference to a path.
        */
        basic_path& cleanup_spec() {
            $windows({
                /*std::wstring ws = generic_wstring();
                std::vector< wchar_t> a = wbytes();
                memcpy(a, ws.c_str(), ws.size());
                ::PathCleanupSpec(NULL, a);
                s = amo::string_utils::wide_to_ansi(ws);
                strcpy(m_path, s.c_str());*/
            })
            
            return *this;
        }
        /*!
        * @fn	path common_prefix(const path& other)
        *
        * @brief	获取两个路径的共同路径.
        *
        * @param	other	The other.
        *
        * @return	A path.
        */
        basic_path common_prefix(const basic_path& other) {
            return common_path(other);
        }
        
        
        /**
        * @fn	static path work_path()
        *
        * @brief	获取工作目录.
        *
        * @return	A path.
        */
        
        static basic_path work_path() {
            $windows({
                wchar_t tszModule[MAX_PATH + 1] = { 0 };
                ::GetCurrentDirectoryW(MAX_PATH, tszModule);
                return basic_path(tszModule);
            })
            $linux({
                char buffer[1000] = { 0 };
                getcwd(buffer, 1000);
                return basic_path(buffer);
            })
            return basic_path();
            
        }
        
        static bool set_work_path_to_app_path() {
            $windows({
                auto wvec = fullAppDir().wbytes();
                BOOL bOk = ::SetCurrentDirectoryW(
                    wvec.data());
                return bOk != FALSE;
            })
            $linux({
                return 0 == chdir(getExeDir().c_str());
            })
            
            return false;
            
        }
        
        
        static bool set_work_path(const std::string& file_or_dir) {
            std::wstring wstr = amo::string_utils::string_to_wide(file_or_dir,
                                CODING_TYPE_ANSI);
            return set_work_path(wstr);
            
        }
        
        static bool set_work_path_u8(const std::string& file_or_dir) {
            std::wstring wstr = amo::string_utils::string_to_wide(file_or_dir,
                                CODING_TYPE_UTF8);
            return set_work_path(wstr);
            
        }
        
        static bool set_work_path(const std::wstring& file_or_dir) {
            basic_path<amo::string_coding_utf8> p(file_or_dir);
            p.absolute();
            
            if (p.is_file()) {
                p = p.parent();
                
            }
            
            $windows({
                auto wvec = p.wbytes();
                BOOL bOk = ::SetCurrentDirectoryW(
                    wvec.data());
                return bOk != FALSE;
            })
            $linux({
                std::string str = p.generic_string();
                return  0 == chdir(str.c_str());
            })
            return false;
            
        }
        
        
        
        /*!
        * @fn	bool is_file_spec()
        *
        * @brief	验证路径是否一个文件名.
        *
        * @return	true if file specifier, false if not.
        */
        bool is_file_spec() const {
            $windows({
                auto wvec = wbytes();
                return ::PathIsFileSpecW(wvec.data()) != FALSE;
            })
            return true;
        }
        /*!
        * @fn	bool is_exe()
        *
        * @brief	验证路径是否是可执行文件。注意:不仅仅是.exe，还有.bat，.com，.src等.
        *
        * @return	true if executable, false if not.
        */
        bool is_exe() const {
            //return PathIsExe()
            return false;
        }
        /*!
        * @fn	bool is_root()
        *
        * @brief		路径是否为根路径.
        *
        * @return	true if root, false if not.
        */
        bool is_root() const {
            if (!this->has_root_path()) {
                return false;
            }
            
            return (this->root_path().string() == this->string());
            //return PathIsRootA(m_path) != FALSE;
        }
        ///*!
        // * @fn	bool is_relative()
        // *
        // * @brief		判断路径是否是相对路径.
        // *
        // * @return	true if relative, false if not.
        // */
        //bool is_relative() const {
        //    return PathIsRelativeA(m_path) != FALSE;
        //}
        /*!
        * @fn	bool is_prefix(const path& prefix)
        *
        * @brief		判断路径是否含有指定前缀.
        *
        * @param	prefix	The prefix.
        *
        * @return	true if prefix, false if not.
        */
        bool is_prefix(const basic_path& prefix) const {
            $windows({
                auto rvec = prefix.wbytes();
                auto wvec = wbytes();
                return ::PathIsPrefixA(rvec.data(),
                                       wvec.data()) == NULL;
            })
            return false;
        }
        /*!
        * @fn	bool is_same_root(const path& other)
        *
        * @brief		判断路径是否有相同根目录.
        *
        * @param	other	The other.
        *
        * @return	true if same root, false if not.
        */
        bool is_same_root(const basic_path& other) {
            $windows({ return (get_drive_number() == other.get_drive_number()); })
            
            return true;
            
        }
        /*!
        * @fn	bool transfer(amo::function<void(path&)> fn_cb, bool recursion = false)
        *
        * @brief	遍历文件夹.
        *
        * @param [in,out]	fn_cb	The cb. 文件回调函数
        * @param	recursion	 	(Optional) true to recursion. true递归遍历， false 只遍历当前文件夹.
        *
        * @return	true if it succeeds, false if it fails.
        */
        bool transfer(amo::function<void(basic_path&)> fn_cb,
                      bool recursion = false) {
                      
                      
            if (!this->is_directory()) {
                return false;
            }
            
            
            auto dir = filesystem_path();
            boost::filesystem::directory_iterator end;
            boost::filesystem::directory_iterator begin(dir);
            
            for (auto iter = begin; iter != end; ++iter) {
                basic_path  p((*iter));
                
                fn_cb(p);
                
                if (p.is_directory() && recursion) {
                    bool bOk = p.transfer(fn_cb, recursion);
                    
                    if (!bOk) {
                        return bOk;
                    }
                }
                
                
            }
            
            return true;
            
        }
        
        /*!
        * @fn	bool file_exists()
        *
        * @brief		验证路径是否存在.
        *
        * @return	true if it succeeds, false if it fails.
        */
        bool file_exists() const {
            return	amo::filesystem::exists(filesystem_path());
            //return ::PathFileExistsA(m_path) != FALSE;
        }
        
        /**
        * @fn	bool is_valid() const
        *
        * @brief	判断路径是否合法（现在只有非法字符）.
        *
        * @return	true if valid, false if not.
        */
        
        bool is_valid() const {
            string_type p(m_path);
            /*  if (p.find('\\') != -1) {
            return false;
            }
            
            if (p.find('/') != -1) {
            return false;
            }
            */
            /*  if (p.find(':') != -1) {
            return false;
            }
            
            if (p.find('?') != -1) {
            return false;
            }
            
            if (p.find('\"') != -1) {
            return false;
            }
            
            if (p.find('<') != -1) {
            return false;
            }
            
            if (p.find('>') != -1) {
            return false;
            }
            
            if (p.find('|') != -1) {
            return false;
            }*/
            return true;
        }
        
        
        static basic_path fullAppDir() {
            return basic_path(getExeDir());
        }
        
        
        static string_type getExeDir() {
        
            $windows({
                wchar_t executionDir[MAX_PATH];
                
                if (::GetModuleFileNameW(NULL, executionDir,
                                         sizeof executionDir) == NULL) {
                    executionDir[0] = 0;
                }
                
                auto retval = amo::string_utils::wide_to_string(executionDir, get_coding_type());
                int index = retval.find_last_of('\\');
                
                if (index >= 0) {
                    return retval.substr(0, index);
                }
                return retval;
            })
            $welse({
                std::string fullpath =
                boost::filesystem::initial_path<boost::filesystem::path>().string();
                return fullpath;
            })
            
        }
        
        static basic_path appName() {
            return basic_path(getExeName());
        }
        
        
        static string_type getExeName() {
            $windows({
                wchar_t executionDir[MAX_PATH];
                
                if (::GetModuleFileNameW(NULL, executionDir,
                                         sizeof executionDir) == NULL) {
                    executionDir[0] = 0;
                }
                
                auto retval = amo::string_utils::wide_to_string(executionDir, get_coding_type());
                int index = retval.find_last_of('\\');
                
                if (index >= 0) {
                    return retval.substr(index + 1);
                }
                return "";
            })
            $linux({
                basic_path p = fullAppName();
                basic_path parent = fullAppDir();
                p.canonicalize(false);
                parent.canonicalize(false);
                p.relative_path_to(parent);
                return p.c_str();
            })
            
            return "";
            
        }
        
        static basic_path fullAppName() {
            return basic_path(getFullExeName());
        }
        
        
        static string_type getFullExeName() {
            $windows({
            
                wchar_t executionDir[MAX_PATH];
                
                if (::GetModuleFileNameW(NULL, executionDir,
                                         sizeof executionDir) == NULL) {
                    executionDir[0] = 0;
                }
                
                auto retval = amo::string_utils::wide_to_string(executionDir, get_coding_type());
                return retval;
            })
            $linux({
                const int MAXBUFSIZE = 1000;
                char buf[MAXBUFSIZE] = { 0 };
                int count = 0;
                
                count = readlink("/proc/self/exe", buf, MAXBUFSIZE);
                
                if (count < 0 || count >= MAXBUFSIZE) {
                    return "";
                }
                
                buf[count] = '\0';
                return string_type(buf);   // utf8
            })
            return "";
        }
        
        static basic_path fullPathInAppDir(const basic_path&
                                           filePath) {
            return getFullPathInExeDir(filePath.c_str(), false);
        }
        
        /*  static path fullPathInAppDir(const string_type & filePath) {
        return getFullPathInExeDir(filePath);
        }*/
        
        static string_type getFullPathInExeDir(
            const string_type & fileName,
            bool includingSlash = true) {
            
            basic_path p = fullAppDir();
            
            p.append(fileName);
            
            if (includingSlash) {
                p.add_backslash();
            }
            
            return p.string();
            
            /* $windows({
            
            string_type file_name = fileName;
            char buf[MAX_PATH];
            ZeroMemory(buf, MAX_PATH);
            
            if (::GetModuleFileNameA(NULL, buf, MAX_PATH)) {
            ::PathRemoveFileSpecA(buf);
            
            if (includingSlash) {
            strcat_s(buf, _countof(buf), "\\");
            }
            
            if (!file_name.empty()) {
            strcat_s(buf, _countof(buf), file_name.c_str());
            }
            }
            
            return string_type (buf);
            })
            $welse({
            amo::path p = getExeDir();
            p.append(fileName);
            
            if (includingSlash)
            p.add_backslash();
            return p.string();
            })*/
            
        }
        
        /*!
        * @fn	path PathYetAnotherMakeUniqueName()
        *
        * @brief	基于已存在的文件，自动创建一个唯一的文件名。
        * 比较有用，比如存在“新建文件”，此函数会创建文件名“新建文件（2）
        * Path yet another make unique name.
        *
        * @return	A path.
        */
        /*path PathYetAnotherMakeUniqueName()
        {
        ::PathYetAnotherMakeUniqueName()
        }*/
        
        
        
        
        // 新接口
        
        
        
        
        
        amo::filesystem::path filesystem_path() const {
            std::string str = m_path;
            $windows({
                amo::string_utils::replace(str, "/", "\\");
            })
            $linux({
                amo::string_utils::replace(str, "\\", "/");
            })
            
            return  amo::filesystem::path(
                        amo::string_utils::string_to_wide(str, get_coding_type()));
            /*return  amo::filesystem::path(
            amo::string(m_path).to_string<amo::filesystem::path::string_type>());*/
        }
        
        void filesystem_path(const amo::filesystem::path&
                             p) {
                             
            std::wstring wstr = p.generic_wstring();
            
            *this = amo::string_utils::wide_to_string(wstr, get_coding_type());
            return;
        }
        
        operator amo::filesystem::path() const {
            return filesystem_path();
        }
        
        operator const std::string()const {
            return c_str();
        }
        
        
        char* c_str() {
            $windows({ canonicalize(true); })
            $welse({ canonicalize(false); })
            
            return m_path;
        }
        
        const std::string  generic_ansi_string() const {
            return filesystem_path().generic_string();
        }
        const std::string  generic_string() const {
            std::wstring wstr = filesystem_path().generic_wstring();
            return amo::string_utils::wide_to_string(wstr, get_coding_type());
        }
        const std::string  generic_u8string() const {
            std::wstring wstr =  filesystem_path().generic_wstring();
            return amo::string_utils::wide_to_utf8(wstr);
        }
        const std::wstring  generic_wstring() const {
            return filesystem_path().generic_wstring();
        }
        
        
        const char* c_str() const {
            return m_path;
        }
        
        const std::string to_linux_string() const {
            basic_path p = *this;
            p.canonicalize(false);
            return p.m_path;
        }
        
        const std::string to_windows_string() const {
            basic_path p = *this;
            p.canonicalize(true);
            return p.m_path;
        }
        
        const  std::string to_string() const {
            return m_path;
        }
        
        const std::string raw_string() const {
            return m_path;
        }
        
        std::wstring raw_wstring() const {
            return amo::string_utils::string_to_wide(m_path, get_coding_type());
        }
        
        /*const basic_string<CodingType> generic_raw_string() const {
        	return basic_string<CodingType>(m_path, is_utf8());
        }
        */
        /**
        * @fn	std::string string() const
        *
        * @brief	返回用于初始化 path 的字符串的副本，其格式符合 path 语法规则。.
        *
        * @return	A std::string.
        */
        
        std::string string() const {
            std::wstring wstr =  filesystem_path().generic_wstring();
            return amo::string_utils::wide_to_string(wstr, get_coding_type());
        }
        
        
        
        //  -----  decomposition  -----
        
        basic_path  root_path() const {
            return filesystem_path().root_path();
        }
        
        /**
        * @fn	path root_name() const
        *
        * @brief	在给定从文件系统根目录开始的路径的情况下，
        * 			此例程将返回包含 PATHNAME 的第一个字符的字符串。.
        *
        * @return	A path.
        */
        
        basic_path  root_name()
        const {
            return filesystem_path().root_name();
        }
        
        /**
        * @fn	amo::path root_directory()
        *
        * @brief	在提供了路径的情况下，此 API 将返回根目录，否则将返回空字符串。
        * 			例如，如果路径包含 /tmp/var1，则此例程将返回 /，即 UNIX 文件系统的根。
        * 			不过，如果路径是相对路径，如 ../mywork/bin，此例程将返回空字符串。.
        *
        * @return	An amo::path.
        */
        
        basic_path root_directory() {
            return filesystem_path().root_directory();
        }
        
        basic_path  relative_path() const {
            return filesystem_path().relative_path();
        }
        basic_path  parent_path() const {
            return filesystem_path().parent_path();
        }
        
        basic_path  filename() const {
            return filesystem_path().filename().generic_wstring();
        }
        basic_path  stem() const {
            return filesystem_path().stem();
        }
        basic_path  extension() const {
            return filesystem_path().extension();
        }
        
        //  -----  query  -----
        
        
        
        //  -----  lexical operations  -----
        
        basic_path  lexically_normal() const {
            return filesystem_path().lexically_normal();
        }
        basic_path  lexically_relative(const basic_path& base) const {
            return filesystem_path().lexically_relative(
                       base.filesystem_path());
        }
        basic_path  lexically_proximate(const basic_path& base)
        const {
            return filesystem_path().lexically_proximate(
                       base.filesystem_path());
        }
        
        
        basic_path&  normalize() {
            amo::filesystem::path p = filesystem_path();
            p.normalize();
            filesystem_path(p);
            
            return *this;
        }
        basic_path&  remove_leaf() {
            amo::filesystem::path p = filesystem_path();
            p.remove_leaf();
            filesystem_path(p);
            return *this;
        }
        basic_path   leaf() const {
            return filesystem_path().leaf();
        }
        basic_path   branch_path() const {
            return filesystem_path().branch_path();
        }
        basic_path   generic() const {
            return filesystem_path().generic();
        }
        
        bool create_file() const {
            if (empty()) {
                return false;
            }
            
            if (file_exists()) {
                return true;
            }
            
            basic_path tmp(*this);
            tmp.absolute();
            tmp.normalize();
            
            basic_path p = tmp.parent_path();
            p.create_directory();
            
            
#ifdef OS_WIN
            std::wofstream ofs(tmp.generic_wstring());
            
            if (ofs.is_open()) {
                ofs.close();
                return true;
            }
            
#else
            std::wstring wstr = tmp.generic_wstring();
            
            std::ofstream ofs(amo::string_utils::wide_to_ansi(wstr));
            
            if (ofs.is_open()) {
                ofs.close();
                return true;
            }
            
#endif // OS_WIN
            
            
            return false;
        }
        
        /**
        * @fn	bool create_directory() const
        *
        * @brief	创建文件夹.
        *
        * @return	true if it succeeds, false if it fails.
        */
        
        bool create_directory() const {
            try {
                if (this->is_directory() && this->exists()) {
                    return true;
                }
                
                if (this->empty()) {
                    return true;
                }
                
                //if (this->is_root()) {
                //    return true;
                //}
                
                return amo::filesystem::create_directories(
                           filesystem_path());
                           
            } catch (const std::exception&) {
                return false;
            }
            
        }
        
        size_t capacity_size() const {
            return amo::filesystem::space(filesystem_path()).capacity;
        }
        size_t free_size() const {
            return amo::filesystem::space(filesystem_path()).free;
        }
        size_t available_size() const {
            return amo::filesystem::space(filesystem_path()).available;
        }
        /**
        * @fn	bool exists() const
        *
        * @brief	此函数检查文件的扩展名.
        * 			文件可以为任何类型:常规文件、目录、符号链接等等.
        *
        * @return	true if it succeeds, false if it fails.
        */
        
        bool exists() const {
            return amo::filesystem::exists(filesystem_path());
        }
        
        /*!
        * @fn	bool is_directory()
        *
        * @brief 判断路径是否是一个有效的目录.
        *
        * @return	true if directory, false if not.
        */
        bool is_directory() const {
            return	amo::filesystem::is_directory(
                        filesystem_path());
        }
        
        /**
        * @fn	bool is_regular() const
        *
        * @brief	判断是否为普通文件
        * 			(即此文件不是目录、符号链接、套接字或设备文件).
        *
        * @return	true if regular, false if not.
        */
        
        bool is_regular() const {
            return	amo::filesystem::is_regular_file(
                        filesystem_path());
        }
        
        /**
        * @fn	bool is_file() const
        *
        * @brief	判断是否为普通文件.
        *
        * @return	true if file, false if not.
        */
        
        bool is_file() const {
            return	is_regular();
        }
        
        /**
        * @fn	bool is_other() const
        *
        * @brief	通常，此函数检查设备文件（如 /dev/tty0）或套接字文件.
        *
        * @return	true if other, false if not.
        */
        
        bool is_other() const {
            return	amo::filesystem::is_other(
                        filesystem_path());
        }
        
        /**
        * @fn	bool equal_to(const amo::path& other) const
        *
        * @brief	两个路径是否等效, 可以判断相对路径 和 绝对路径.
        *
        * @param	other	The other.
        *
        * @return	true if it succeeds, false if it fails.
        */
        
        bool equal_to(const basic_path& other) const {
            return amo::filesystem::equivalent(
                       filesystem_path(),
                       other.filesystem_path());
        }
        
        /**
        * @fn	bool is_empty() const
        *
        * @brief	如果路径与文件夹对应，此函数将检查文件夹是否为空，
        * 			并据此返回“True”或“False”。如果路径与文件对应，
        * 			此函数将检查文件的大小是否等于 0。
        * 			对于文件的硬链接或符号链接的情况，
        * 			此 API 将检查原始文件是否为空。.
        *
        * @return	true if empty, false if not.
        */
        
        bool is_empty() const {
            return	amo::filesystem::is_empty(
                        filesystem_path());
        }
        
        /**
        * @fn	amo::path to_absolute()
        *
        * @brief	此函数是与 bool equivalent(const path1&amp; p1, const path2&amp; p2) 同一系列的另一个 API。
        * 			给定当前工作目录中任意文件路径的情况下，此 API 将返回该文件的绝对路径。 例如，如果用户位于目录 /home/user1 并查询文件 ../user2/file2，
        * 			此函数将返回 /home/user2/file2，即文件 file2 的完整路径名.
        *
        * @param	p	The amo::path to process.
        *
        * @return	An amo::path.
        */
        
        basic_path& to_absolute() {
            basic_path p(amo::filesystem::system_complete(
                             filesystem_path()));
            *this = p;
            return *this;
        }
        
        basic_path to_absolute_c() const {
            basic_path p(amo::filesystem::system_complete(
                             filesystem_path()));
            return p;
        }
        
        basic_path& absolute() {
            return to_absolute();
        }
        
        ///**
        // * @fn	std::string extension() const
        // *
        // * @brief	此函数以前面带句点 (.) 的形式返回给定文件名的扩展名。
        // * 			例如，对于文件名为 test.cpp 的文件，extension 将返回 .cpp。
        // * 			对于文件没有扩展名的情况，此函数将返回空字符串。
        // * 			对于隐藏文件（即 UNIX 系统中文件名以 . 开始的文件），
        // * 			此函数将相应地计算扩展名类型或返回空字符串
        // * 			（因此，对于 .test.profile，此例程将返回 .profile）。
        // *
        // * @return	A std::string.
        // */
        //
        //std::string extension() const {
        //    return string_type (amo::filesystem::extension(
        //                           filesystem_path()).c_str()).str();
        //}
        
        /**
        * @fn	std::string basename() const
        *
        * @brief	这是与 extension 互补的例程。它将返回文件名中 . 之前的字符串。
        * 			请注意，即使提供了绝对文件名，此 API 仍然仅会返回属于文件名的直接部分.
        *
        * @return	A std::string.
        */
        
        std::string basename() const {
            std::wstring wstr = this->stem().generic_wstring();
            return amo::string_utils::wide_to_string(wstr, get_coding_type());
            //return amo::filesystem::basename(filesystem_path());
        }
        
        /**
        * @fn	std::string change_extension(const std::string& new_extension)
        *
        * @brief	此 API 将返回反映更改后的名称的新字符串。
        * 			请注意，与 oldpath 对应的文件保持不变。
        * 			这只是一个常规函数。另请注意，您必须显式地在扩展名中指定点。
        * 			例如，change_extension("test.c", "so") 会得到 testso，而不是 test.so。.
        *
        * @param	new_extension	The new extension.
        *
        * @return	A std::string.
        */
        
        basic_path& change_extension(const std::string&
                                     new_extension) {
            amo::filesystem::path oldpath(filesystem_path());
            amo::filesystem::path p = basic_path(
                                          new_extension).filesystem_path();
            this->filesystem_path(
                amo::filesystem::change_extension(oldpath, p));
            return *this;
        }
        
        /*!
        * @fn	path& path::remove_extension()
        *
        * @brief	去除文件路径扩展名.
        *
        * @return	A reference to a path.
        */
        basic_path& remove_extension() {
            return change_extension("");
        }
        
        /*!
        * @fn	path& path::rename_extension(const string_type & ext);
        *
        * @brief	更改文件路径扩展名.
        *
        * @param	ext	The extent.
        *
        * @return	A reference to a path.
        */
        basic_path& rename_extension(const std::string& ext) {
            return change_extension(ext);
        }
        
        
        
        /*!
        * @fn	bool copy_to(const amo::path& to)
        *
        * @brief	复制文件或目录
        *
        * @param	to	to.
        *
        * @return	true if it succeeds, false if it fails.
        */
        bool copy_to(const basic_path& to) {
            //to.branch_path().create_directory();
            
            try {
            
                if (this->is_file()) {
                    basic_path p = to.parent();
                    
                    if (!p.is_root()) {
                        p.create_directory();
                    }
                    
                    amo::filesystem::copy_file(filesystem_path(),
                                               to.filesystem_path(),
                                               amo::filesystem::copy_option::overwrite_if_exists);
                    return true;
                } else {
                    copy_files(filesystem_path(), to.filesystem_path());
                    return true;
                }
                
                
            } catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
                return false;
            }
        }
        
        /* bool move_to(const amo::path& to) {
        try {
        amo::filesystem::rename(filesystem_path(), to.filesystem_path());
        return true;
        } catch (const std::exception& e) {
        e.what();
        return false;
        }
        }*/
        
        bool move_to(const basic_path& to) {
        
            if (!this->is_file()) {
                to.parent().create_directory();
            }
            
            if (!rename(to)) {
                bool ret = this->copy_to(to);
                
                if (!ret) {
                    return false;
                }
                
                return  remove_all();
            } else {
                return true;
            }
            
#ifdef OS_WIN
            
            basic_path p1 = *this;
            basic_path p2 = to;
            p1.absolute().normalize();
            p2.absolute().normalize();
            
            
            auto wvec = p1.wbytes();
            auto wrhs = p2.wbytes();
            
            if (this->is_directory()) {
                to.create_directory();
                SHFILEOPSTRUCTW FileOp = { 0 };
                FileOp.hwnd = HWND_DESKTOP;
                FileOp.fFlags = FOF_NO_UI |   //不出现确认对话框
                                FOF_SILENT | // 不显示进度
                                FOF_NOCONFIRMMKDIR; //需要时直接创建一个文件夹,不需用户确定
                FileOp.pFrom = wvec.data();
                FileOp.pTo = wrhs.data();
                FileOp.wFunc = FO_MOVE;
                return SHFileOperationW(&FileOp) == 0;
            } else if (this->is_file()) {
                basic_path(to).parent().create_directory();
                return ::MoveFileW(wvec.data(), wrhs.data()) != FALSE;
            }
            
            
#else
            basic_path p1 = *this;
            basic_path p2 = to;
            p1.absolute().normalize();
            p2.absolute().normalize();
            
            std::string src = p1.generic_string();
            std::string dst = p2.generic_string();
            std::string cmd = "mv -f ";
            cmd += src;
            cmd += " ";
            cmd += dst;
            std::cout << cmd << std::endl;
            system(cmd.c_str());
            //system("mv ./aaa/bbb/ ./ccc/");
#endif
            
            return true;
            
        }
        
        
        bool rename(const basic_path& to) {
            boost::system::error_code ec;
            boost::filesystem::rename(this->filesystem_path(), to.filesystem_path(), ec);
            return !ec;
        }
        
        /**
        * @fn	bool remove()
        *
        * @brief	移除一个文件或空目录.
        *
        * @return	true if it succeeds, false if it fails.
        */
        
        bool remove() {
            try {
                if (!exists()) {
                    return true;
                }
                
                return   amo::filesystem::remove(filesystem_path());
                
            } catch (const std::exception& e) {
                std::string ss = e.what();
                return false;
            }
        }
        
        /**
        * @fn	bool remove_all()
        *
        * @brief	移除一个文件或递归地移除一个目录及其所有内容.
        *
        * @return	true if it succeeds, false if it fails.
        */
        
        bool remove_all() {
            try {
                amo::filesystem::remove_all(filesystem_path());
                return true;
            } catch (const std::exception& e) {
                std::string ss = e.what();
                return false;
            }
        }
        
        std::vector<wchar_t> wbytes()const {
            std::wstring wstr = generic_wstring();
            std::vector<wchar_t> vec(1000, 0);
            wcscpy((wchar_t*)vec.data(), wstr.c_str());
            return vec;
        }
        
        std::vector<char> bytes()const {
            std::string str = generic_string();
            std::vector<char> vec(1000, 0);
            strcpy((char*)vec.data(), str.c_str());
            return vec;
        }
    private:
        void copy_files(const amo::filesystem::path &src,
                        const amo::filesystem::path &dst) {
                        
            if (!amo::filesystem::exists(dst)) {
                amo::filesystem::create_directories(dst);
            }
            
            for (amo::filesystem::directory_iterator it(src);
                    it != amo::filesystem::directory_iterator(); ++it) {
                const amo::filesystem::path newSrc = it->path();
                const amo::filesystem::path newDst = dst /
                                                     it->path().filename().generic_wstring();
                                                     
                if (amo::filesystem::is_directory(newSrc)) {
                    copy_files(newSrc, newDst);
                } else if (amo::filesystem::is_regular_file(newSrc)) {
                    amo::filesystem::copy_file(newSrc, newDst,
                                               amo::filesystem::copy_option::overwrite_if_exists);
                } else {
                    std::cerr << "Error: unrecognized file - " << newSrc.string() << std::endl;
                }
            }
        }
        
        
    private:
        //string_type m_path;
        /** @brief	The path[ 1000]. */
        char m_path[1000];
        
    };
    
}



#endif //AMO_BASIC_PATH_D025EE85_6284_4EB1_AB33_6A15CF1692B2_HPP__