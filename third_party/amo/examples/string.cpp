#include <iostream>

#include <gtest/gtest.h>
#include <amo/string.hpp>
#include <amo/json.hpp>
#include <unordered_map>

namespace {

    static const std::string CONST_STRING =
        "中文路徑중국어 경로語のパスالمسار الصينية Chinese path2.txt";
        
    static std::string get_test_u8str() {
        std::string test_str =
            u8"中文路徑중국어 경로語のパスالمسار الصينية Chinese path2.txt";
        return test_str;
    }
    
    static std::string get_test_str() {
        std::string test_str = amo::string_utils::utf8_to_ansi(get_test_u8str());
        return test_str;
    }
    
    static std::string get_test_str2() {
        std::string test_str =  "abcdefg";
        return test_str;
    }
    
    
    
    
    static std::wstring get_test_wstr() {
        std::string str = get_test_u8str();
        return amo::string_utils::utf8_to_wide(str);
    }
    
    static std::wstring get_test_wstr2() {
        std::string str = get_test_str2();
        return amo::string_utils::utf8_to_wide(str);
    }
    
    static std::string get_locale_str() {
        if (DEFAULT_STRING_UTF8) {
            return get_test_u8str();
        } else {
            return get_test_str();
        }
        
    }
#ifdef UNICODE
    static std::wstring get_unicode_str() {
        return get_test_wstr();
    }
#else
    static std::string get_unicode_str() {
        return get_test_str();
    }
#endif
    
    static bool is_eq(const std::wstring& s1, const std::wstring& s2) {
        if (s1.size() != s2.size()) {
            return false;
        }
        
        std::string str1 = amo::string_utils::wide_to_utf8(s1);
        std::string str2 = amo::string_utils::wide_to_utf8(s2);
        return str1 == str2;
    }
    
    static  bool true_or_false() {
        std::string from_encoding = std::use_facet<boost::locale::info>
                                    (amo::string_utils::get_system_locale()).encoding();
                                    
        if (from_encoding.find("utf") != -1 || from_encoding.find("UTF") != -1) {
            return true;
        } else {
            return false;
        }
        
    }
}

TEST(string, constructor) {
    {
        std::string str = get_test_str();
        std::wstring wstr = get_test_wstr();
        amo::string s1;
        amo::string s2(str, false);
        amo::string s3(str.c_str(), false);
        amo::string s4(wstr);
        amo::string s5(wstr.c_str());
        amo::string s6(s5);
        amo::string s7 = s6;
        amo::u8string s8 = s6.to_wide();
        
        EXPECT_EQ(is_eq(s7.to_wide(), wstr), true_or_false());
        //amo::string s6 = get_test_str();
    }
    
    {
        std::string str = get_test_u8str();
        std::wstring wstr = get_test_wstr();
        amo::u8string s1;
        amo::u8string s2(str, true);
        amo::u8string s3(str.c_str(), true);
        amo::u8string s4(wstr);
        amo::u8string s5(wstr.c_str());
        amo::u8string s6(s5);
        amo::u8string s7 = s6;
        amo::string s8 = s6.to_wide();
        EXPECT_EQ(is_eq(s7.to_wide(), wstr), true);
        
    }
    
    //amo::string s2 = "234";
    //s2 = "111";
    
}

TEST(string, from_constant) {

    {
        amo::string s1 = amo::string::from_constant(CONST_STRING);
        EXPECT_EQ(is_eq(s1.to_wide(), get_test_wstr()), true_or_false());
        
        amo::string s2 = amo::string::from_constant(get_test_str2());
        EXPECT_EQ(s2.str(), get_test_str2());
    }
    
    {
        amo::u8string s1 = amo::u8string::from_constant("abcdef");
        EXPECT_EQ(is_eq(s1.to_wide(), L"abcdef"), true);
    }
}

TEST(string, from_utf8) {
    {
        amo::string s1 = amo::string::from_utf8(u8"abcdef");
        EXPECT_EQ(is_eq(s1.to_wide(), L"abcdef"), true);
        amo::string s2 = amo::string::from_utf8(get_test_u8str());
        EXPECT_EQ(is_eq(s2.to_wide(), get_test_wstr()), true_or_false());
    }
    
    {
        amo::u8string s1 = amo::u8string::from_utf8(u8"中文字符串");
        EXPECT_EQ(is_eq(s1.to_wide(), L"中文字符串"), true);
    }
}

