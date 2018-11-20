// Created by amoylel on 29/09/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_BASIC_STRING_52E89957_B4A1_48AE_85D7_438BFA8B69C9_HPP__
#define AMO_BASIC_STRING_52E89957_B4A1_48AE_85D7_438BFA8B69C9_HPP__


#include <string>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <vector>

#include <amo/config.hpp>
#include <amo/stdint.hpp>
#include <amo/lexical_cast.hpp>
#include <amo/string/string_utils.hpp>
#include <amo/string/tiny_string.hpp>
#include <amo/string/coding_type.hpp>
#include <amo/impl/basic_json.hpp>

#ifdef	__linux__
#include <cstdarg>
#endif
#ifdef __GNUC__
#define vsprintf_s vsnprintf
#endif
#include <regex>
#include <stack>
#include <amo/binary.hpp>





#ifndef OS_WIN
#define DEFAULT_STRING_UTF8 true
#else
#define DEFAULT_STRING_UTF8 false
#endif

// 以下只对中文件系统有效
// 编码
// 1. ANSI 编码
// 一个文本文件ANSI格式的，就表示他的语言编码与操作系统的一致。
// 在简体中文操作系统中就是 GBK 的
// 在繁体中文操作系统中就是 Big5 的

// 2. WIDE
// std::wstring 在linux和windows(gcc msvc) 编译出来都是一样的

// 字面常量
// 1.msvc 不管源代码文件是什么格式保存都会将常量字符串转换为GB2312， 只测了windows平台，
// 如果以utf-8 编码保存还是会编码为GB3212
// 2.gcc 会将常量字符串转换为对应格式，只测了linux平台
// 如果以utf-8 编码保存会编码为utf8
// 宽字符不受影响 都是GBK编码（ANSI）

// linux 使用该类需要设置本地化信息  setlocale(LC_ALL, "zh_CN");

// 参考
// http://blog.csdn.net/gong_xucheng/article/details/25950607
// http://bbs.csdn.net/topics/370102948
// http://bbs.csdn.net/topics/110110206
// http://blog.chinaunix.net/uid-731376-id-7966.html
// http://bjoern.hoehrmann.de/utf-8/decoder/dfa/
// http://stackoverflow.com/questions/1031645/how-to-detect-utf-8-in-plain-c


namespace amo {
    //字符串转换类，用于各类字符集之间的转换，目前功能不完整
    
    template<typename CodingType>
    class basic_string : public tiny_string {
    
    public:
        static bool is_utf8_constant() {
            static int b_is_utf8 = -1;
            
            if (b_is_utf8 == -1) {
                std::string str = "简单字面常量编码测试";
                bool b_utf8 = amo::string_utils::is_utf8(str);
                b_is_utf8 = b_utf8 ? 1 : 0;
            }
            
            return b_is_utf8 == 1;
        }
        
        static  StringCodingType get_coding_type()   {
            return CodingType::coding_type;
        }
        
        static bool is_utf8()   {
            return CodingType::coding_type == StringCodingType::CODING_TYPE_UTF8;
        }
        
    public:
        basic_string() {
        
        }
        
        basic_string(const basic_string& str) {
            m_str = str.m_str;
            
        }
        
        basic_string(const std::string& str, bool is_utf8/* = DEFAULT_STRING_UTF8*/) {
            if (is_utf8) {
                m_str = amo::string_utils::utf8_to_string(str, get_coding_type());
            } else {
                m_str = amo::string_utils::ansi_to_string(str, get_coding_type());
            }
            
        }
        
        basic_string(const char* str, bool is_utf8 /*= DEFAULT_STRING_UTF8*/) {
            if (strlen(str) == 0) {
                return;
            }
            
            if (is_utf8) {
                m_str = amo::string_utils::utf8_to_string(str, get_coding_type());
            } else {
                m_str = amo::string_utils::ansi_to_string(str, get_coding_type());
            }
        }
        
        /*  basic_string(const std::string& str) {
              m_str = str;
        
          }
        
          basic_string(const char* str) {
              m_str = str;
          }*/
        
        basic_string(const std::wstring& str) {
            m_str = amo::string_utils::wide_to_string(str, get_coding_type());
        }
        
        
        basic_string(const wchar_t* str) {
            m_str = amo::string_utils::wide_to_string(str, get_coding_type());
        }
        
