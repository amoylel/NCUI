// Created by amoylel on 30/09/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_STRING_UTILS_D72E1F7C_8BFA_48BA_A990_6129BC202A86_HPP__
#define AMO_STRING_UTILS_D72E1F7C_8BFA_48BA_A990_6129BC202A86_HPP__






#include <string>
#include <unordered_map>
#include <regex>
#include <amo/config.hpp>
//#include <amo/windows.hpp>

//#include <amo/string/iconv.hpp>
#include <amo/string/coding_type.hpp>
#include <algorithm>

#include <boost/locale.hpp>
#include <boost/locale/encoding.hpp>
#include <boost/locale/util.hpp>
#include <boost/locale.hpp>

//#include <codecvt>


namespace amo {
    namespace string_utils {
    
    
        /*static std::string gb2312_to_utf8(std::string const &strGb2312) {
            std::vector<wchar_t> buff(strGb2312.size());
        #ifdef _MSC_VER
            std::locale loc("zh-CN");
        #else
            std::locale loc("zh_CN.GB18030");
        #endif
            wchar_t* pwszNext = nullptr;
            const char* pszNext = nullptr;
            mbstate_t state = {};
            int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t> >
                      (loc).in(state,
                               strGb2312.data(), strGb2312.data() + strGb2312.size(), pszNext,
                               buff.data(), buff.data() + buff.size(), pwszNext);
        
            if (std::codecvt_base::ok == res) {
                std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
                return cutf8.to_bytes(std::wstring(buff.data(), pwszNext));
            }
        
            return "";
        
        }
        
        static std::string utf8_to_gb2312(std::string const &strUtf8) {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
            std::wstring wTemp = cutf8.from_bytes(strUtf8);
        #ifdef _MSC_VER
            std::locale loc("zh-CN");
        #else
            std::locale loc("zh_CN.GB18030");
        #endif
            const wchar_t* pwszNext = nullptr;
            char* pszNext = nullptr;
            mbstate_t state = {};
        
            std::vector<char> buff(wTemp.size() * 2);
            int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t> >
                      (loc).out(state,
                                wTemp.data(), wTemp.data() + wTemp.size(), pwszNext,
                                buff.data(), buff.data() + buff.size(), pszNext);
        
            if (std::codecvt_base::ok == res) {
                return std::string(buff.data(), pszNext);
            }
        
            return "";
        }*/
        //
        //        #ifdef _WIN32
        //        static std::string UnicodeToAnsi(LPCWSTR pString) {
        //            char *pANSI = NULL;
        //            int nByte = 0;
        //            nByte = WideCharToMultiByte(CP_ACP, 0, pString, -1, NULL, 0, NULL, NULL);
        //            pANSI = (char*)malloc(nByte + 1);
        //            memset(pANSI, 0, nByte + 1);
        //            WideCharToMultiByte(CP_ACP, 0, pString, -1, pANSI, nByte, NULL, NULL);
        //            std::string str = pANSI;
        //            free(pANSI);
        //            return str;
        //        }
        //
        //        static std::string UnicodeToUtf8(const wchar_t* unicode) {
        //            int len;
        //            len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
        //            char *szUtf8 = (char*)malloc(len + 1);
        //            memset(szUtf8, 0, len + 1);
        //            WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
        //            std::string retval =
        //                szUtf8;// (szUtf8, len); len长度有问题，会多个\0出来
        //            free(szUtf8);
        //            return retval;
        //        }
        //        static std::wstring AnsiToUnicode(LPCSTR pString) {
        //            wchar_t *pUnicode = NULL;
        //            int nWideCharLen = 0;
        //            nWideCharLen = MultiByteToWideChar(CP_ACP, 0, pString, -1, NULL, 0);
        //            pUnicode = (wchar_t*)malloc((nWideCharLen + 1) * sizeof(wchar_t));
        //            memset(pUnicode, 0, (nWideCharLen + 1) * sizeof(wchar_t));
        //            MultiByteToWideChar(CP_ACP, 0, pString, -1, pUnicode, nWideCharLen);
        //            std::wstring str = pUnicode;
        //            free(pUnicode);
        //            return str;
        //        }
        //
        //        static std::string UTF8ToANSI(LPCSTR pString) {
        //            int nwLen = MultiByteToWideChar(CP_UTF8, 0, pString, -1, NULL, 0);
        //            wchar_t * pwBuf = new wchar_t[nwLen + 1];
        //            memset(pwBuf, 0, (nwLen + 1) * sizeof(wchar_t));
        //            MultiByteToWideChar(CP_UTF8, 0, pString, strlen(pString), pwBuf, nwLen);
        //            int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
        //            char * pBuf = new char[nLen + 1];
        //            memset(pBuf, 0, nLen + 1);
        //            WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
        //            std::string retStr = pBuf;
        //            delete[]pBuf;
        //            delete[]pwBuf;
        //            pBuf = NULL;
        //            pwBuf = NULL;
        //
        //            return retStr;
        //        }
        //
        //        static std::string ANSIToUTF8(LPCSTR lpszAnsi) {
        //            int wlen = MultiByteToWideChar(CP_ACP, 0, lpszAnsi, -1, NULL, 0);
        //            wchar_t* pwBuf = (wchar_t*)malloc(sizeof(wchar_t) * (wlen + 1));
        //            memset(pwBuf, 0, (wlen + 1) * sizeof(wchar_t));
        //            MultiByteToWideChar(CP_ACP, 0, lpszAnsi, strlen(lpszAnsi), pwBuf, wlen);
        //            int len = WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
        //            char* pBuf = (char*)malloc(len + 1);
        //            memset(pBuf, 0, len + 1);
        //            WideCharToMultiByte(CP_UTF8, 0, pwBuf, wlen, pBuf, len, NULL, NULL);
        //            std::string str = pBuf;
        //            free(pBuf);
        //            free(pwBuf);
        //            return str;
        //        }
        //
        //
        //
        //        static std::wstring UTF8ToUnicode(LPCSTR pString) {
        //            int nwLen = MultiByteToWideChar(CP_UTF8, 0, pString, -1, NULL, 0);
        //            wchar_t * pwBuf = new wchar_t[nwLen + 1];
        //            memset(pwBuf, 0, (nwLen + 1) * sizeof(wchar_t));
        //            MultiByteToWideChar(CP_UTF8, 0, pString, strlen(pString), pwBuf, nwLen);
        //            std::wstring retStr = pwBuf;
        //            delete[]pwBuf;
        //            pwBuf = NULL;
        //
        //            return retStr;
        //        }
        //        #endif
        //
        //        #ifdef __linux__
        //
        //        static std::wstring string_to_swtring(const char *pc) {
        //            std::wstring val = L"";
        //
        //            if (NULL == pc) {
        //                return val;
        //            }
        //
        //            //size_t size_of_ch = strlen(pc)*sizeof(char);
        //            //size_t size_of_wc = get_wchar_size(pc);
        //            size_t size_of_wc;
        //            size_t destlen = mbstowcs(0, pc, 0);
        //
        //            if (destlen == (size_t)(-1)) {
        //                return val;
        //            }
        //
        //            size_of_wc = destlen + 1;
        //            wchar_t * pw = new wchar_t[size_of_wc];
        //            mbstowcs(pw, pc, size_of_wc);
        //            val = pw;
        //            delete pw;
        //            return val;
        //        }
        //        /*
        //        wstring 转换为 string
        //        */
        //        static std::string wstring_to_string(const wchar_t * pw) {
        //            std::string val = "";
        //
        //            if (!pw) {
        //                return val;
        //            }
        //
        //            size_t size = wcslen(pw) * sizeof(wchar_t);
        //            char *pc = NULL;
        //
        //            if (!(pc = (char*)malloc(size))) {
        //                return val;
        //            }
        //
        //            size_t destlen = wcstombs(pc, pw, size);
        //
        //            /*转换不为空时，返回值为-1。如果为空，返回值0*/
        //            if (destlen == (size_t)(0)) {
        //                return val;
        //            }
        //
        //            val = pc;
        //            delete pc;
        //            return val;
        //        }
        //#endif
        //
        //        static std::wstring utf8_to_wide(const std::string& utf8string) {
        //            $windows({
        //                return UTF8ToUnicode(utf8string.c_str());
        //            })
        //            $linux({
        //                amo::converter conv("UTF-8", "GBK");
        //                std::string out ;
        //                conv.convert(utf8string, out);
        //                return string_to_swtring(out.c_str());
        //            })
        //            return L"";
        //
        //        }
        //
        //        static std::string wide_to_utf8(const std::wstring& widestring) {
        //            $windows({
        //                return UnicodeToUtf8(widestring.c_str());
        //            })
        //            $linux({
        //                amo::converter conv("GBK", "UTF-8");
        //                std::string in = wstring_to_string(widestring.c_str());
        //                std::string out;
        //                conv.convert(in, out);
        //                return out;
        //            })
        //            return "";
        //        }
        //
        //        static std::string wide_to_ansi(const std::wstring& widestring) {
        //            $windows({
        //                return UnicodeToAnsi(widestring.c_str());
        //            })
        //            $linux({
        //                return wstring_to_string(widestring.c_str());
        //            })
        //            return "";
        //        }
        //
        //        static std::wstring ansi_to_wide(const std::string& ansistring) {
        //            $windows({
        //                return AnsiToUnicode(ansistring.c_str());
        //            })
        //            $linux({
        //                return string_to_swtring(ansistring.c_str());
        //            })
        //            return L"";
        //        }
        //
        //        static std::string ansi_to_utf8(const std::string& ansistring) {
        //            $windows({
        //                return ANSIToUTF8(ansistring.c_str());
        //            })
        //            $linux({
        //                amo::converter conv("GBK", "UTF-8");
        //                std::string out;
        //                conv.convert(ansistring, out);
        //                return out;
        //            })
        //            return "";
        //        }
        //
        //        static std::string utf8_to_ansi(const std::string& utf8string) {
        //            $windows({
        //                return UTF8ToANSI(utf8string.c_str());
        //            })
        //            $linux({
        //                amo::converter conv("UTF-8", "GBK");
        //                std::string out;
        //                conv.convert(utf8string, out);
        //                return out;
        //            })
        //            return "";
        //		}
        //
        
        
        static std::string to_upper(const std::string& m_str);
        