TEST(string, from_unicode) {
    {
        amo::string s1 = amo::string::from_unicode(get_test_wstr());
        EXPECT_EQ(is_eq(s1.to_wide(), get_test_wstr()), true_or_false());
    }
    
    {
        amo::u8string s1 = amo::u8string::from_unicode(L"中文字符串");
        EXPECT_EQ(is_eq(s1.to_wide(), L"中文字符串"), true);
    }
}

TEST(string, from_ansi) {
    {
        amo::string s1 = amo::string::from_ansi(get_test_str());
        EXPECT_EQ(is_eq(s1.to_wide(), get_test_wstr()), true_or_false());
        
        amo::string s2 = amo::string::from_ansi(get_test_str2());
        EXPECT_EQ(s2.str(), get_test_str2());
    }
    
    {
        amo::u8string s1 = amo::u8string::from_ansi(get_test_str());
        EXPECT_EQ(is_eq(s1.to_wide(), get_test_wstr()), true_or_false());
    }
}


TEST(string, to_ansi) {
    {
        amo::string s1(get_test_str(), false);
        EXPECT_EQ(s1.to_ansi(), get_test_str());
        
        amo::string s2(get_test_u8str(), true);
        EXPECT_EQ(s2.to_ansi(), get_test_str());
        
        amo::string s3(get_test_wstr());
        EXPECT_EQ(s3.to_ansi(), get_test_str());
    }
    
    {
        amo::u8string s1(get_test_str(), false);
        EXPECT_EQ(s1.to_ansi(), get_test_str());
        
        amo::u8string s2(get_test_u8str(), true);
        EXPECT_EQ(s2.to_ansi(), get_test_str());
        
        amo::u8string s3(get_test_wstr());
        EXPECT_EQ(s3.to_ansi(), get_test_str());
    }
}

TEST(string, to_wide) {
    {
        amo::string s1(get_test_str(), false);
        EXPECT_EQ(is_eq(s1.to_wide(), get_test_wstr()), true_or_false());
        
        amo::string s2(get_test_u8str(), true);
        EXPECT_EQ(is_eq(s2.to_wide(), get_test_wstr()), true_or_false());
        
        amo::string s3(get_test_wstr());
        EXPECT_EQ(is_eq(s3.to_wide(), get_test_wstr()), true_or_false());
    }
    
    {
        amo::u8string s1(get_test_str(), false);
        EXPECT_EQ(is_eq(s1.to_wide(), get_test_wstr()), true_or_false());
        
        amo::u8string s2(get_test_u8str(), true);
        EXPECT_EQ(is_eq(s2.to_wide(), get_test_wstr()), true);
        
        amo::u8string s3(get_test_wstr());
        EXPECT_EQ(is_eq(s3.to_wide(), get_test_wstr()), true);
    }
}

TEST(string, to_utf8) {
    {
        amo::string s1(get_test_str(), false);
        EXPECT_EQ(s1.to_utf8() == get_test_u8str(), true_or_false());
        
        amo::string s2(get_test_u8str(), true);
        EXPECT_EQ(s2.to_utf8() ==  get_test_u8str(), true_or_false());
        
        amo::string s3(get_test_wstr());
        EXPECT_EQ(s3.to_utf8() == get_test_u8str(), true_or_false());
    }
    
    {
        amo::u8string s1(get_test_str(), false);
        EXPECT_EQ(s1.to_utf8() == get_test_u8str(), true_or_false());
        
        amo::u8string s2(get_test_u8str(), true);
        EXPECT_EQ(s2.to_utf8(), get_test_u8str());
        
        amo::u8string s3(get_test_wstr());
        EXPECT_EQ(s3.to_utf8(), get_test_u8str());
    }
}


TEST(string, to_locale) {
    {
        amo::string s1(get_test_str(), false);
        EXPECT_EQ(s1.to_locale(), get_locale_str());
        
        amo::string s2(get_test_u8str(), true);
        EXPECT_EQ(s2.to_locale(), get_locale_str());
        
        amo::string s3(get_test_wstr());
        EXPECT_EQ(s3.to_locale(), get_locale_str());
    }
    
    {
        amo::u8string s1(get_test_str(), false);
        EXPECT_EQ(s1.to_locale(), get_locale_str());
        
        amo::u8string s2(get_test_u8str(), true);
        EXPECT_EQ(s2.to_locale(), get_locale_str());
        
        amo::u8string s3(get_test_wstr());
        EXPECT_EQ(s3.to_locale(), get_locale_str());
    }
    
}