        /* basic_string& operator=(const std::string& str) const {
             m_str = str;
             return *this;
         }*/
        /*  basic_string& operator=(const char* str) const {
              m_str += str;
              return *this;
          }
          */
        
        static basic_string from_constant(const std::string& str) {
            return basic_string(str, is_utf8_constant());
        }
        
        static basic_string from_constant(const char* str) {
            return basic_string(str, is_utf8_constant());
        }
        static basic_string from_utf8(const std::string& str) {
            return basic_string(str, true);
        }
        
        static basic_string from_utf8(const char* str) {
            return basic_string(str, true);
        }
        
        static basic_string from_unicode(const std::wstring& str) {
            return basic_string(str);
        }
        
        static basic_string from_unicode(const wchar_t* str) {
            return basic_string(str);
        }
        
        static basic_string from_ansi(const std::string& str) {
            return basic_string(str, false);
        }
        
        static basic_string from_ansi(const char* str) {
            return basic_string(str, false);
        }
        
        std::string to_ansi() {
            return  amo::string_utils::string_to_ansi(m_str, get_coding_type());
        }
        
        std::string to_ansi() const {
            return  amo::string_utils::string_to_ansi(m_str, get_coding_type());
        }
        
        std::wstring to_wide() {
            return this->operator std::wstring();
        }
        
        std::wstring to_wide() const {
            return this->operator std::wstring();
        }
        
        std::string to_utf8() {
            return  amo::string_utils::string_to_utf8(m_str, get_coding_type());
        }
        
        std::string to_utf8() const {
            return amo::string_utils::string_to_utf8(m_str, get_coding_type());
        }
        
        std::string to_locale() {
            if (DEFAULT_STRING_UTF8) {
                return to_utf8();
            } else {
                return to_ansi();
            }
        }
        
        std::string to_locale() const {
            if (DEFAULT_STRING_UTF8) {
                return to_utf8();
            } else {
                return to_ansi();
            }
        }
        
        
#ifdef UNICODE
        std::wstring to_unicode() {
            return this->operator std::wstring();
        }
        
        std::wstring to_unicode() const {
            return this->operator std::wstring();
        }
#else
        std::string to_unicode() {
            return this->operator std::string();
        }
        
        std::string to_unicode() const {
            return this->operator std::string();
        }
#endif
        
        
        //#ifdef OS_WIN
        //        template<typename T>
        //        T to_string() const {
        //            return this->operator T();
        //        }
        //#endif // OS_WIN
        
        template<typename T>
        T to_string(T = T()) const {
            return this->operator T();
        }
        
        
        operator std::wstring() {
            return amo::string_utils::string_to_wide(m_str, get_coding_type());
        }
        
        operator std::wstring() const {
            return amo::string_utils::string_to_wide(m_str, get_coding_type());
        }
        
        basic_string& operator+=(const char* str) {
            m_str += str;
            return *this;
        }
        
        basic_string& operator+=(const std::string& str) {
            m_str += str;
            return *this;
        }
        
        
        basic_string& operator+=(const basic_string& str) {
            m_str += str.m_str;
            return *this;
        }
        
        
        basic_string& operator+=(const std::wstring& str) {
            m_str += amo::string_utils::wide_to_string(str, get_coding_type());
            return *this;
        }
        
        basic_string& operator+=(const wchar_t* str) {
            m_str += amo::string_utils::wide_to_string(str, get_coding_type());
            return *this;
        }
        
        
        
        basic_string operator+(const std::string& str) {
            return basic_string(m_str + str, is_utf8());
        }
        
        basic_string operator+(const char* str) {
            return basic_string(m_str + str, is_utf8());
        }
        
        basic_string operator+(const basic_string& str) {
            return basic_string(m_str + str.m_str, is_utf8());
        }
        
        
        basic_string operator+(const std::wstring& str) {
            return basic_string(m_str + amo::string_utils::wide_to_string(str,
                                get_coding_type()),
                                is_utf8());
        }
        
        basic_string operator+(const wchar_t* str) {
            return basic_string(m_str + amo::string_utils::wide_to_string(str,
                                get_coding_type()),
                                is_utf8());
        }
        
        
        bool operator!=(const std::string& str) {
            return m_str != str;
        }
        
        bool operator!=(const char* str) {
            return m_str != str;
        }
        