        static std::locale  get_system_locale() {
            std::string code_page = boost::locale::util::get_system_locale();
            return boost::locale::generator().generate(code_page);
        }
        
        static bool  is_utf8_system() {
            std::string from_encoding = std::use_facet<boost::locale::info>
                                        (get_system_locale()).encoding();
                                        
            from_encoding = amo::string_utils::to_upper(from_encoding);
            
            if (from_encoding.find("UTF-8") != -1) {
                return true;
            }
            
            if (from_encoding.find("UTF8") != -1) {
                return true;
            }
            
            return false;
        }
        
        static std::string ansi_to_utf8(const std::string& src) {
            std::string from_encoding = std::use_facet<boost::locale::info>
                                        (get_system_locale()).encoding();
            return  boost::locale::conv::between(src, "UTF-8", from_encoding);
        }
        
        static std::wstring ansi_to_wide(const std::string& src) {
            return  boost::locale::conv::to_utf<wchar_t>(src, get_system_locale());
        }
        
        static std::wstring utf8_to_wide(const std::string& src) {
            return  boost::locale::conv::utf_to_utf<wchar_t>(src);
        }
        
        static std::string utf8_to_ansi(const std::string& src) {
            std::string from_encoding = std::use_facet<boost::locale::info>
                                        (get_system_locale()).encoding();
            return  boost::locale::conv::from_utf(src, from_encoding);
        }
        