TEST(string, to_string) {
    {
        amo::string s1(get_test_str(), false);
        EXPECT_EQ(s1.to_string<std::string>(), get_test_str());
        
        amo::string s2(get_test_u8str(), true);
        EXPECT_EQ(s2.to_string<std::string>(), get_test_str());
        
        amo::string s3(get_test_wstr());
        EXPECT_EQ(s3.to_string<std::string>(), get_test_str());
    }
    
    {
        amo::u8string s1(get_test_str(), false);
        EXPECT_EQ(s1.to_string<std::string>() == get_test_u8str(), true_or_false());
        
        amo::u8string s2(get_test_u8str(), true);
        EXPECT_EQ(s2.to_string<std::string>(), get_test_u8str());
        
        amo::u8string s3(get_test_wstr());
        EXPECT_EQ(s3.to_string<std::string>(), get_test_u8str());
    }
    
    {
    
    
        amo::string s1(get_test_str(), false);
        EXPECT_EQ(is_eq(s1.to_string<std::wstring>(), get_test_wstr()),
                  true_or_false());
                  
        amo::string s2(get_test_u8str(), true);
        EXPECT_EQ(is_eq(s2.to_string<std::wstring>(), get_test_wstr()),
                  true_or_false());
                  
        amo::string s3(get_test_wstr());
        EXPECT_EQ(is_eq(s3.to_string<std::wstring>(), get_test_wstr()),
                  true_or_false());
                  
        amo::string s4(get_test_str2(), false);
        EXPECT_EQ(is_eq(s4.to_string<std::wstring>(), get_test_wstr2()), true);
    }
    
    {
        amo::u8string s1(get_test_str(), false);
        EXPECT_EQ(s1.to_string<std::wstring>() == get_test_wstr(), true_or_false());
        
        amo::u8string s2(get_test_u8str(), true);
        EXPECT_EQ(s2.to_string<std::wstring>(), get_test_wstr());
        
        amo::u8string s3(get_test_wstr());
        EXPECT_EQ(s3.to_string<std::wstring>(), get_test_wstr());
    }
}