        bool operator!=(const basic_string& str) {
            return m_str != str.m_str;
        }
        
        
        bool operator!=(const std::wstring& str) {
            std::string tmp = amo::string_utils::wide_to_string(str, get_coding_type());
            return m_str != tmp;
        }
        
        bool operator!=(const wchar_t* str) {
            std::string tmp = amo::string_utils::wide_to_string(str, get_coding_type());
            return m_str != tmp;
        }
        
        
        
        
        bool operator==(const std::string& str) {
            return m_str == str;
        }
        
        bool operator==(const char* str) {
            return m_str == str;
        }
        
        bool operator==(const basic_string& str) {
            return m_str == str.m_str;
        }
        
        
        bool operator==(const std::wstring& str) {
            std::string tmp = amo::string_utils::wide_to_string(str, get_coding_type());
            return m_str == tmp;
        }
        
        bool operator==(const wchar_t* str) {
            std::string tmp = amo::string_utils::wide_to_string(str, get_coding_type());
            return m_str == tmp;
        }
        
        
        
        bool operator==(const std::string& str) const {
            return m_str == str;
        }
        
        bool operator==(const char* str) const {
            return m_str == str;
        }
        
        bool operator==(const basic_string& str) const {
            return m_str == str.m_str;
        }
        
        
        bool operator==(const std::wstring& str) const {
            std::string tmp = amo::string_utils::wide_to_string(str, get_coding_type());
            return m_str == tmp;
        }
        
        bool operator==(const wchar_t* str) const {
            std::string tmp = amo::string_utils::wide_to_string(str, get_coding_type());
            return m_str == tmp;
        }
        
        
        basic_string& operator+=(char _Ch) {
            m_str.append((size_type)1, _Ch);
            return *this;
        }
        
        
        basic_string& operator+=(wchar_t _Ch) {
            std::wstring ws = L"";
            ws += _Ch;
            this->operator+=(ws);
            return *this;
        }
        //friend std::ostream& operator<<(std::ostream&, const basic_string&);
        /* std::ostream& operator<<(std::ostream&output) const {
             output << m_str;
             return output;
         }*/
        
        basic_string& append(const _Myt& _Right) {
            m_str.append(_Right);
            return (*this);
        }
        
        basic_string& append(const _Myt& _Right, size_type _Roff, size_type _Count) {
            m_str.append(_Right, _Roff, _Count);
            return (*this);
        }
        
        basic_string& append(const _Elem *_Ptr, size_type _Count) {
            m_str.append(_Ptr, _Count);
            return (*this);
        }
        
        basic_string& append(const _Elem *_Ptr) {
            m_str.append(_Ptr);
            return (*this);
        }
        
        basic_string& append(size_type _Count, _Elem _Ch) {
            m_str.append(_Count, _Ch);
            return (*this);
        }
        
        
        
        basic_string& append(const_pointer _First, const_pointer _Last) {
            m_str.append(_First, _Last);
            return (*this);
        }
        
        basic_string& append(const_iterator _First, const_iterator _Last) {
            m_str.append(_First, _Last);
            return (*this);
        }
        
        basic_string& assign(const _Myt& _Right) {
            m_str.assign(_Right);
            return (*this);
        }
        
        basic_string& assign(const _Myt& _Right, size_type _Roff, size_type _Count) {
            m_str.assign(_Right, _Roff, _Count);
            return (*this);
        }
        
        basic_string& assign(const _Elem *_Ptr, size_type _Count) {
            m_str.assign(_Ptr, _Count);
            return (*this);
        }
        
        basic_string& assign(const _Elem *_Ptr) {
            m_str.assign(_Ptr);
            return (*this);
        }
        
        basic_string& assign(size_type _Count, _Elem _Ch) {
            m_str.assign(_Count, _Ch);
            return (*this);
        }
        
        
        
        basic_string& assign(const_pointer _First, const_pointer _Last) {
            m_str.assign(_First, _Last);
            return (*this);
        }
        
        basic_string& assign(const_iterator _First, const_iterator _Last) {
            m_str.assign(_First, _Last);
            return (*this);
        }
        
        basic_string& insert(size_type _Off, const _Myt& _Right) {
            m_str.insert(_Off, _Right);
            return (*this);
        }
        
        basic_string& insert(size_type _Off, const _Myt& _Right, size_type _Roff,
                             size_type _Count) {
            m_str.insert(_Off, _Right, _Roff, _Count);
            return (*this);
        }
        