        static std::string wide_to_ansi(const std::wstring& src) {
            std::string from_encoding = std::use_facet<boost::locale::info>
                                        (get_system_locale()).encoding();
            return boost::locale::conv::from_utf(src, from_encoding);
        }
        
        static std::string wide_to_utf8(const std::wstring& src) {
            return boost::locale::conv::utf_to_utf<char>(src);
        }
        
        
        
        static std::string utf8_to_string(const std::string& utf8string,
                                          StringCodingType codingtype) {
            switch (codingtype) {
            case amo::CODING_TYPE_ANSI:
                return utf8_to_ansi(utf8string);
                
            case amo::CODING_TYPE_UNKNOWN:
            case amo::CODING_TYPE_UTF8:
            default:
                return utf8string;
            }
        }
        
        static std::string ansi_to_string(const std::string& ansistring,
                                          StringCodingType codingtype) {
            switch (codingtype) {
            case amo::CODING_TYPE_UTF8:
                return ansi_to_utf8(ansistring);
                
            case amo::CODING_TYPE_UNKNOWN:
            case amo::CODING_TYPE_ANSI:
            default:
                return ansistring;
            }
        }
        
        static std::string wide_to_string(const std::wstring& widestring,
                                          StringCodingType codingtype) {
            switch (codingtype) {
            case amo::CODING_TYPE_UTF8:
                return wide_to_utf8(widestring);
                
            case amo::CODING_TYPE_UNKNOWN:
            case amo::CODING_TYPE_ANSI:
            default:
                return wide_to_ansi(widestring);
            }
        }
        
        static std::wstring string_to_wide(const std::string& anystring,
                                           StringCodingType codingtype) {
                                           
            switch (codingtype) {
            case amo::CODING_TYPE_UTF8:
                return utf8_to_wide(anystring);
                
            case amo::CODING_TYPE_ANSI:
            case amo::CODING_TYPE_UNKNOWN:
            default:
                return ansi_to_wide(anystring);
            }
        }
        