TEST(string, operatorAdd) {
    {
        std::string rhs = get_test_str2() + "0";
        amo::string s1(get_test_str2(), false);
        s1 = s1 +  "0";
        EXPECT_EQ(s1.to_ansi(), rhs);
        
        s1 = s1 +  std::to_string(1);
        rhs += std::to_string(1);
        EXPECT_EQ(s1.to_ansi(), rhs);
        
        s1 = s1 +  s1;
        rhs += rhs;
        EXPECT_EQ(s1.to_ansi(), rhs);
        
        s1 = s1 +  get_test_str();
        rhs += get_test_str();
        EXPECT_EQ(s1.to_ansi(), rhs);
        
        s1 = s1 +  get_test_u8str();
        rhs += get_test_u8str();
        EXPECT_EQ(s1.to_ansi(), rhs);
        
        s1 = s1 +  get_test_wstr();
        rhs += amo::string_utils::wide_to_ansi(get_test_wstr());
        EXPECT_EQ(s1.to_ansi(), rhs);
        
        s1 = s1 +  L"的的的1111";
        rhs += amo::string_utils::wide_to_ansi(L"的的的1111");
        EXPECT_EQ(s1.to_ansi(), rhs);
        
    }
    
    {
        std::string rhs = get_test_str2() + "0";
        amo::string s1(get_test_str2(), false);
        s1 += "0";
        EXPECT_EQ(s1.to_ansi(), rhs);
        
        s1 += std::to_string(1);
        rhs += std::to_string(1);
        EXPECT_EQ(s1.to_ansi(), rhs);
        
        s1 += s1;
        rhs += rhs;
        EXPECT_EQ(s1.to_ansi(), rhs);
        
        s1 += get_test_str();
        rhs += get_test_str();
        EXPECT_EQ(s1.to_ansi(), rhs);
        
        s1 += get_test_u8str();
        rhs += get_test_u8str();
        EXPECT_EQ(s1.to_ansi(), rhs);
        
        s1 += get_test_wstr();
        rhs += amo::string_utils::wide_to_ansi(get_test_wstr());
        EXPECT_EQ(s1.to_ansi(), rhs);
        
        s1 += L"的的的1111";
        rhs += amo::string_utils::wide_to_ansi(L"的的的1111");
        EXPECT_EQ(s1.to_ansi(), rhs);
        
    }
    
    {
        std::string rhs = get_test_str2() + "0";
        amo::u8string s1(get_test_str2(), false);
        s1 = s1 + "0";
        EXPECT_EQ(s1.to_utf8(), rhs);
        
        s1 = s1 + std::to_string(1);
        rhs += std::to_string(1);
        EXPECT_EQ(s1.to_utf8(), rhs);
        
        s1 = s1 + s1;
        rhs += rhs;
        EXPECT_EQ(s1.to_utf8(), rhs);
        
        s1 = s1 + get_test_str();
        rhs += get_test_str();
        EXPECT_EQ(s1.to_utf8(), rhs);
        
        s1 = s1 + get_test_u8str();
        rhs += get_test_u8str();
        EXPECT_EQ(s1.to_utf8(), rhs);
        
        s1 = s1 + get_test_wstr();
        rhs += amo::string_utils::wide_to_utf8(get_test_wstr());
        EXPECT_EQ(s1.to_utf8(), rhs);
        
        s1 = s1 + L"的的的1111";
        rhs += amo::string_utils::wide_to_utf8(L"的的的1111");
        EXPECT_EQ(s1.to_utf8(), rhs);
        
        s1 = s1 + get_test_str();
        rhs += get_test_u8str();
        EXPECT_EQ(s1.to_utf8() == rhs, true_or_false());
        
    }
    
    {
        std::string rhs = get_test_str2() + "0";
        amo::u8string s1(get_test_str2(), false);
        s1 += "0";
        EXPECT_EQ(s1.to_utf8(), rhs);
        
        s1 += std::to_string(1);
        rhs += std::to_string(1);
        EXPECT_EQ(s1.to_utf8(), rhs);
        
        s1 += s1;
        rhs += rhs;
        EXPECT_EQ(s1.to_utf8(), rhs);
        
        s1 += get_test_str();
        rhs += get_test_str();
        EXPECT_EQ(s1.to_utf8(), rhs);
        
        s1 += get_test_u8str();
        rhs += get_test_u8str();
        EXPECT_EQ(s1.to_utf8(), rhs);
        
        s1 += get_test_wstr();
        rhs += amo::string_utils::wide_to_utf8(get_test_wstr());
        EXPECT_EQ(s1.to_utf8(), rhs);
        
        s1 += L"的的的1111";
        rhs += amo::string_utils::wide_to_utf8(L"的的的1111");
        EXPECT_EQ(s1.to_utf8(), rhs);
        
        s1 += get_test_str();
        rhs += get_test_u8str();
        EXPECT_EQ(s1.to_utf8() == rhs, true_or_false());
        
    }
    
}

TEST(string, operatorNE) {
    {
        amo::string s1(get_test_str(), false);
        amo::string s2(get_test_u8str(), true);
        
        EXPECT_EQ(s1 != get_test_u8str(), !true_or_false());
        EXPECT_EQ(s1 != get_test_str(), false);
        EXPECT_EQ(s1 !=  get_test_wstr(), false);
        EXPECT_EQ(s1 != s1, false);
        EXPECT_EQ(s1 != s2, false);
    }
    
    {
        amo::u8string s1(get_test_str(), false);
        amo::u8string s2(get_test_u8str(), true);
        
        EXPECT_EQ(s1 != get_test_u8str(), !true_or_false());
        EXPECT_EQ(s2 != get_test_u8str(), false);
        EXPECT_EQ(!is_eq(s1.to_wide(), get_test_wstr()), !true_or_false());
        EXPECT_EQ(s2 != get_test_wstr(), false);
        EXPECT_EQ(s1 != s1, false);
        EXPECT_EQ(s1 != s2, !true_or_false());
    }
}


TEST(string, operatorEQ) {
    {
        amo::string s1(get_test_str(), false);
        amo::string s2(get_test_u8str(), true);
        
        EXPECT_EQ(s1 == get_test_u8str(), true_or_false());
        EXPECT_EQ(s1 == get_test_str(), true);
        EXPECT_EQ(s1 == get_test_wstr(), true);
        EXPECT_EQ(s1 == s1, true);
        EXPECT_EQ(s1 == s2, true);
    }
    
    {
        amo::u8string s1(get_test_str(), false);
        amo::u8string s2(get_test_u8str(), true);
        
        EXPECT_EQ(s1 == get_test_u8str(), true_or_false());
        EXPECT_EQ(s2 == get_test_u8str(), true);
        EXPECT_EQ(s1 == get_test_wstr(), true_or_false());
        EXPECT_EQ(s2 == get_test_wstr(), true);
        EXPECT_EQ(s1 == s1, true);
        EXPECT_EQ(s1 == s2, true_or_false());
    }
}