        basic_string& insert(size_type _Off, const _Elem *_Ptr, size_type _Count) {
            m_str.insert(_Off, _Ptr, _Count);
            return (*this);
        }
        
        basic_string& insert(size_type _Off, const _Elem *_Ptr) {
            m_str.insert(_Off, _Ptr);
            return (*this);
        }
        
        basic_string& insert(size_type _Off, size_type _Count, _Elem _Ch) {
            m_str.insert(_Off, _Count, _Ch);
            return (*this);
        }
        
        void push_back(wchar_t _Ch) {
            this->operator+=(_Ch);
        }
        
        iterator erase(iterator _Where) {
            return m_str.erase(_Where);
        }
        
        iterator erase(iterator _First, iterator _Last) {
            return m_str.erase(_First, _Last);
        }
        
        basic_string& erase(size_type _Off = 0) {
            m_str.erase(_Off);
            return (*this);
        }
        
        basic_string& erase(size_type _Off, size_type _Count) {
            m_str.erase(_Off, _Count);
            return *this;
        }
        
        
        
        
        basic_string& replace(size_type _Off, size_type _N0, const _Myt& _Right) {
            m_str.replace(_Off, _N0, _Right);
            return *this;
        }
        
        basic_string& replace(size_type _Off, size_type _N0, const _Myt& _Right,
                              size_type _Roff, size_type _Count) {
            m_str.replace(_Off, _N0, _Right, _Roff, _Count);
            return *this;
        }
        
        basic_string& replace(size_type _Off,
                              size_type _N0, const _Elem *_Ptr, size_type _Count) {
            m_str.replace(_Off, _N0, _Ptr, _Count);
            return *this;
        }
        
        basic_string& replace(size_type _Off, size_type _N0, const _Elem *_Ptr) {
            m_str.replace(_Off, _N0, _Ptr);
            return *this;
        }
        
        basic_string& replace(size_type _Off, size_type _N0, size_type _Count,
                              _Elem _Ch) {
            m_str.replace(_Off, _N0, _Count, _Ch);
            return *this;
        }
        
        basic_string& replace(iterator _First, iterator _Last, const _Myt& _Right) {
            m_str.replace(_First, _Last, _Right);
            return *this;
        }
        
        basic_string& replace(iterator _First, iterator _Last, const _Elem *_Ptr,
                              size_type _Count) {
            m_str.replace(_First, _Last, _Ptr, _Count);
            return *this;
        }
        
        basic_string& replace(iterator _First, iterator _Last,
                              const _Elem *_Ptr) {
            m_str.replace(_First, _Last, _Ptr);
            return *this;
        }
        
        basic_string& replace(iterator _First, iterator _Last, size_type _Count,
                              _Elem _Ch) {
            m_str.replace(_First, _Last, _Count, _Ch);
            return *this;
        }
        
        basic_string& replace(iterator _First, iterator _Last, const_pointer _First2,
                              const_pointer _Last2) {
            m_str.replace(_First, _Last, _First2, _Last2);
            return *this;
        }
        
        basic_string& replace(iterator _First, iterator _Last, pointer _First2,
                              pointer _Last2) {
            m_str.replace(_First, _Last, _First2, _Last2);
            return *this;
        }
        
        basic_string& replace(iterator _First, iterator _Last, iterator _First2,
                              iterator _Last2) {
            m_str.replace(_First, _Last, _First2, _Last2);
            return *this;
        }
        
        basic_string substr(size_type _Off = 0,
                            size_type _Count = std::string::npos) const {
            return basic_string(m_str.substr(_Off, _Count), is_utf8());
        }
        
        
        int compare(const basic_string& _Right) const {
            return m_str.compare(_Right);
        }
        
        int compare(size_type _Off, size_type _N0, const _Myt& _Right) const {
            return m_str.compare(_Off, _N0, _Right);
        }
        
        int compare(size_type _Off, size_type _N0, const _Myt& _Right, size_type _Roff,
                    size_type _Count) const {
            return m_str.compare(_Off, _N0, _Right, _Roff, _Count);
        }
        
        int compare(const _Elem *_Ptr) const {
            return m_str.compare(_Ptr);
        }
        
        int compare(size_type _Off, size_type _N0, const _Elem *_Ptr) const {
            return m_str.compare(_Off, _N0, _Ptr);
        }
        