        static std::string string_to_utf8(const std::string& anystring,
                                          StringCodingType codingtype) {
                                          
            switch (codingtype) {
            case amo::CODING_TYPE_ANSI:
                return ansi_to_utf8(anystring);
                
            case amo::CODING_TYPE_UTF8:
            case amo::CODING_TYPE_UNKNOWN:
            default:
                return anystring;
            }
        }
        
        
        static std::string string_to_ansi(const std::string& anystring,
                                          StringCodingType codingtype) {
                                          
            switch (codingtype) {
            case amo::CODING_TYPE_UTF8:
                return utf8_to_ansi(anystring);
                
            case amo::CODING_TYPE_ANSI:
            
            case amo::CODING_TYPE_UNKNOWN:
            default:
                return anystring;
            }
        }
        
        static bool is_utf8(const char * string) {
            if (!string) {
                return false;
            }
            
            const unsigned char * bytes = (const unsigned char *)string;
            
            while (*bytes) {
                if ((// ASCII
                            // use bytes[0] <= 0x7F to allow ASCII control characters
                            bytes[0] == 0x09 ||
                            bytes[0] == 0x0A ||
                            bytes[0] == 0x0D ||
                            (0x20 <= bytes[0] && bytes[0] <= 0x7E)
                        )
                   ) {
                    bytes += 1;
                    continue;
                }
                
                if ((// non-overlong 2-byte
                            (0xC2 <= bytes[0] && bytes[0] <= 0xDF) &&
                            (0x80 <= bytes[1] && bytes[1] <= 0xBF)
                        )
                   ) {
                    bytes += 2;
                    continue;
                }
                
                if ((// excluding overlongs
                            bytes[0] == 0xE0 &&
                            (0xA0 <= bytes[1] && bytes[1] <= 0xBF) &&
                            (0x80 <= bytes[2] && bytes[2] <= 0xBF)
                        ) ||
                        (// straight 3-byte
                            ((0xE1 <= bytes[0] && bytes[0] <= 0xEC) ||
                             bytes[0] == 0xEE ||
                             bytes[0] == 0xEF) &&
                            (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
                            (0x80 <= bytes[2] && bytes[2] <= 0xBF)
                        ) ||
                        (// excluding surrogates
                            bytes[0] == 0xED &&
                            (0x80 <= bytes[1] && bytes[1] <= 0x9F) &&
                            (0x80 <= bytes[2] && bytes[2] <= 0xBF)
                        )
                   ) {
                    bytes += 3;
                    continue;
                }
                
                if ((// planes 1-3
                            bytes[0] == 0xF0 &&
                            (0x90 <= bytes[1] && bytes[1] <= 0xBF) &&
                            (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                            (0x80 <= bytes[3] && bytes[3] <= 0xBF)
                        ) ||
                        (// planes 4-15
                            (0xF1 <= bytes[0] && bytes[0] <= 0xF3) &&
                            (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
                            (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                            (0x80 <= bytes[3] && bytes[3] <= 0xBF)
                        ) ||
                        (// plane 16
                            bytes[0] == 0xF4 &&
                            (0x80 <= bytes[1] && bytes[1] <= 0x8F) &&
                            (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                            (0x80 <= bytes[3] && bytes[3] <= 0xBF)
                        )
                   ) {
                    bytes += 4;
                    continue;
                }
                
                return false;
            }
            
            return true;
        }
        
        static bool is_utf8(const std::string& str) {
            return is_utf8(str.c_str());
        }
        
        static bool validate_utf8(const char *str, int len) {
            int i;
            int j;
            int codelen;
            int codepoint;
            const unsigned char *ustr = (const unsigned char *)str;
            
            if (!str) {
                return false;
            }
            
            if (len < 0 || len > 65536) {
                return false;
            }
            
            for (i = 0; i < len; i++) {
                if (ustr[i] == 0) {
                    return false;
                } else if (ustr[i] <= 0x7f) {
                    codelen = 1;
                    codepoint = ustr[i];
                } else if ((ustr[i] & 0xE0) == 0xC0) {
                    /* 110xxxxx - 2 byte sequence */
                    if (ustr[i] == 0xC0 || ustr[i] == 0xC1) {
                        /* Invalid bytes */
                        return false;
                    }
                    
                    codelen = 2;
                    codepoint = (ustr[i] & 0x1F);
                } else if ((ustr[i] & 0xF0) == 0xE0) {
                    // 1110xxxx - 3 byte sequence
                    codelen = 3;
                    codepoint = (ustr[i] & 0x0F);
                } else if ((ustr[i] & 0xF8) == 0xF0) {
                    // 11110xxx - 4 byte sequence
                    if (ustr[i] > 0xF4) {
                        /* Invalid, this would produce values > 0x10FFFF. */
                        return false;
                    }
                    
                    codelen = 4;
                    codepoint = (ustr[i] & 0x07);
                } else {
                    /* Unexpected continuation byte. */
                    return false;
                }
                
                /* Reconstruct full code point */
                if (i == len - codelen + 1) {
                    /* Not enough data */
                    return false;
                }
                
                for (j = 0; j < codelen - 1; j++) {
                    if ((ustr[++i] & 0xC0) != 0x80) {
                        /* Not a continuation byte */
                        return false;
                    }
                    
                    codepoint = (codepoint << 6) | (ustr[i] & 0x3F);
                }
                
                /* Check for UTF-16 high/low surrogates */
                if (codepoint >= 0xD800 && codepoint <= 0xDFFF) {
                    return false;
                }
                
                /* Check for overlong or out of range encodings */
                if (codelen == 2 && codepoint < 0x0080) {
                    return false;
                } else if (codelen == 3 && codepoint < 0x0800) {
                    return false;
                } else if (codelen == 4 && (codepoint < 0x10000 || codepoint > 0x10FFFF)) {
                    return false;
                }
            }
            
            return true;
        }
        
        static bool validate_utf8(const std::string& str) {
            return validate_utf8(str.c_str(), str.size());
        }
        
        
        static std::vector< std::string > split(const std::string& src,
                                                const std::string& sub)   {
            std::string s = src;
            std::string delim = sub;
            
            std::vector<std::string> ret;
            size_t start = 0;
            
            if (s.empty()) {
                ret.push_back("");
                return ret;
            }
            
            size_t index = s.find_first_of(delim, start);
            
            while (index != std::string::npos) {
                ret.push_back(std::string(s.substr(start, index - start)));
                start = index + sub.size();
                index = s.find_first_of(delim, start);
            }
            
            size_t size1 = src.size();
            
            if (start < src.size()) {
                ret.push_back(std::string(s.substr(start)));
            }
            
            return ret;
        }
        
        static std::vector< std::string > split_regex(const std::string& src,
                const std::string& sub) {
            std::string s = src;
            std::string delim = sub;
            
            std::string str_reg = "^.+?" + sub;
            std::regex reg(str_reg);
            
            std::vector<std::string> ret;
            size_t start = 0;
            
            if (s.empty()) {
                ret.push_back("");
                return ret;
            }
            
            std::smatch m;
            std::vector<std::string> sources;
            
            
            while (std::regex_search(s, m, reg)) {
                std::string mstr = m[0].str();
                std::string item = mstr.substr(0, mstr.size() - sub.size());
                ret.push_back(item);
                
                s = s.substr(m.prefix().length() + m[0].length());
            }
            
            
            return ret;
        }
        
        static void replace(std::string& m_str, const std::string &strsrc,
                            const std::string &strdst) {
            if (strsrc.empty()) {
                return  ;
            }
            
            std::string::size_type pos = 0;
            std::string::size_type srclen = strsrc.size();
            std::string::size_type dstlen = strdst.size();
            
            while ((pos = m_str.find(strsrc, pos)) != std::string::npos) {
                m_str.replace(pos, srclen, strdst);
                pos += dstlen;
            }
            
            return  ;
        }
        
        
        
        static void replace(std::wstring& m_str, const std::wstring &strsrc,
                            const std::wstring &strdst) {
            if (strsrc.empty()) {
                return;
            }
            
            std::wstring::size_type pos = 0;
            std::wstring::size_type srclen = strsrc.size();
            std::wstring::size_type dstlen = strdst.size();
            
            while ((pos = m_str.find(strsrc, pos)) != std::wstring::npos) {
                m_str.replace(pos, srclen, strdst);
                pos += dstlen;
            }
            
            return;
        }
        
        
        static void replace_first(std::string& m_str, const std::string &strsrc,
                                  const std::string &strdst) {
            if (strsrc.empty()) {
                return;
            }
            
            std::string::size_type pos = 0;
            std::string::size_type srclen = strsrc.size();
            std::string::size_type dstlen = strdst.size();
            
            while ((pos = m_str.find(strsrc, pos)) != std::string::npos) {
                m_str.replace(pos, srclen, strdst);
                break;
            }
            
            return;
        }
        
        static void replace_first(std::wstring& m_str, const std::wstring &strsrc,
                                  const std::wstring &strdst) {
            if (strsrc.empty()) {
                return;
            }
            
            std::wstring::size_type pos = 0;
            std::wstring::size_type srclen = strsrc.size();
            std::wstring::size_type dstlen = strdst.size();
            
            while ((pos = m_str.find(strsrc, pos)) != std::wstring::npos) {
                m_str.replace(pos, srclen, strdst);
                break;
            }
            
            return;
        }
        static std::string replace_c(const std::string& m_str,
                                     const std::string &strsrc,
                                     const std::string &strdst) {
            std::string str = m_str;
            replace(str, strsrc, strdst);
            return str;
        }
        static std::wstring replace_c(const std::wstring& m_str,
                                      const std::wstring &strsrc,
                                      const std::wstring &strdst) {
            std::wstring str = m_str;
            replace(str, strsrc, strdst);
            return str;
        }
        
        static std::string replace_first_c(const std::string& m_str,
                                           const std::string &strsrc,
                                           const std::string &strdst) {
            std::string str = m_str;
            replace_first(str, strsrc, strdst);
            return str;
        }
        
        static std::wstring replace_first_c(const std::wstring& m_str,
                                            const std::wstring &strsrc,
                                            const std::wstring &strdst) {
            std::wstring str = m_str;
            replace_first_c(str, strsrc, strdst);
            return str;
        }
        
        static std::string mapValueToString(const
                                            std::unordered_map<std::string, std::string>& map, const std::string& key) {
            auto iter = map.find(key);
            
            if (iter == map.end()) {
                return "";
            }
            
            return iter->second;
        }
        
        static std::string format(const std::string& src,
                                  const std::unordered_map<std::string, std::string>& map)   {
            std::regex args("\\{\\w+\\}");
            std::smatch m;
            std::string ss = src;
            std::string leftFlag = "--4652edc3-0bd2-4653-abbb-1eb8842da732--";
            std::string rightFlag = "--2d2fa095-cef4-4b9e-87f3-b9aeff613241 --";
            
            while (std::regex_search(ss, m, args)) {
                std::string key = m[0].str();
                replace(key, "{", "\\{");
                replace(key, "}", "\\}");
                
                std::regex e(key);
                replace(key, "\\{", "");
                replace(key, "\\}", "");
                
                std::string dststr = mapValueToString(map, key);
                amo::string_utils::replace(dststr, "{", leftFlag);
                amo::string_utils::replace(dststr, "}", rightFlag);
                ss = std::regex_replace(ss, e, dststr);
                /* ss = std::regex_replace(ss, e, mapValueToString(map, key));*/
            }
            
            amo::string_utils::replace(ss, leftFlag, "{");
            amo::string_utils::replace(ss, rightFlag, "}");
            return ss;
        }
        
        
        static  std::string& to_upper(std::string& m_str) {
            std::transform(m_str.begin(), m_str.end(), m_str.begin(), toupper);
            return m_str;
        }
        
        
        static std::string to_upper(const std::string& m_str)  {
            std::string s(m_str);
            std::transform(m_str.begin(), m_str.end(), s.begin(), toupper);
            return s;
        }
        
        static std::string to_upper_copy(const std::string& m_str) {
            std::string s(m_str);
            std::transform(m_str.begin(), m_str.end(), s.begin(), toupper);
            return s;
            
        }
        
        static std::string& to_lower(std::string& m_str) {
            std::transform(m_str.begin(), m_str.end(), m_str.begin(), tolower);
            return m_str;
        }
        
        
        static  std::string to_lower(const std::string& m_str) {
            std::string s(m_str);
            std::transform(m_str.begin(), m_str.end(), s.begin(), tolower);
            return s;
        }
        
        static  std::string to_lower_copy(const std::string& m_str) {
            std::string s(m_str);
            std::transform(m_str.begin(), m_str.end(), s.begin(), tolower);
            return s;
        }
        
        /*!
        * @fn	static int compare(const std::string& a, const std::string& b, bool ignoreCase = false)
        *
        * @brief	比较两个字符串的大小.
        *
        * @param	a		  	Constant standard string&amp; to be compared.
        * @param	b		  	Constant standard string&amp; to be compared.
        * @param	ignoreCase	(Optional) bool to be compared.
        *
        * @return	Negative if 'a' is less than 'b', 0 if they are equal, or positive if it is greater.
        */
        
        static int compare(const std::string& a, const std::string& b,
                           bool ignoreCase = false) {
            std::string left = a;
            std::string right = b;
            
            if (ignoreCase) {
                amo::string_utils::to_upper(left);
                amo::string_utils::to_upper(right);
            }
            
            int len = (std::min)(left.size(), right.size());
            
            for (int i = 0; i < len; ++i) {
                if (left[i] < right[i]) {
                    return -1;
                }
                
                if (left[i] > right[i]) {
                    return 1;
                }
            }
            
            if (left.size() == right.size()) {
                return 0;
            }
            
            return left.size() < right.size();
        }
        
        // 移除字符串中的重复字符，可以指定字符
        static  std::string& unique(std::string& src,
                                    const std::string& target = " ") {
            auto iter =  std::unique(src.begin(), src.end(), [&](char l, char r) {
                if (target.find(l) != std::string::npos
                        && target.find(r) != std::string::npos) {
                    return true;
                }
                
                return false;
            });
            
            src.erase(iter, src.end());
            
            return src;
        }
        
        static std::string unique_copy(std::string& src,
                                       const std::string& target = " ") {
            std::string val = src;
            unique(val, target);
            return val;
        }
        
        //删除左边所有包含在target中的字符
        static  std::string& trim(std::string& src, const std::string& target = " ") {
            replace(src, target, "");
            return src;
        }
        
        static std::string trim_c(const std::string& src,
                                  const std::string& target = " ")   {
            std::string ret = src;
            replace(ret, target, "");
            return ret;
        }
        
        //删除左边所有包含在target中的字符
        static  std::string& trim_left(std::string& src,
                                       const std::string& target = " \t") {
            while (!src.empty() && (target.find(*src.begin()) != std::string::npos)) {
                src.erase(src.begin());
            }
            
            return src;
        }
        
        
        static  std::string trim_left_copy(const std::string& val,
                                           const std::string& target = " \t") {
            std::string src = val;
            
            while (!src.empty() && (target.find(*src.begin()) != std::string::npos)) {
                src.erase(src.begin());
            }
            
            return src;
        }
        
        // 删除左边所有字符，直到遇到所给的任意一字符为止
        static  std::string& trim_left_until(std::string& src,
                                             const std::string& target) {
            while (!src.empty() && (target.find(*src.begin()) == std::string::npos)) {
                src.erase(src.begin());
            }
            
            return src;
        }
        
        static  std::string& trim_left_until_copy(std::string& val,
                const std::string& target) {
            std::string src = val;
            trim_left_until(src, target);
            return src;
        }
        
        //删除右边所有包含在target中的字符
        static std::string& trim_right(std::string& src,
                                       const std::string& target = " \t") {
            while (!src.empty() && target.find(*src.rbegin()) != std::string::npos) {
                src.erase(--src.end());
            }
            
            return src;
        }
        
        static std::string trim_right_copy(const std::string& val,
                                           const std::string& target) {
            std::string src = val;
            
            while (!src.empty() && target.find(*src.rbegin()) != std::string::npos) {
                src.erase(--src.end());
            }
            
            return src;
        }
        
        // 删除右边所有字符，直到遇到 所给的任意一字符为止
        static  std::string& trim_right_until(std::string& src,
                                              const std::string& target) {
            while (!src.empty() && target.find(*src.rbegin()) == std::string::npos) {
                src.erase(--src.end());
            }
            
            return src;
        }
        
        static  std::string& trim_right_until_copy(std::string& val,
                const std::string& target) {
            std::string src = val;
            trim_right_until(src, target);
            return src;
        }
        
        
        //返回左边count个字符，count大于总长度则返回整个字符串
        static  std::string left(std::string& src, std::string::size_type count)   {
            return src.substr(0, count);
        }
        
        //返回右边count个字符，count大于总长度则返回整个字符串
        static  std::string right(std::string& src, std::string::size_type count)   {
            return src.substr(src.size() < count ? 0 : src.size() - count);
        }
        
        //忽略大小写判断两个字符串是否相等
        static int compare_no_case(const std::string& src, const std::string& rhs)   {
            return to_lower(src).compare(to_lower(rhs));
        }
        
        
        //判断字符串是否以制定字符串开头
        static bool begin_with(const std::string& str, const std::string& rhs)   {
            return str.find(rhs) == std::string::size_type(0);
        }
        
        //判断字符串是否以制定字符串结尾
        static bool end_with(const std::string& str, const std::string& rhs)   {
            if (rhs.size() > str.size()) {
                return false;
            }
            
            return str.compare(str.size() - rhs.size(), rhs.size(), rhs) == 0;
        }
        
        //判断字符串是否以制定字符串开头
        static bool start_with(const std::string& str, const std::string& rhs)   {
            return begin_with(str, rhs);
        }
        
        
        
        template<typename T>
        static T to_string(const std::string& atr)   {
            return T();
        }
        
        template<> std::string to_string<std::string>(const std::string& atr)   {
            return atr;
        }
        template<> std::wstring to_string<std::wstring>(const std::string&
                atr)   {
            return ansi_to_wide(atr);
        }
        
        
        template < typename T>
        static std::string to_string_impl(const T& val, int bit, int unit,
                                          char separator = ' ') {
                                          
                                          
            static  std::vector<char> hexes = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C',
                                                'D', 'E', 'F'
                                              };
                                              
            static std::vector<int8_t> masks = { 0, 0x1, 0x3, 0x7, 0xf};
            
            if (bit < 1 || bit >= masks.size()) {
                return "";
            }
            
            
            
            T mask = masks[bit];
            std::string retval;
            
            int len = sizeof(T) * 8;
            int residual = len % bit;
            
            if (residual > 0) {
                int index = (val & (mask << (len - residual))) >> (len - residual);
                retval += hexes[index];
            }
            
            len -= residual;
            
            
            for (int i = len - bit; i >= 0; i -= bit) {
                int index = (val & (mask << i)) >> i;
                retval += hexes[index];
                
                if (unit > 0 && (i % unit == 0)) {
                    retval += separator;
                }
            }
            
            return retval;
        }
        
        
        
        template < typename T>
        static std::string to_string(const T& val, int base = 10,
                                     int unit = 0,
                                     char separator = ' ') {
            switch (base) {
            case 2:
                return to_string_impl(val, 1, unit, separator);
                
            case 4:
                return to_string_impl(val, 2, unit, separator);
                
            case 8:
                return to_string_impl(val, 3, unit, separator);
                
            case 10:
                return std::to_string(val);
                
            case 16:
                return to_string_impl(val, 4, unit, separator);
                
            default:
                return std::to_string(val);
            }
            
            return "";
        }
        
        template < typename T>
        static std::string to_hex(const T& val, bool prefix = true) {
            if (prefix) {
                return "0x" + to_string(val, 16);
            } else {
                return to_string(val, 16);
            }
        }
        
        template < typename T>
        static std::string to_octal(const T& val, bool prefix = true) {
            if (prefix) {
                return "0" + to_string(val, 8);
            } else {
                return to_string(val, 8);
            }
        }
        
        template < typename T>
        static std::string to_binary(const T& val) {
            return to_string(val, 2);
        }
        
        
        
        // algin 对齐方式 0 左对齐 1 居中 2 右对齐
        // min_len 字符串最小长度，如果长度不够，将被齐，转为为0 表示以所给字符串长度为
        
        static void make_same_length(std::vector<std::string>& vec, int align = 0,
                                     int min_len = 0, char fill_char = ' ') {
            std::vector<int> arr;
            
            for (auto& p : vec) {
                arr.push_back(p.size());
            }
            
            if (min_len > 0) {
                arr.push_back(min_len);
            }
            
            auto iter = std::max_element(arr.begin(), arr.end());
            int minLen = *iter;
            
            
            
            if (align == 0) {
                for (auto& p : vec) {
                    std::string right_fill_str = "";
                    int fill_len = minLen - p.size();
                    
                    if (fill_len > 0) {
                        right_fill_str = std::string(fill_len, fill_char);
                    }
                    
                    p = p + right_fill_str;
                }
                
                
                
            } else if (align == 1) {
                for (auto& p : vec) {
                    std::string left_fill_str = "";
                    std::string right_fill_str = "";
                    int fill_len = minLen - p.size();
                    
                    if (fill_len > 0) {
                        int left_fill_len = fill_len / 2;
                        int right_fill_len = fill_len - left_fill_len;
                        left_fill_str = std::string(left_fill_len, fill_char);
                        right_fill_str = std::string(right_fill_len, fill_char);
                    }
                    
                    p = left_fill_str + p + right_fill_str;
                }
            } else if (align == 2) {
                for (auto& p : vec) {
                    std::string left_fill_str = "";
                    std::string right_fill_str = "";
                    int fill_len = minLen - p.size();
                    
                    if (fill_len > 0) {
                        left_fill_str = std::string(fill_len, fill_char);
                    }
                    
                    p = left_fill_str + p;
                }
            }
            
            return;
        }
        
        
        
    }
}



#endif //AMO_STRING_UTILS_D72E1F7C_8BFA_48BA_A990_6129BC202A86_HPP__