TEST(string, split) {
    {
        amo::string s1("aaabbbaaabbbaaabbbaaa", false);
        // bbb bbb bbb
        EXPECT_EQ(s1.split(L"a").size(), 3);
        // abbb abbb abbb a
        EXPECT_EQ(s1.split(L"aa").size(), 4);
        // bbb bbb bbb
        EXPECT_EQ(s1.split(L"aaa").size(), 3);
        
        // "" "" "" bbb "" "" bbb  "" "" bbb "" ""
        EXPECT_EQ(s1.split(L"a", false).size(), 12);
        // ""  abbb abbb  abbb a
        EXPECT_EQ(s1.split(L"aa", false).size(), 5);
        // ""  bbb bbb bbb
        EXPECT_EQ(s1.split(L"aaa", false).size(), 4);
        
    }
    
    {
        amo::u8string s1(
            u8"中中中文文文中中中文文文中中中文文文中中中", true);
        // 文文文 文文文 文文文
        EXPECT_EQ(s1.split(L"中").size(), 3);
        // 中文文文 中文文文 中文文文 中
        EXPECT_EQ(s1.split(L"中中").size(), 4);
        // 文文文 文文文 文文文
        EXPECT_EQ(s1.split(L"中中中").size(), 3);
        
        // "" "" "" 文文文 "" "" 文文文  "" "" 文文文 "" ""
        EXPECT_EQ(s1.split(L"中", false).size(), 12);
        // ""  中文文文 中文文文  中文文文 中
        EXPECT_EQ(s1.split(L"中中", false).size(), 5);
        // ""  文文文 文文文 文文文
        EXPECT_EQ(s1.split(L"中中中", false).size(), 4);
        
    }
}

TEST(string, replace) {
    {
        amo::string s1("aaabbbaaabbbaaabbbaaa", false);
        
        EXPECT_EQ(s1.replace(L"aa", L"") == "abbbabbbabbba", true);
        EXPECT_EQ(s1.replace(L"bbb", L"") == "aaaa", true);
    }
    {
        const amo::string s1("aaabbbaaabbbaaabbbaaa", false);
        
        EXPECT_EQ(s1.replace(L"aa", L"") == "abbbabbbabbba", true);
        EXPECT_EQ(s1.replace(L"bbb", L"") == "aaaaaaaaaaaa", true);
    }
    
    {
        amo::u8string s1(
            u8"中中中文文文中中中文文文中中中文文文中中中", true);
        EXPECT_EQ(s1.replace(L"中中",
                             L"") == u8"中文文文中文文文中文文文中", true);
        EXPECT_EQ(s1.replace(L"文文文", L"") == u8"中中中中", true);
        
    }
    
    {
        const amo::u8string s1(
            u8"中中中文文文中中中文文文中中中文文文中中中", true);
        EXPECT_EQ(s1.replace(L"中中",
                             L"") == u8"中文文文中文文文中文文文中", true);
        EXPECT_EQ(s1.replace(L"文文文",
                             L"") == u8"中中中中中中中中中中中中", true);
                             
    }
}

TEST(string, to_upper) {
    {
        amo::string s1("aaabbbaaabbbaaabbbaaa", false);
        EXPECT_EQ(s1.to_upper() == "AAABBBAAABBBAAABBBAAA", true);
    }
    
    {
        const amo::string s1("aaabbbaaabbbaaabbbaaa", false);
        auto val = s1.to_upper();
        EXPECT_EQ(s1 == "aaabbbaaabbbaaabbbaaa", true);
        EXPECT_EQ(val == "AAABBBAAABBBAAABBBAAA", true);
    }
    
    {
        amo::u8string s1("aaabbbaaabbbaaabbbaaa", false);
        EXPECT_EQ(s1.to_upper() == "AAABBBAAABBBAAABBBAAA", true);
    }
    
    {
        const amo::u8string s1("aaabbbaaabbbaaabbbaaa", false);
        auto val = s1.to_upper();
        EXPECT_EQ(s1 == "aaabbbaaabbbaaabbbaaa", true);
        EXPECT_EQ(val == "AAABBBAAABBBAAABBBAAA", true);
    }
}