        int compare(size_type _Off, size_type _N0, const _Elem *_Ptr,
                    size_type _Count) const {
            return m_str.compare(_Off, _N0, _Ptr, _Count);
        }
        
        
        
        bool operator < (const basic_string& rhs) const {
            return m_str < rhs.m_str;
        }
        
        //------------------------------------------------------------------------------
        // 方法:    ImeString::Split
        // 功能:    分割字符串
        // 访问:    public
        // 返回:    std::vector< ImeString >
        // 参数:    ImeString sub
        //
        // 说明:
        //
        //------------------------------------------------------------------------------
        std::vector< basic_string > split(const basic_string& sub,
                                          bool remove_empty_str = true) const {
            std::string s = m_str;
            std::string delim = sub;
            
            std::vector<basic_string> ret;
            size_t start = 0;
            
            if (s.empty()) {
                return ret;
            }
            
            size_t index = s.find(delim, start);
            
            while (index != std::string::npos) {
            
                if (remove_empty_str) {
                    if (index - start != 0) {
                        ret.push_back(basic_string(s.substr(start, index - start), is_utf8()));
                    }
                } else {
                    ret.push_back(basic_string(s.substr(start, index - start), is_utf8()));
                }
                
                start = index + sub.size();
                index = s.find(delim, start);
            }
            
            size_t size1 = m_str.size();
            
            if (start < m_str.size()) {
                ret.push_back(basic_string(s.substr(start), is_utf8()));
            }
            
            return ret;
        }
        
        
        //------------------------------------------------------------------------------
        // 方法:    Replace
        // 功能:    替换字符串
        // 访问:    public
        // 返回:    ImeString&
        // 参数:    const ImeString & strsrc
        // 参数:    const ImeString & strdst
        //
        // 说明:
        //
        //------------------------------------------------------------------------------
        basic_string& replace(const basic_string &strsrc,
                              const basic_string &strdst) {
            amo::string_utils::replace(m_str, strsrc, strdst);
            return *this;
        }
        
        
        basic_string replace(const basic_string &strsrc,
                             const basic_string &strdst) const {
            basic_string str = *this;
            str.replace(strsrc, strdst);
            return str;
        }
        
        basic_string replace_c(const basic_string &strsrc,
                               const basic_string &strdst)const {
            return replace(strsrc, strdst);
        }
        
        //------------------------------------------------------------------------------
        // 方法:    ToUpper
        // 功能:    所有字符转为大写，改变自身
        // 访问:    public
        // 返回:    ImeString&
        //
        // 说明:
        //
        //------------------------------------------------------------------------------
        basic_string& to_upper() {
            std::transform(m_str.begin(), m_str.end(), m_str.begin(), toupper);
            return *this;
        }
        
        
        //------------------------------------------------------------------------------
        // 方法:    ToUpper
        // 功能:    所有字符转为大写，不改变自身
        // 访问:    public
        // 返回:    ImeString
        //
        // 说明:
        //
        //------------------------------------------------------------------------------
        basic_string to_upper() const {
            basic_string s(m_str, is_utf8());
            std::transform(m_str.begin(), m_str.end(), s.str().begin(), toupper);
            return s;
        }
        
        //------------------------------------------------------------------------------
        // 方法:    ToLower
        // 功能:    所有字符转为小写，改变自身
        // 访问:    public
        // 返回:    ImeString&
        //
        // 说明:
        //
        //------------------------------------------------------------------------------
        basic_string& to_lower() {
            std::transform(m_str.begin(), m_str.end(), m_str.begin(), tolower);
            return *this;
        }
        
        
        //------------------------------------------------------------------------------
        // 方法:    ToLower
        // 功能:    所有字符转为大写，不改变自身
        // 访问:    public
        // 返回:    ImeString
        //
        // 说明:
        //
        //------------------------------------------------------------------------------
        basic_string to_lower() const {
            basic_string s(m_str, is_utf8());
            std::transform(m_str.begin(), m_str.end(), s.str().begin(), tolower);
            return s;
        }
        
        //删除左边所有包含在target中的字符
        basic_string& trim(const basic_string& target = L" ") {
            return replace(target, L"");
        }
        
        basic_string trim(const basic_string& target = L" ") const {
            return replace_c(target, L"");
        }
        
        basic_string trim_c(const basic_string& target = L" ") const {
            return replace_c(target, L"");
        }
        
        //删除左边所有包含在target中的字符
        basic_string& trim_left(const basic_string& target) {
            while (!empty() && (target.find(*begin()) != std::string::npos)) {
                erase(begin());
            }
            
            return *this;
        }
        
        //删除右边所有包含在target中的字符
        basic_string& trim_right(const basic_string& target) {
            while (!empty() && target.find(*rbegin()) != std::string::npos) {
                m_str.erase(--end());
            }
            
            return *this;
        }
        
        //返回左边count个字符，count大于总长度则返回整个字符串
        basic_string left(size_type count) const {
            return substr(0, count);
        }
        
        //返回右边count个字符，count大于总长度则返回整个字符串
        basic_string right(size_type count) const {
            return substr(size() < count ? 0 : size() - count);
        }
        
        //忽略大小写判断两个字符串是否相等
        int compare_no_case(const basic_string& rhs) const {
            return to_lower().compare(rhs.to_lower());
        }
        
        //判断字符串是否以制定字符串开头
        bool begin_with(const basic_string& rhs) const {
            return find(rhs) == size_type(0);
        }
        
        //判断字符串是否以制定字符串结尾
        bool end_with(const basic_string& rhs) const {
            if (rhs.size() > size()) {
                return false;
            }
            
            return compare(size() - rhs.size(), rhs.size(), rhs.str()) == 0;
        }
        bool start_with(const basic_string& rhs) const {
            return begin_with(rhs);
        }
        
        //------------------------------------------------------------------------------
        // 方法:    ToNumber
        // 功能:    转换成数字
        // 访问:    public
        // 返回:    boost::T
        // 参数:    int base
        //
        // 说明:
        //
        //------------------------------------------------------------------------------
        template<typename T>
        T to_number(int base = 10) {
            T t = T();
            std::string str = m_str;
            amo::string_utils::trim_left(str, " \t");
            
            std::stringstream ss(str);
            
            return convert_to_number(ss, t, base);
        }
        
        template<typename T>
        T lexical_cast() {
            std::string str = m_str;
            amo::string_utils::trim_left(str, " \t");
            return amo::lexical_cast<T>(str);
            
        }
        
        //将整数转化为字符串
        //base:进制数。可以为8,10,16，如果其它值则强制为10
        template<typename T>
        static basic_string from_number(T number, int base = 10) {
        
            int offset = 0;
            int mask = 0;
            
            if (base == 2) {
                offset = 1;
                mask = 0x1;
            } else if (base == 8) {
                offset = 3;
                mask = 0x7;
            } else if (base == 16) {
                offset = 4;
                mask = 0xf;
            }
            
            if (offset == 0) {
                return  basic_string(std::to_string(number), is_utf8());
            }
            
            //	在C语言中，对于移位操作执行的是逻辑左移和算术右移，不过对于无符号类型，所有的移位操作都是逻辑的。
            T val = number;
            
            basic_string retval;
            
            
            std::stack<char> st;
            
            size_t len = sizeof(T) * 8 / offset;
            
            amo::binary bin(number);
            
            for (size_t i = 0 ; i < len; i++) {
                amo::binary b1 = bin.sub((i + 1) * offset - 1, offset);
                char c = b1;
                
                if (c < 10) {
                    st.push(c + 0x30);
                } else {
                    st.push(c - 10 + 0x61);
                }
            }
            
            
            len = sizeof(T) * 8;
            
            if ((len % offset) != 0) {
                amo::binary b1 = bin.sub(len - 1, len % offset);
                char c = b1;
                
                if (c < 10) {
                    st.push(c + 0x30);
                } else {
                    st.push(c - 10 + 0x61);
                }
            }
            
            while (!st.empty()) {
                if (st.top() == '0') {
                    st.pop();
                } else {
                    break;
                }
            }
            
            
            while (!st.empty()) {
                char c = st.top();
                st.pop();
                retval += c;
            }
            
            return retval;
            
        }
        
        //将float转化为字符串
        //f:格式化参数。可以为'f','e','E','g','G'。'f'为定点数,'e'或'E'表示科学计数法
        //  'g'或‘G’表示格式化为定点数或科学计数法，看哪一个表示方便。
        //prec:小数点后的位数(定点数表示法)或总的有效位数(科学计数法)
        static basic_string from_number(float number, _Elem f = _Elem('g'),
                                        int prec = 6) {
            return from_number(static_cast<double>(number), f, prec);
        }
        