TEST(string, to_lower) {
    {
        amo::string s1("aaabbbaaabbbaaabbbaaA", false);
        EXPECT_EQ(s1.to_lower() == "aaabbbaaabbbaaabbbaaa", true);
    }
    
    {
        const amo::string s1("aaabbbaaabbbaaabbbaaA", false);
        auto val = s1.to_lower();
        EXPECT_EQ(s1 == "aaabbbaaabbbaaabbbaaA", true);
        EXPECT_EQ(val == "aaabbbaaabbbaaabbbaaa", true);
    }
    
    {
        amo::u8string s1("aaabbbaaabbbaaabbbaaA", false);
        EXPECT_EQ(s1.to_lower() == "aaabbbaaabbbaaabbbaaa", true);
    }
    
    {
        const amo::u8string s1("aaabbbaaabbbaaabbbaaA", false);
        auto val = s1.to_lower();
        EXPECT_EQ(s1 == "aaabbbaaabbbaaabbbaaA", true);
        EXPECT_EQ(val == "aaabbbaaabbbaaabbbaaa", true);
    }
}

TEST(string, trim) {
    {
        amo::string s1("aaabbbaaabbbaaabbbaaA", false);
        EXPECT_EQ(s1.trim(L"a") == "bbbbbbbbbA", true);
    }
    
    {
        const amo::string s1("aaabbbaaabbbaaabbbaaA", false);
        auto val = s1.trim(L"a");
        EXPECT_EQ(s1 == "aaabbbaaabbbaaabbbaaA", true);
        EXPECT_EQ(val == "bbbbbbbbbA", true);
    }
    
    {
        amo::u8string s1("aaabbbaaabbbaaabbbaaA", false);
        EXPECT_EQ(s1.trim(L"a") == "bbbbbbbbbA", true);
    }
    
    {
        const amo::u8string s1("aaabbbaaabbbaaabbbaaA", false);
        auto val = s1.trim(L"a");
        EXPECT_EQ(s1 == "aaabbbaaabbbaaabbbaaA", true);
        EXPECT_EQ(val == "bbbbbbbbbA", true);
    }
    
    {
        amo::u8string s1(
            u8"中中中文文文中中中文文文中中中文文文中中中", true);
        EXPECT_EQ(s1.trim(L"中文") ==
                  u8"中中文文中中文文中中文文中中中", true);
        EXPECT_EQ(s1.trim(L"中") == u8"文文文文文文", true);
        EXPECT_EQ(s1.trim(L"中文") == u8"文文文文文文", true);
    }
}

TEST(string, trim_left) {
    {
        amo::string s1("aaabbbaaabbbaaabbbaaA", false);
        EXPECT_EQ(s1.trim_left(L"a") == "bbbaaabbbaaabbbaaA", true);
        EXPECT_EQ(s1.trim_left(L"ab") == "A", true);
    }
    
    {
        amo::u8string s1(
            u8"中中中文文文中中中文文文中中中文文文中中中", true);
        EXPECT_EQ(s1.trim_left(L"中") ==
                  u8"文文文中中中文文文中中中文文文中中中", true);
        EXPECT_EQ(s1.trim_left(L"中文") == u8"", true);
    }
}

TEST(string, trim_right) {
    {
        amo::string s1("aaabbbaaabbbaaabbbaaA", false);
        EXPECT_EQ(s1.trim_right(L"a") == "aaabbbaaabbbaaabbbaaA", true);
        EXPECT_EQ(s1.trim_right(L"Aa") == "aaabbbaaabbbaaabbb", true);
    }
    
    {
        amo::u8string s1(
            u8"中中中文文文中中中文文文中中中文文文中中中", true);
        EXPECT_EQ(s1.trim_right(L"中") ==
                  u8"中中中文文文中中中文文文中中中文文文", true);
        EXPECT_EQ(s1.trim_right(L"中文") == u8"", true);
    }
}


TEST(string, left) {
    {
        amo::string s1("aaabbbaaabbbaaabbbaaA", false);
        EXPECT_EQ(s1.left(3) == "aaa", true);
    }
    
    {
        amo::u8string s1(
            u8"中中中文文文中中中文文文中中中文文文中中中", true);
        amo::u8string s2(u8"中中中", true);
        EXPECT_EQ(s1.left(s2.size()) == u8"中中中", true);
    }
}


TEST(string, right) {
    {
        amo::string s1("aaabbbaaabbbaaabbbaaA", false);
        EXPECT_EQ(s1.right(3) == "aaA", true);
    }
    
    {
        amo::u8string s1(
            u8"中中中文文文中中中文文文中中中文文文中中中", true);
        amo::u8string s2(u8"中中中", true);
        EXPECT_EQ(s1.right(s2.size()) == u8"中中中", true);
    }
}