        //将double转化为字符串，参数解释同上
        static basic_string from_number(double number, _Elem f = _Elem('g'),
                                        int prec = 6) {
            std::stringstream ss;
            ss << std::setprecision(prec);
            
            if (f == 'f') {
                ss << std::setiosflags(std::ios_base::fixed);
            } else if ((f == 'e') || (f == 'E')) {
                ss << std::setiosflags(std::ios_base::scientific);
            }
            
            ss << number;
            return basic_string(ss.str(), is_utf8());
        }
        
        
        
        basic_string format(const amo::basic_json<CodingType>& json) const {
            std::regex args("\\{\\w+\\}");
            std::smatch m;
            std::string ss = m_str;
            std::string leftFlag = "--4652edc3-0bd2-4653-abbb-1eb8842da732--";
            std::string rightFlag = "--2d2fa095-cef4-4b9e-87f3-b9aeff613241 --";
            
            while (std::regex_search(ss, m, args)) {
                basic_string key(m[0].str(), this->is_utf8());
                key = key.replace(basic_string("{", is_utf8()), basic_string("\\{", is_utf8()));
                key = key.replace(basic_string("}", is_utf8()), basic_string("\\}", is_utf8()));
                
                std::regex e(key.str());
                key = key.replace(basic_string("\\{", is_utf8()), basic_string("", is_utf8()));
                key = key.replace(basic_string("\\}", is_utf8()), basic_string("", is_utf8()));
                
                std::string dststr = jsonvalue_to_string(json, key.str());
                amo::string_utils::replace(dststr, "{", leftFlag);
                amo::string_utils::replace(dststr, "}", rightFlag);
                ss = std::regex_replace(ss, e, dststr);
            }
            
            amo::string_utils::replace(ss, leftFlag, "{");
            amo::string_utils::replace(ss, rightFlag, "}");
            
            return basic_string(ss, is_utf8());
        }
        
        basic_string format(const
                            std::unordered_map<std::string, std::string>&
                            map) {
            return	basic_string(amo::string_utils::format(m_str, map), is_utf8());
        }
        
        
        //将szFormat格式化为字符串，参数解释同sprintf
        basic_string& format(const _Elem * szFormat, ...) {
            if (!szFormat) {
                return *this;
            }
            
            va_list argList;
            va_start(argList, szFormat);
            formatv(szFormat, argList);
            va_end(argList);
            return *this;
        }
        
        //将szFormat格式化为字符串，参数解释同sprintf
        void formatv(const _Elem * szFormat, va_list argList) {
            if (!szFormat) {
                return;
            }
            
            int nLength = _vscprintf(szFormat, argList);
            
            if (nLength < 0) {
                return;
            }
            
            resize(nLength);
            vsprintf_s(c_str(), nLength + 1, szFormat, argList);
            va_end(argList);
        }
#ifdef __linux__
        int _vscprintf(const char * format, va_list pargs) {
            int retval;
            va_list argcopy;
            va_copy(argcopy, pargs);
            retval = vsnprintf(NULL, 0, format, argcopy);
            va_end(argcopy);
            return retval;
        }
#endif
        
    private:
        static std::ios_base::fmtflags BaseFlag(int base) {
            switch (base) {
            case 16:
                return std::ios_base::hex;
                
            case 8:
                return std::ios_base::oct;
                
                
                
            default:
                return std::ios_base::dec;
            }
            
            /*  case 2:
            return std::ios_base::binary;*/
            /* return (base == 16) ? (std::ios_base::hex) :
                    ((base == 8) ? (std::ios_base::oct) : (std::ios_base::dec));*/
        }
        
        template< typename Type>
        Type convert_to_number(std::stringstream & ss, Type t, int base) {
            //std::stoull() 转换
            if (base == 2) {
                std::string str = ss.str();
                
                amo::string_utils::trim_left(str, " \t");
                amo::string_utils::trim_left(str, "0");
                
                if (str.size() > 64) {
                    str = str.substr(0, 64);
                }
                
                std::vector<int> vec;
                
                for (size_t i = 0; i < str.size(); ++i) {
                
                    if (str[i] == '0') {
                        vec.push_back(0);
                    } else if (str[i] == '1') {
                        vec.push_back(1);
                    } else {
                        break;
                    }
                }
                
                uint64_t val = 0;
                
                for (size_t i = 0; i < vec.size(); ++i) {
                    val |= (vec[vec.size() - i - 1] << i);
                }
                
                return (Type)val;
                
            } else {
                uint64_t val = 0;
                ss.setf(BaseFlag(base), std::ios_base::basefield);
                ss >> val;
                return (Type)val;
            }
            
        }
        
        /*int8_t convert_to_number(std::stringstream& ss, int8_t t,
                                 int base) {
            int32_t val;
            ss.setf(BaseFlag(base), std::ios_base::basefield);
            ss >> val;
            return (int8_t)val;
        }
        */
        
        //unsigned short convert_to_number(std::stringstream& ss, unsigned short t,
        //                                 int/*ignore base*/) {
        //    int val = 0;
        //    ss >> val;
        //    return (unsigned short)val;
        //}
        
        float convert_to_number(std::stringstream & ss, float t, int/*ignore base*/) {
            ss >> t;
            return t;
        }
        
        
        double convert_to_number(std::stringstream & ss, double t, int/*ignore base*/) {
            ss >> t;
            return t;
        }
        
        
        std::string jsonvalue_to_string(const amo::basic_json<CodingType>& json,
                                        const std::string & key) const {
            if (!json.contains_key(key)) {
                return "";
            }
            
            if (json.is_null(key)) {
                return "null";
            } else if (json.is_false(key)) {
                return "false";
            } else if (json.is_true(key)) {
                return "true";
            } else if (json.is_bool(key)) {
                bool b = json.getBool(key);
                
                if (b) {
                    return "true";
                }
                
                return "false";
            } else if (json.is_object(key)) {
                return "object";
            } else if (json.is_array(key)) {
                return "array";
            }
            /*	else if (m_json.is_number(key))  {
            }*/
            else if (json.is_int(key)) {
                return amo::basic_string<CodingType>::from_number<int32_t>(json.getInt
                        (key));
            } else 	if (json.is_uint(key)) {
                return amo::basic_string<CodingType>::from_number<uint32_t>(json.getUint
                        (key));
            } else if (json.is_int64(key)) {
                return amo::basic_string<CodingType>::from_number<int64_t>(json.getInt64
                        (key));
            } else if (json.is_uint64(key)) {
                return amo::basic_string<CodingType>::from_number<uint64_t>(json.getUint64
                        (key));
            } else if (json.is_double(key)) {
                return amo::basic_string<CodingType>::from_number(json.getDouble(key));
            } else if (json.is_string(key)) {
                return json.getString(key);
            }
            
            return "";
        }
        
    private:
    
    
    };
    
    
}




namespace std {
    template <>
    struct hash<amo::basic_string<::amo::string_coding_ansi> > :
        public unary_function
        < amo::basic_string<::amo::string_coding_ansi>, size_t > {
    public:
        size_t operator()(const
                          amo::basic_string<::amo::string_coding_ansi> & value)
        const {
            std::hash<std::string> hs;
            return hs(value);
        }
    };
    
    
    template <>
    struct equal_to<amo::basic_string<::amo::string_coding_ansi> > :
        public
        unary_function
        < amo::basic_string<::amo::string_coding_ansi>, bool > {
    public:
        bool operator()(const amo::basic_string<::amo::string_coding_ansi>
                        & x,
                        const amo::basic_string<::amo::string_coding_ansi> & y) const {
            std::equal_to<std::string> eq;
            return eq(x, y);
        }
    };
    
    template <>
    struct hash<amo::basic_string<::amo::string_coding_utf8> > :
        public unary_function
        < amo::basic_string<::amo::string_coding_utf8>, size_t > {
    public:
        size_t operator()(const
                          amo::basic_string<::amo::string_coding_utf8>& value) const {
            std::hash<std::string> hs;
            return hs(value);
        }
    };
    
    
    template <>
    struct equal_to<amo::basic_string<::amo::string_coding_utf8>> :
                public
                unary_function
                < amo::basic_string<::amo::string_coding_utf8>, bool > {
    public:
        bool operator()(const
                        amo::basic_string<::amo::string_coding_utf8>& x,
                        const amo::basic_string<::amo::string_coding_utf8>& y) const {
            std::equal_to<std::string> eq;
            return eq(x, y);
        }
    };
    
} // namespace std


#endif //AMO_BASIC_STRING_52E89957_B4A1_48AE_85D7_438BFA8B69C9_HPP__