TEST(string, begin_with) {
    {
        amo::string s1("aaabbbaaabbbaaabbbaaA", false);
        EXPECT_EQ(s1.begin_with(L"aa1a"), false);
        EXPECT_EQ(s1.begin_with(L"aaa"), true);
    }
    
    {
        amo::u8string s1(
            u8"中中中文文文中中中文文文中中中文文文中中中", true);
        EXPECT_EQ(s1.begin_with(L"中中中"), true);
        EXPECT_EQ(s1.begin_with(L"aaa"), false);
    }
}

TEST(string, end_with) {
    {
        amo::string s1("aaabbbaaabbbaaabbbaaA", false);
        EXPECT_EQ(s1.end_with(L"aa1a"), false);
        EXPECT_EQ(s1.end_with(L"aaA"), true);
    }
    
    {
        amo::u8string s1(
            u8"中中中文文文中中中文文文中中中文文文中中中", true);
        EXPECT_EQ(s1.end_with(L"中中中"), true);
        EXPECT_EQ(s1.end_with(L"aaa"), false);
    }
}

TEST(string, to_number) {
    {
        amo::string s1(std::to_string((std::numeric_limits<int8_t>::max)()), false);
        auto ssss = s1.lexical_cast<int8_t>();
        EXPECT_EQ(s1.to_number<int8_t>(), (std::numeric_limits<int8_t>::max)());
        
        amo::string s2(std::to_string((std::numeric_limits<int16_t>::max)()), false);
        EXPECT_EQ(s2.to_number<int16_t>(), (std::numeric_limits<int16_t>::max)());
        
        amo::string s3(std::to_string((std::numeric_limits<int32_t>::max)()), false);
        EXPECT_EQ(s3.to_number<int32_t>(), (std::numeric_limits<int32_t>::max)());
        
        amo::string s4(std::to_string((std::numeric_limits<int64_t>::max)()), false);
        EXPECT_EQ(s4.to_number<int64_t>(), (std::numeric_limits<int64_t>::max)());
        
        amo::string s5(std::to_string((std::numeric_limits<double>::max)()), false);
        EXPECT_EQ(s5.to_number<double>(), (std::numeric_limits<double>::max)());
        
        amo::string s6(std::to_string((std::numeric_limits<float>::max)()), false);
        EXPECT_EQ(s6.to_number<float>(), (std::numeric_limits<float>::max)());
        
        amo::string s7(" 123a", false);
        EXPECT_EQ(s7.to_number<int32_t>(), 123);
        
    }
    
    {
        //11 1100 0100 1110 0110 1111 1111
        amo::string s1("  0111100010011100110111111113", false);
        //amo::string s1("1111", false);
        EXPECT_EQ(s1.to_number<int8_t>(2), (int8_t)0xff);
        EXPECT_EQ(s1.to_number<int16_t>(2), (int16_t)0xe6ff);
        EXPECT_EQ(s1.to_number<int32_t>(2), 0x3c4e6ff);
        
    }
    
    {
        //001 010 011 100 101 110 111
        // 101 0011 1001 0111 0111
        amo::string s1("12345678", false);
        
        EXPECT_EQ(s1.to_number<int8_t>(8), 0x77);
        EXPECT_EQ(s1.to_number<int16_t>(8), 0x3977);
        EXPECT_EQ(s1.to_number<int32_t>(8), 0x53977);
        
    }
    
    {
        amo::string s1("7abcdef", false);
        EXPECT_EQ(s1.to_number<int8_t>(16), (int8_t)0xef);
        EXPECT_EQ(s1.to_number<int16_t>(16), (int16_t)0xcdef);
        EXPECT_EQ(s1.to_number<uint8_t>(16), 0xef);
        EXPECT_EQ(s1.to_number<uint16_t>(16), 0xcdef);
        EXPECT_EQ(s1.to_number<int32_t>(16), 0x7abcdef);
        
    }
    
    
    
}


TEST(string, from_number) {
    //dec
    {
    
        amo::string s1 = amo::string::from_number((std::numeric_limits<int8_t>::max)(),
                         2);
        amo::string s2 = amo::string::from_number((std::numeric_limits<int8_t>::max)(),
                         8);
        amo::string s3 = amo::string::from_number((std::numeric_limits<int8_t>::max)(),
                         10);
        amo::string s4 = amo::string::from_number((std::numeric_limits<int8_t>::max)(),
                         16);
        EXPECT_EQ(s1 == L"1111111", true);
        EXPECT_EQ(s2 == L"177", true);
        EXPECT_EQ(s3 == L"127", true);
        EXPECT_EQ(s4 == L"7f", true);
    }
    
    {
    
        amo::string s1 = amo::string::from_number((std::numeric_limits<int8_t>::min)(),
                         2);
        amo::string s2 = amo::string::from_number((int8_t) - 127, 2);
        amo::string s3 = amo::string::from_number((int16_t) - 127, 2);
        amo::string s4 = amo::string::from_number((int32_t) - 127, 2);
        amo::string s5 = amo::string::from_number((int64_t) - 127, 2);
        
        EXPECT_EQ(s1 == L"10000000", true);
        EXPECT_EQ(s2 == L"10000001", true);
        EXPECT_EQ(s3 == L"1111111110000001", true);
        EXPECT_EQ(s4 == L"11111111111111111111111110000001", true);
        EXPECT_EQ(s5 ==
                  L"1111111111111111111111111111111111111111111111111111111110000001", true);
    }
    
    {
    
        amo::string s1 = amo::string::from_number((std::numeric_limits<int8_t>::min)(),
                         8);
        amo::string s2 = amo::string::from_number((int8_t) - 127, 8);
        amo::string s3 = amo::string::from_number((int16_t) - 127, 8);
        amo::string s4 = amo::string::from_number((int32_t) - 127, 8);
        amo::string s5 = amo::string::from_number((int64_t) - 127, 8);
        
        EXPECT_EQ(s1 == L"200", true);
        EXPECT_EQ(s2 == L"201", true);
        EXPECT_EQ(s3 == L"177601", true);
        EXPECT_EQ(s4 == L"37777777601", true);
        EXPECT_EQ(s5 == L"1777777777777777777601", true);
    }
    
    {
        //11 1100 0100 1110 0110 1111 1111
        //11 110 001 001 110 011 011 111 111
        //amo::string s1("  0111100010011100110111111113", false);
        amo::string s1 = amo::string::from_number(0x3c4e6ff, 2);
        amo::string s2 = amo::string::from_number(0x3c4e6ff, 8);
        amo::string s3 = amo::string::from_number(0x3c4e6ff, 10);
        amo::string s4 = amo::string::from_number(0x3c4e6ff, 16);
        EXPECT_EQ(s1 == L"11110001001110011011111111", true);
        EXPECT_EQ(s2 == L"361163377", true);
        EXPECT_EQ(s3 == L"63235839", true);
        EXPECT_EQ(s4 == L"3c4e6ff", true);
        
        EXPECT_EQ(s1.to_number<int8_t>(2), (int8_t)0xff);
        EXPECT_EQ(s1.to_number<int16_t>(2), (int16_t)0xe6ff);
        EXPECT_EQ(s1.to_number<int32_t>(2), 0x3c4e6ff);
        
    }
    
    {
        amo::string s1 = amo::string::from_number(-0.323);
        amo::string s2 = amo::string::from_number(111.3333);
        amo::string s3 = amo::string::from_number(12.32e3);
        amo::string s4 = amo::string::from_number(0x3c4e6ff, 16);
        
    }
    
    
}

TEST(string, format) {
    {
        amo::string s1 = L"{aa}+{bb}={cc}";
        amo::json json;
        json.put("aa", 3);
        json.put("bb", 4);
        json.put("cc", 7);
        EXPECT_EQ(s1.format(json), "3+4=7");
    }
    
    {
        amo::u8string s1 = L"{aa}+{bb}={cc}";
        amo::u8json json;
        json.put("aa", u8"中");
        json.put("bb", u8"文");
        json.put("cc", u8"中文");
        EXPECT_EQ(s1.format(json), u8"中+文=中文");
    }
    
    
    {
        amo::string s1 = L"{aa}+{bb}={cc}";
        EXPECT_EQ(s1.format({ { "aa", 3 }, { "bb", 4 }, { "cc", "7" } }), "3+4=7");
    }
    
    {
        std::unordered_map<std::string, std::string> map({ { "aa", "3" }, { "bb", "4" }, { "cc", "7" } });
        amo::string s1 = L"{aa}+{bb}={cc}";
        EXPECT_EQ(s1.format(map), "3+4=7");
    }
    
}