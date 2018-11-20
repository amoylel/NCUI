#include <iostream>
#include <regex>
#include <gtest/gtest.h>

#include <amo/config.hpp>
#include <amo/path.hpp>
#include <amo/string.hpp>
#include <utility>




namespace {


    static std::string get_u8path_str() {
        std::string test_str =
            u8"中文路徑중국어 경로語のパスالمسار الصينية Chinese path2.txt";
        return test_str;
    }
    
    static std::string get_u8path_dir() {
        std::string test_str = u8"DIR中文路徑중국어DIR";
        return test_str;
    }
    
    static std::string get_ansi_path_dir() {
        std::string test_str = amo::string_utils::utf8_to_ansi(get_u8path_dir());
        return test_str;
    }
    
    
    static std::string get_ansi_path_str() {
        std::string test_str = amo::string_utils::utf8_to_ansi(get_u8path_str());
        return test_str;
    }
    
    static std::string get_ansi_path_str2() {
        std::string test_str = "abcdefg.txt";
        return test_str;
    }
    
    static std::wstring get_path_wstr() {
        std::string str = get_u8path_str();
        return amo::string_utils::utf8_to_wide(str);
    }
    
    static std::wstring get_path_wstr2() {
        std::string str = get_ansi_path_str2();
        return amo::string_utils::utf8_to_wide(str);
    }
    
    static std::string get_locale_path_str() {
        if (DEFAULT_STRING_UTF8) {
            return get_u8path_str();
        } else {
            return get_ansi_path_str();
        }
        
    }
#ifdef UNICODE
    static std::wstring get_unicode_str() {
        return get_path_wstr();
    }
#else
    static std::string get_unicode_str() {
        return get_ansi_path_str();
    }
#endif
    
    static  bool true_or_false() {
        std::string from_encoding = std::use_facet<boost::locale::info>
                                    (amo::string_utils::get_system_locale()).encoding();
                                    
        if (from_encoding.find("utf") != -1 || from_encoding.find("UTF") != -1) {
            return false;
        } else {
            return true;
        }
        
    }
    static  bool win_or_linux() {
        std::string from_encoding = std::use_facet<boost::locale::info>
                                    (amo::string_utils::get_system_locale()).encoding();
                                    
        if (from_encoding.find("utf") != -1 || from_encoding.find("UTF") != -1) {
            return false;
        } else {
            return true;
        }
        
    }
}

TEST(path, constructor) {
    {
        amo::string str(get_ansi_path_str(), false);
        amo::path p1;
        amo::path p2(get_ansi_path_str());
        amo::path p3(get_ansi_path_str2());
        amo::path p4(get_path_wstr());
        amo::path p5(get_path_wstr2());
        amo::path p6(str);
        amo::path p7(p5);
        amo::path p8 = boost::filesystem::path(get_path_wstr2());
        
        EXPECT_EQ(false, p1.exists());
        EXPECT_EQ(false, p2.string().empty());
        EXPECT_EQ(false, p3.string().empty());
        EXPECT_EQ(false, p4.string().empty());
        EXPECT_EQ(false, p5.string().empty());
        EXPECT_EQ(false, p6.string().empty());
        EXPECT_EQ(false, p7.string().empty());
        EXPECT_EQ(false, p8.string().empty());
        EXPECT_EQ(true, amo::path::fullAppName().exists());
    }
    
    {
        amo::u8string str(get_u8path_str(), true);
        amo::u8path p1;
        amo::u8path p2(get_ansi_path_str());
        amo::u8path p3(get_ansi_path_str2());
        amo::u8path p4(get_path_wstr());
        amo::u8path p5(get_path_wstr2());
        amo::u8path p6(str);
        amo::u8path p7(p5);
        amo::u8path p8 = boost::filesystem::path(get_path_wstr());
        
        EXPECT_EQ(false, p1.exists());
        EXPECT_EQ(false, p2.string().empty());
        EXPECT_EQ(false, p3.string().empty());
        EXPECT_EQ(false, p4.string().empty());
        EXPECT_EQ(false, p5.string().empty());
        EXPECT_EQ(false, p6.string().empty());
        EXPECT_EQ(false, p7.string().empty());
        EXPECT_EQ(false, p8.string().empty());
        EXPECT_EQ(true, amo::u8path::fullAppName().exists());
    }
    
}

TEST(path, reset) {
    {
        amo::path p1;
        p1.reset_path("amo");
        EXPECT_EQ(p1.string(), "amo");
        p1.reset_path("");
        EXPECT_EQ(p1.string(), "");
        p1.reset_path(L"amo");
        EXPECT_EQ(p1.string(), "amo");
        p1.reset_path(get_ansi_path_str());
        EXPECT_EQ(p1.string(), get_ansi_path_str());
        
    }
    
    {
        amo::u8path p1;
        p1.reset_path("amo");
        EXPECT_EQ(p1.string(), "amo");
        p1.reset_path("");
        EXPECT_EQ(p1.string(), "");
        p1.reset_path(L"amo");
        EXPECT_EQ(p1.string(), "amo");
        p1.reset_path(get_u8path_str());
        EXPECT_EQ(p1.string(), get_u8path_str());
        
    }
}

TEST(path, has_blanks) {
    {
        amo::path p1(get_ansi_path_str());
        EXPECT_EQ(p1.has_blanks(), true);
        p1.reset_path(get_ansi_path_str2());
        EXPECT_EQ(p1.has_blanks(), false);
    }
    
    {
        amo::u8path p1(get_u8path_str());
        EXPECT_EQ(p1.has_blanks(), true);
        p1.reset_path(get_ansi_path_str2());
        EXPECT_EQ(p1.has_blanks(), false);
    }
}


TEST(path, is_complete) {
    {
        amo::path p1(get_ansi_path_str());
        EXPECT_EQ(p1.is_complete(), false);
        p1.absolute();
        EXPECT_EQ(p1.is_complete(), true);
        p1 = amo::path::fullAppName();
        EXPECT_EQ(p1.is_complete(), true);
        p1 = amo::path::fullAppDir();
        EXPECT_EQ(p1.is_complete(), true);
    }
    
    {
        amo::u8path p1(get_u8path_str());
        EXPECT_EQ(p1.is_complete(), false);
        p1.absolute();
        EXPECT_EQ(p1.is_complete(), true);
        p1 = amo::u8path::fullAppName();
        EXPECT_EQ(p1.is_complete(), true);
        p1 = amo::u8path::fullAppDir();
        EXPECT_EQ(p1.is_complete(), true);
    }
}

TEST(path, empty) {
    {
        amo::path p1;
        EXPECT_EQ(true, p1.empty());
        p1.reset_path("amo");
        EXPECT_EQ(false, p1.empty());
    }
    
    {
        amo::u8path p1;
        EXPECT_EQ(true, p1.empty());
        p1.reset_path("amo");
        EXPECT_EQ(false, p1.empty());
    }
    
}

TEST(path, filename_is_dot) {
    {
        amo::path p1;
        EXPECT_EQ(false, p1.filename_is_dot());
        p1.reset_path("amo");
        EXPECT_EQ(false, p1.filename_is_dot());
        p1.reset_path(".");
        EXPECT_EQ(true, p1.filename_is_dot());
        p1.reset_path("./");
        EXPECT_EQ(true, p1.filename_is_dot());
        p1.reset_path("/.");
        EXPECT_EQ(true, p1.filename_is_dot());
        p1.reset_path("./.");
        EXPECT_EQ(true, p1.filename_is_dot());
        p1.reset_path("./..");
        EXPECT_EQ(false, p1.filename_is_dot());
        p1.reset_path("./123");
        EXPECT_EQ(false, p1.filename_is_dot());
    }
}


TEST(path, filename_is_dot_dot) {
    {
        amo::path p1;
        EXPECT_EQ(false, p1.filename_is_dot_dot());
        p1.reset_path("amo");
        EXPECT_EQ(false, p1.filename_is_dot_dot());
        p1.reset_path("..");
        EXPECT_EQ(true, p1.filename_is_dot_dot());
        p1.reset_path("../");
        EXPECT_EQ(false, p1.filename_is_dot_dot());
        p1.reset_path("/..");
        EXPECT_EQ(true, p1.filename_is_dot_dot());
        p1.reset_path("../..");
        EXPECT_EQ(true, p1.filename_is_dot_dot());
        p1.reset_path("../.");
        EXPECT_EQ(false, p1.filename_is_dot_dot());
        p1.reset_path("../123");
        EXPECT_EQ(false, p1.filename_is_dot_dot());
        p1.reset_path("./../");
        EXPECT_EQ(false, p1.filename_is_dot_dot());
        p1.reset_path("/../");
        EXPECT_EQ(false, p1.filename_is_dot_dot());
        p1.reset_path("11..");
        EXPECT_EQ(false, p1.filename_is_dot_dot());
        p1.reset_path("..11");
        EXPECT_EQ(false, p1.filename_is_dot_dot());
    }
}


TEST(path, has_root_path) {
    amo::path p1;
    EXPECT_EQ(false, p1.has_root_path());
    p1.reset_path("amo");
    EXPECT_EQ(false, p1.has_root_path());
    p1.absolute();
    EXPECT_EQ(true, p1.has_root_path());
    EXPECT_EQ(true, amo::path::fullAppName().has_root_path());
}

TEST(path, has_root_name) {
    amo::path p1;
    EXPECT_EQ(false, p1.has_root_name());
    p1.reset_path("amo");
    EXPECT_EQ(false, p1.has_root_name());
    p1.absolute();
    EXPECT_EQ(true_or_false(), p1.has_root_name());
    EXPECT_EQ(true_or_false(), amo::path::fullAppName().absolute().has_root_name());
}

TEST(path, has_root_directory) {
    amo::path p1;
    EXPECT_EQ(false, p1.has_root_directory());
    p1.reset_path("amo/");
    EXPECT_EQ(false, p1.has_root_directory());
    
    p1.absolute();
    EXPECT_EQ(true, p1.has_root_directory());
    EXPECT_EQ(true, amo::path::fullAppName().has_root_directory());
}

TEST(path, has_relative_path) {
    amo::path p1;
    EXPECT_EQ(false, p1.has_relative_path());
    p1.reset_path("amo");
    EXPECT_EQ(true, p1.has_relative_path());
    p1.absolute();
    EXPECT_EQ(true, p1.has_relative_path());
    EXPECT_EQ(true, amo::path::fullAppName().has_relative_path());
    p1 = p1.root_directory();
    EXPECT_EQ(false, p1.has_relative_path());
    
#ifdef OS_WIN
    p1.reset_path("C:/");
    EXPECT_EQ(false, p1.has_relative_path());
#endif // OS_WIN
}

TEST(path, has_parent_path) {

    //
    amo::path p1;
    EXPECT_EQ(false, p1.has_parent_path());
    p1.reset_path("aaa/amo/");
    EXPECT_EQ(true, p1.has_parent_path());
    p1 = p1.parent();
    EXPECT_EQ(true, p1.has_parent_path());
    p1.absolute();
    EXPECT_EQ(true, p1.has_parent_path());
    EXPECT_EQ(true, amo::path::fullAppName().has_parent_path());
    p1 = p1.root_directory();
    EXPECT_EQ(false, p1.has_parent_path());
    
#ifdef OS_WIN
    p1.reset_path("C:/");
    EXPECT_EQ(true, p1.has_parent_path());
    p1.reset_path("C:");
    EXPECT_EQ(false, p1.has_parent_path());
#endif // OS_WIN
}

TEST(path, has_filename) {

    //  not empty is has filename
    amo::path p1;
    EXPECT_EQ(false, p1.has_filename());
    p1.reset_path("amo/amo/");
    EXPECT_EQ(true, p1.has_filename());
    p1 = p1.parent();
    EXPECT_EQ(true, p1.has_filename());
    p1.absolute();
    EXPECT_EQ(true, p1.has_filename());
    EXPECT_EQ(true, amo::path::fullAppName().has_filename());
    p1 = p1.root_directory();
    EXPECT_EQ(true, p1.has_filename());
    
#ifdef OS_WIN
    p1.reset_path("C:/");
    EXPECT_EQ(true, p1.has_filename());
    p1.reset_path("C:");
    EXPECT_EQ(true, p1.has_filename());
#endif // OS_WIN
}


TEST(path, has_stem) {

    //  not empty is has stem
    amo::path p1;
    EXPECT_EQ(false, p1.has_stem());
    p1.reset_path("amo/amo/");
    EXPECT_EQ(true, p1.has_stem());
    p1 = p1.parent();
    EXPECT_EQ(true, p1.has_stem());
    p1.absolute();
    EXPECT_EQ(true, p1.has_stem());
    
    EXPECT_EQ(true, amo::path::fullAppName().has_stem());
    p1 = p1.root_directory();
    EXPECT_EQ(true, p1.has_stem());
    
#ifdef OS_WIN
    p1.reset_path("C:/");
    EXPECT_EQ(true, p1.has_stem());
    p1.reset_path("C:");
    EXPECT_EQ(true, p1.has_stem());
#endif // OS_WIN
}


TEST(path, has_extension) {

    amo::path p1;
    EXPECT_EQ(false, p1.has_extension());
    p1.reset_path("amo/");
    EXPECT_EQ(false, p1.has_extension());
    p1 = p1.parent();
    EXPECT_EQ(false, p1.has_extension());
    p1.absolute();
    EXPECT_EQ(false, p1.has_extension());
    p1.append("text.txt");
    EXPECT_EQ(true, p1.has_extension());
#ifdef OS_WIN
    EXPECT_EQ(true, amo::path::fullAppName().has_extension());
#else
    EXPECT_EQ(false, amo::path::fullAppName().has_extension());
#endif // OS_WIN
    
    p1 = p1.root_directory();
    EXPECT_EQ(false, p1.has_extension());
    
#ifdef OS_WIN
    p1.reset_path("C:/");
    EXPECT_EQ(false, p1.has_extension());
    p1.reset_path("C:");
    EXPECT_EQ(false, p1.has_extension());
#endif // OS_WIN
}


TEST(path, is_relative) {
    amo::path p1;
    EXPECT_EQ(true, p1.is_relative());
    p1.reset_path("amo/amo/");
    EXPECT_EQ(true, p1.is_relative());
    p1 = p1.parent();
    EXPECT_EQ(true, p1.is_relative());
    p1.absolute();
    std::cout << p1.raw_string() << std::endl;
    EXPECT_EQ(false, p1.is_relative());
    p1.append("text.txt");
    EXPECT_EQ(false, p1.is_relative());
    EXPECT_EQ(false, amo::path::fullAppName().is_relative());
    p1 = p1.root_directory();
    std::cout << p1.raw_string() << std::endl;
    EXPECT_EQ(win_or_linux(), p1.is_relative());
    
#ifdef OS_WIN
    p1.reset_path("C:/");
    EXPECT_EQ(false, p1.is_relative());
    p1.reset_path("C:");
    EXPECT_EQ(true, p1.is_relative());
#endif // OS_WIN
}


TEST(path, is_absolute) {

    amo::path p1;
    EXPECT_EQ(false, p1.is_absolute());
    p1.reset_path("amo/amo/");
    EXPECT_EQ(false, p1.is_absolute());
    p1 = p1.parent();
    EXPECT_EQ(false, p1.is_absolute());
    p1.absolute();
    EXPECT_EQ(true, p1.is_absolute());
    p1.append("text.txt");
    EXPECT_EQ(true, p1.is_absolute());
    EXPECT_EQ(true, amo::path::fullAppName().is_absolute());
    p1 = p1.root_directory();
    std::cout << p1.raw_string() << std::endl;
    EXPECT_EQ(!win_or_linux(), p1.is_absolute());
    
#ifdef OS_WIN
    p1.reset_path("C:/");
    EXPECT_EQ(true, p1.is_absolute());
    p1.reset_path("C:");
    EXPECT_EQ(false, p1.is_absolute());
#endif // OS_WIN
}



TEST(path, has_leaf) {

    //   not empty is has leaf
    amo::path p1;
    EXPECT_EQ(false, p1.has_leaf());
    p1.reset_path("amo/amo/");
    EXPECT_EQ(true, p1.has_leaf());
    p1 = p1.parent();
    EXPECT_EQ(true, p1.has_leaf());
    p1.absolute();
    EXPECT_EQ(true, p1.has_leaf());
    p1.append("text.txt");
    EXPECT_EQ(true, p1.has_leaf());
    EXPECT_EQ(true, amo::path::fullAppName().has_leaf());
    p1 = p1.root_directory();
    EXPECT_EQ(true, p1.has_leaf());
    
#ifdef OS_WIN
    p1.reset_path("C:/");
    EXPECT_EQ(true, p1.has_leaf());
    p1.reset_path("C:");
    EXPECT_EQ(true, p1.has_leaf());
#endif // OS_WIN
}


TEST(path, has_branch_path) {

    //
    amo::path p1;
    EXPECT_EQ(false, p1.has_branch_path());
    p1.reset_path("amo/amo/");
    EXPECT_EQ(true, p1.has_branch_path());
    p1 = p1.parent();
    EXPECT_EQ(true, p1.has_branch_path());
    EXPECT_EQ(false, p1.parent().has_branch_path());
    p1.absolute();
    EXPECT_EQ(true, p1.has_branch_path());
    p1.append("text.txt");
    EXPECT_EQ(true, p1.has_branch_path());
    EXPECT_EQ(true, amo::path::fullAppName().has_branch_path());
    p1 = p1.root_directory();
    EXPECT_EQ(false, p1.has_branch_path());
    
#ifdef OS_WIN
    p1.reset_path("C:/");
    EXPECT_EQ(true, p1.has_branch_path());
    p1.reset_path("C:");
    EXPECT_EQ(false, p1.has_branch_path());
#endif // OS_WIN
}


TEST(path, remove_args) {
    amo::path p1("aaa -333 -4");
    p1.remove_args();
    EXPECT_EQ(p1.string(), "aaa");
    
    p1.reset_path("22/sdd ");
    p1.remove_args();
    EXPECT_EQ(p1.string(), "22/sdd");
    
    p1.reset_path("22/sdd /");
    p1.remove_args();
    EXPECT_EQ(p1.string(), "22/sdd /");
    
    
}

TEST(path, remove_backslash) {
    amo::path p1("aaa ");
    p1.remove_backslash();
    EXPECT_EQ(p1.string(), "aaa ");
    p1.reset_path("aaa /");
    p1.remove_backslash();
    EXPECT_EQ(p1.string(), "aaa ");
    p1.reset_path("aaa \\");
    p1.remove_backslash();
    EXPECT_EQ(p1.string(), "aaa ");
}

TEST(path, remove_front_backslash) {
    amo::path p1("/aaa ");
    p1.remove_front_backslash();
    EXPECT_EQ(p1.to_string(), "aaa ");
    p1.reset_path("./aaa /");
    p1.remove_front_backslash();
    EXPECT_EQ(p1.to_string(), "aaa /");
    p1.reset_path("\\aaa \\");
    p1.remove_front_backslash();
    EXPECT_EQ(p1.to_string(), "aaa \\");
    p1.reset_path(".\\aaa \\");
    p1.remove_front_backslash();
    EXPECT_EQ(p1.to_string(), "aaa \\");
}

TEST(path, remove_front_backslash_copy) {
    amo::path p1("/aaa ");
    p1 = p1.remove_front_backslash_copy();
    EXPECT_EQ(p1.to_string(), "aaa ");
    p1.reset_path("./aaa /");
    p1 = p1.remove_front_backslash_copy();
    EXPECT_EQ(p1.to_string(), "aaa /");
    p1.reset_path("\\aaa \\");
    p1 = p1.remove_front_backslash_copy();
    EXPECT_EQ(p1.to_string(), "aaa \\");
    p1.reset_path(".\\aaa \\");
    p1 = p1.remove_front_backslash_copy();
    EXPECT_EQ(p1.to_string(), "aaa \\");
}

TEST(path, add_backslash) {
    amo::path p1("/aaa ");
    p1.add_backslash();
    EXPECT_EQ(p1.to_string(), "/aaa /");
    p1.reset_path("./aaa /");
    p1.add_backslash();
    EXPECT_EQ(p1.to_string(), "./aaa /");
    p1.reset_path("\\aaa \\");
    p1.add_backslash();
    EXPECT_EQ(p1.to_string(), "\\aaa /");
    p1.reset_path(".\\aaa \\");
    p1.add_backslash();
    EXPECT_EQ(p1.to_string(), ".\\aaa /");
}

TEST(path, add_front_backslash) {
    amo::path p1("/aaa ");
    p1.add_front_backslash();
    EXPECT_EQ(p1.to_string(), "./aaa ");
    p1.reset_path("./aaa /");
    p1.add_front_backslash();
    EXPECT_EQ(p1.to_string(), "./aaa /");
    p1.reset_path("\\aaa \\");
    p1.add_front_backslash();
    EXPECT_EQ(p1.to_string(), "./aaa \\");
    p1.reset_path(".\\aaa \\");
    p1.add_front_backslash();
    EXPECT_EQ(p1.to_string(), "./aaa \\");
    
    p1.reset_path(".\\aaa \\");
    p1.add_front_backslash("/");
    EXPECT_EQ(p1.to_string(), "/aaa \\");
}


TEST(path, remove_blanks) {
    amo::path p1("/aaa ");
    p1.remove_backslash().remove_blanks();
    EXPECT_EQ(p1.to_string(), "/aaa");
    p1.reset_path(" ./aaa /");
    p1.remove_backslash().remove_blanks();
    EXPECT_EQ(p1.to_string(), "./aaa");
    p1.reset_path(" \\aaa \\");
    p1.remove_backslash().remove_blanks();
    EXPECT_EQ(p1.to_string(), "\\aaa");
    p1.reset_path(" .\\aaa \\");
    p1.remove_backslash().remove_blanks();
    EXPECT_EQ(p1.to_string(), ".\\aaa");
}

TEST(path, add_extension) {
    {
        amo::path p1("/aaa ");
        p1.add_extension(".cc");
        EXPECT_EQ(p1.to_string(), "/aaa .cc");
        p1.add_extension(".cc");
        EXPECT_EQ(p1.to_string(), "/aaa .cc.cc");
    }
    
    
    {
        amo::path p1("/aaa /");
        p1.add_extension(".cc");
        EXPECT_EQ(p1.to_string(), "/aaa /.cc");
        p1.add_extension(".cc");
        EXPECT_EQ(p1.to_string(), "/aaa /.cc.cc");
    }
    
}

TEST(path, remove_file_spec) {
    amo::path p1 = amo::path::fullAppName();
    amo::path p2 = amo::path::fullAppDir();
    p1.remove_file_spec();
    
    EXPECT_EQ(p1.string(), p2.string());
    
}

TEST(path, remove_file_spec_copy) {
    amo::path p1 = amo::path::fullAppName();
    amo::path p2 = amo::path::fullAppDir();
    p1 = p1.remove_file_spec_copy();
    
    EXPECT_EQ(p1.string(), p2.string());
}

TEST(path, quote_spaces) {
    {
        amo::path p1 = amo::path::fullAppDir();
        p1.append("23 433");
        p1.quote_spaces();
        std::string rhs = "\"";
        rhs += amo::path::fullAppDir().append("23 433").to_linux_string();
        rhs += "\"";
        EXPECT_EQ(p1.to_linux_string(), rhs);
    }
    {
        amo::path p1("./");
        p1.append("23433");
        amo::path p2 = p1;
        p1.quote_spaces();
        
        EXPECT_EQ(p1.to_linux_string(), p2.to_linux_string());
    }
    
}
TEST(path, quote) {
    {
        amo::path p1 = amo::path::fullAppDir();
        p1.append("23 433");
        p1.quote();
        std::string rhs = "\"";
        rhs += amo::path::fullAppDir().append("23 433").to_linux_string();
        rhs += "\"";
        EXPECT_EQ(p1.to_linux_string(), rhs);
    }
    {
        amo::path p1 = amo::path::fullAppDir();
        p1.append("23433");
        p1.quote();
        std::string rhs = "\"";
        rhs += amo::path::fullAppDir().append("23433").to_linux_string();
        rhs += "\"";
        EXPECT_EQ(p1.to_linux_string(), rhs);
    }
    
}

TEST(path, append) {
    amo::path p;
    p.append("ccc");
    p.append("ddd");
    EXPECT_EQ("ccc/ddd", p.to_linux_string());
    p.append("/eee/");
    EXPECT_EQ("ccc/ddd/eee/", p.to_linux_string());
    p.append("//fff/");
    EXPECT_EQ("ccc/ddd/eee/fff/", p.to_linux_string());
    p.append("./ggg/");
    EXPECT_EQ("ccc/ddd/eee/fff/ggg/", p.to_linux_string());
    p.append("./././/hhh/");
    EXPECT_EQ("ccc/ddd/eee/fff/ggg/hhh/", p.to_linux_string());
    p.append(".\\.\\.//iii");
    EXPECT_EQ("ccc/ddd/eee/fff/ggg/hhh/iii", p.to_linux_string());
    p.append(".\\.\\.//jjj./");
    EXPECT_EQ("ccc/ddd/eee/fff/ggg/hhh/iii/jjj/", p.to_linux_string());
    
    amo::path p2(".");
    p2.append(p);
    EXPECT_EQ("./ccc/ddd/eee/fff/ggg/hhh/iii/jjj/", p2.to_linux_string());
    
    p.append(".\\.\\..//kkk./");
    EXPECT_NE("ccc/ddd/eee/fff/ggg/hhh/iii/jjj/kkk/", p.to_linux_string());
    EXPECT_EQ("ccc/ddd/eee/fff/ggg/hhh/iii/jjj/../kkk/", p.to_linux_string());
}


TEST(path, append_c) {
    amo::path p;
    p = p.append_c("ccc");
    p = p.append_c("ddd");
    EXPECT_EQ("ccc/ddd", p.to_linux_string());
    p = p.append_c("/eee/");
    EXPECT_EQ("ccc/ddd/eee/", p.to_linux_string());
    p = p.append_c("//fff/");
    EXPECT_EQ("ccc/ddd/eee/fff/", p.to_linux_string());
    p = p.append_c("./ggg/");
    EXPECT_EQ("ccc/ddd/eee/fff/ggg/", p.to_linux_string());
    p = p.append_c("./././/hhh/");
    EXPECT_EQ("ccc/ddd/eee/fff/ggg/hhh/", p.to_linux_string());
    p = p.append_c(".\\.\\.//iii");
    EXPECT_EQ("ccc/ddd/eee/fff/ggg/hhh/iii", p.to_linux_string());
    p = p.append_c(".\\.\\.//jjj./");
    EXPECT_EQ("ccc/ddd/eee/fff/ggg/hhh/iii/jjj/", p.to_linux_string());
    
    amo::path p2(".");
    p2 = p2.append_c(p);
    EXPECT_EQ("./ccc/ddd/eee/fff/ggg/hhh/iii/jjj/", p2.to_linux_string());
    
    p = p.append_c(".\\.\\..//kkk./");
    EXPECT_NE("ccc/ddd/eee/fff/ggg/hhh/iii/jjj/kkk/", p.to_linux_string());
    EXPECT_EQ("ccc/ddd/eee/fff/ggg/hhh/iii/jjj/../kkk/", p.to_linux_string());
}



TEST(path, skip_root) {
    amo::path p = amo::path::fullAppName();
    p.skip_root();
    std::cout << p.to_string() << std::endl;
    
}

TEST(path, strip_path) {
    amo::path p = amo::path::fullAppName();
    p.strip_path();
    std::string str = p.to_linux_string();
    EXPECT_EQ(str.find('/'), -1);
    
}


TEST(path, strip_to_root) {
    amo::path p = amo::path::fullAppName();
    p.strip_to_root();
    std::string str = p.to_linux_string();
    EXPECT_NE(str.find('/'), -1);
}

TEST(path, compact_path) {
#ifdef OS_WIN
    amo::path p = amo::path::fullAppName();
    p.compact_path(40);
    std::string str = p.to_linux_string();
    EXPECT_NE(str.find('/'), -1);
#endif // OS_WIN
    
}


TEST(path, parent) {
    amo::path p;
    p.append("ccc");
    p.append("ddd");
    p.append("/eee/");
    p.append("//fff/");
    p.append("./ggg/");
    p.append("./././/hhh/");
    p.append(".\\.\\.//iii");
    p.append(".\\.\\.//jjj./");
    p.append(".\\.\\..//kkk./");
    
    
    p = p.parent();
    EXPECT_EQ("ccc/ddd/eee/fff/ggg/hhh/iii/jjj/../",
              amo::path(p).to_linux_string());
    p = p.parent();
    EXPECT_EQ("ccc/ddd/eee/fff/ggg/hhh/iii/jjj/",
              amo::path(p).to_linux_string());
    p = p.parent();
    EXPECT_EQ("ccc/ddd/eee/fff/ggg/hhh/iii/",
              amo::path(p).to_linux_string());
    p = p.parent();
    EXPECT_EQ("ccc/ddd/eee/fff/ggg/hhh/",
              amo::path(p).to_linux_string());
    p = p.parent();
    EXPECT_EQ("ccc/ddd/eee/fff/ggg/",
              amo::path(p).to_linux_string());
    p = p.parent();
    EXPECT_EQ("ccc/ddd/eee/fff/",
              amo::path(p).to_linux_string());
    p = p.parent();
    EXPECT_EQ("ccc/ddd/eee/",
              amo::path(p).to_linux_string());
    p = p.parent();
    EXPECT_EQ("ccc/ddd/",
              amo::path(p).to_linux_string());
    p = p.parent();
    EXPECT_EQ("ccc/",
              amo::path(p).to_linux_string());
    p = p.parent();
    EXPECT_EQ("",
              amo::path(p).to_linux_string());
    p = p.parent();
    EXPECT_EQ("",
              amo::path(p).to_linux_string());
}

TEST(path, find) {
    amo::path p;
    p.append("ccc");
    p.append("ddd");
    p.append("/eee/");
    p.append("//fff/");
    p.append("./ggg/");
    p.append("./././/hhh/");
    p.append(".\\.\\.//iii");
    p.append(".\\.\\.//jjj./");
    p.append(".\\.\\..//kkk./");
    
    EXPECT_EQ(p.find(".\\.\\hhh"), true);
    EXPECT_EQ(p.find(".\\.\\hhh/iii"), true);
    EXPECT_EQ(p.find("./jjj"), true);
    EXPECT_EQ(p.find("./jjjd"), false);
    
}

TEST(path, find_extension) {
    amo::path p;
    EXPECT_EQ(p.find_extension(), "");
    p.append("ddd");
    EXPECT_EQ(p.find_extension(), "");
    p.append("eee/");
    EXPECT_EQ(p.find_extension(), "");
    p.append("fff.cc");
    EXPECT_EQ(p.find_extension(), ".cc");
    
#ifdef OS_WIN
    EXPECT_EQ(".exe", amo::path::fullAppName().find_extension());
#else
    EXPECT_EQ("", amo::path::fullAppName().find_extension());
#endif
    
}

TEST(path, find_file_name) {
    amo::path p;
    EXPECT_EQ(p.find_file_name(), "");
    p.append("ddd");
    EXPECT_EQ(p.find_file_name(), "ddd");
    p.append("eee/");
    EXPECT_EQ(p.find_file_name(), ".");
    p.append("fff.cc");
    EXPECT_EQ(p.find_file_name(), "fff.cc");
    
}
TEST(path, find_suffix_array) {
    std::vector<std::string> exts{ ".exe", ".cc" };
    amo::path p;
    EXPECT_EQ(p.find_suffix_array(exts), false);
    p.append("aaa.cc");
    EXPECT_EQ(p.find_suffix_array(exts), true);
    
    
#ifdef OS_WIN
    EXPECT_EQ(true, amo::path::fullAppName().find_suffix_array(exts));
#else
    EXPECT_EQ(false, amo::path::fullAppName().find_suffix_array(exts));
#endif
    
}
TEST(path, get_args) {

    amo::path p1("aaa -333 -4");
    
    EXPECT_EQ(p1.get_args(), "-333 -4");
    
    p1.reset_path("22/sdd ");
    EXPECT_EQ(p1.get_args(), "");
    
    p1.reset_path("22/sdd /");
    EXPECT_EQ(p1.get_args(), "");
}

TEST(path, get_drive_number) {
    amo::path p;
    EXPECT_EQ(false, p.exists());
    EXPECT_EQ(true, amo::path::fullAppName().get_drive_number() >= 0);
}

TEST(path, split) {
    amo::path p;
    EXPECT_EQ(1, p.split(false).size());
    p.reset_path("./234/aaa///3333");
    EXPECT_EQ(3, p.split(false).size());
    p.reset_path("./.\\./234/aaa///3333");
    EXPECT_EQ(3, p.split(false).size());
    EXPECT_EQ(true, amo::path::fullAppName().exists());
}
TEST(path, common_path) {

    try {
        amo::path p;
        amo::path p2;
        
        p.reset_path("./234/aaa///3333");
        amo::path p3 = p.common_path(p2);
        EXPECT_EQ(p3.remove_front_backslash().to_linux_string(), "");
        p2.reset_path("./22");
        p3 = p.common_path(p2);
        
        {
            auto p3 = p;
            auto p4 = p2;
            p.absolute();
            p.canonicalize(false);
            p.remove_backslash();
            
            p4.absolute();
            p4.canonicalize(false);
            p4.remove_backslash();
            
            std::cout << p.to_linux_string() << std::endl;
            std::cout << p4.to_linux_string() << std::endl;
            
            p.normalize();
            p4.normalize();
            
            std::cout << p.to_linux_string() << std::endl;
            std::cout << p4.to_linux_string() << std::endl;
            std::cout << "------------" << std::endl;
            std::cout << amo::path::fullAppDir().normalize().to_linux_string() << std::endl;
            
        }
        EXPECT_EQ(p3.remove_front_backslash().to_linux_string(),
                  amo::path::fullAppDir().remove_front_backslash().to_linux_string());
        p3 = p.common_path(p2, false);
        std::cout << p3.to_linux_string() << std::endl;
        EXPECT_EQ(p3.remove_front_backslash().to_linux_string(), "");
        EXPECT_EQ(true, amo::path::fullAppName().exists());
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    
    
}

TEST(path, relative_path_from) {

    {
        amo::path p1 = amo::path::fullAppName();
        amo::path p2 = amo::path::fullAppDir();
        //p2 = p2.parent();
        p2.append("FDS");
        p1.relative_path_from(p2);
#ifdef OS_WIN
        EXPECT_EQ(p1.to_linux_string(), "./../examples.exe");
#else
        EXPECT_EQ(p1.to_linux_string(), "./../examples");
#endif // OS_WIN
        
    }
    
    {
        amo::path p1("111/222/333/");
        amo::path p2("111/444");
        p1.relative_path_from(p2);
        EXPECT_EQ(p1.to_linux_string(), "./../222/333");
    }
}

TEST(path, relative_path_from_copy) {

    {
        amo::path p1 = amo::path::fullAppName();
        amo::path p2 = amo::path::fullAppDir();
        //p2 = p2.parent();
        p2.append("FDS");
        p1 = p1.relative_path_from_copy(p2);
#ifdef OS_WIN
        EXPECT_EQ(p1.to_linux_string(), "./../examples.exe");
#else
        EXPECT_EQ(p1.to_linux_string(), "./../examples");
#endif // OS_WIN
        
    }
    
    {
        amo::path p1("111/222/333/");
        amo::path p2("111/444");
        p1 =  p1.relative_path_from_copy(p2);
        EXPECT_EQ(p1.to_linux_string(), "./../222/333");
    }
}


TEST(path, sub_path_from) {

    {
        amo::path p1 = amo::path::fullAppName();
        amo::path p2 = amo::path::fullAppDir();
        
        p1.sub_path_from(p2);
#ifdef OS_WIN
        EXPECT_EQ(p1.to_linux_string(), "./examples.exe");
#else
        EXPECT_EQ(p1.to_linux_string(), "./examples");
#endif // OS_WIN
        
    }
    
    {
        amo::path p1("111/222/333/");
        amo::path p2("111/444");
        p1.sub_path_from(p2);
        EXPECT_EQ(p1.to_linux_string(), "./../222/333");
    }
}


TEST(path, sub_path_from_copy) {

    {
        amo::path p1 = amo::path::fullAppName();
        amo::path p2 = amo::path::fullAppDir();
        p1 = p1.sub_path_from_copy(p2);
#ifdef OS_WIN
        EXPECT_EQ(p1.to_linux_string(), "./examples.exe");
#else
        EXPECT_EQ(p1.to_linux_string(), "./examples");
#endif // OS_WIN
        
    }
    
    {
        amo::path p1("111/222/333/");
        amo::path p2("111/444");
        p1 = p1.sub_path_from_copy(p2);
        EXPECT_EQ(p1.to_linux_string(), "./../222/333");
    }
}

TEST(path, trim_right) {

    {
        amo::path p1 = amo::path::fullAppName();
        amo::path p2(amo::path::getExeName());
        //p2 = p2.parent();
        p1.trim_right(p2);
        p1.remove_backslash();
        EXPECT_EQ(p1.normalize().to_linux_string(),
                  amo::path::fullAppDir().remove_backslash().normalize().to_linux_string());
                  
                  
    }
    
    
}

TEST(path, trim_right_copy) {

    {
        amo::path p1 = amo::path::fullAppName();
        amo::path p2(amo::path::getExeName());
        //p2 = p2.parent();
        p1 = p1.trim_right_copy(p2);
        p1.remove_backslash();
        EXPECT_EQ(p1.to_linux_string(),
                  amo::path::fullAppDir().remove_backslash().to_linux_string());
    }
    
}


TEST(path, trim_left) {

    {
        amo::path p("./");
        p = p.trim_left();
        EXPECT_EQ(p.to_string(), "");
        p.reset_path(" \t./");
        p = p.trim_left();
        EXPECT_EQ(p.to_string(), "");
        p.reset_path("./ \t");
        p = p.trim_left();
        EXPECT_EQ(p.to_string(), " \t");
        p.reset_path("./ \t");
        p = p.trim_left();
        EXPECT_EQ(p.to_string(), " \t");
        EXPECT_EQ(true, amo::path::fullAppName().exists());
    }
    {
        amo::path p1 = amo::path::fullAppName();
        amo::path p2(amo::path::fullAppDir());
        //p2 = p2.parent();
        p1.trim_left(p2);
        
        
        EXPECT_EQ(p1 == amo::path(amo::path::getExeName()), true);
    }
    
    
}

TEST(path, trim_left_copy) {

    {
        amo::path p1 = amo::path::fullAppName();
        amo::path p2(amo::path::fullAppDir());
        //p2 = p2.parent();
        p1 = p1.trim_left_copy(p2);
        
        EXPECT_EQ(p1 == amo::path(amo::path::getExeName()), true);
    }
    
}


TEST(path, canonicalize) {
    {
        amo::path p1("./111/./.\\222/333/////.\\444.555/666");
        p1.canonicalize(false);
        p1.canonicalize(true);
        amo::path p2(amo::path::fullAppDir());
        EXPECT_EQ(p1.to_linux_string(),  "./111/222/333/444.555/666");
        
    }
    {
        amo::path p1("./../111/./.\\222/333/////.\\444.555/666");
        p1.canonicalize(false);
        p1.canonicalize(true);
        
        amo::path p2(amo::path::fullAppDir());
        EXPECT_EQ(p1.to_linux_string(), "./../111/222/333/444.555/666");
        
    }
}

TEST(path, remove_dot_dot) {
    {
        amo::path p1("./111/./.\\222/333/////.\\444.555/666/../");
        p1.canonicalize(false);
        p1.remove_dot_dot();
        amo::path p2(amo::path::fullAppDir());
        EXPECT_EQ(p1.to_linux_string(), "./111/222/333/444.555/");
        
    }
    {
        amo::path p1("./../111/./.\\222/333/////.\\444..555/../../../666/../");
        p1.canonicalize(false);
        p1.remove_dot_dot();
        EXPECT_EQ(p1.to_linux_string(), "./../111/");
    }
    
    {
        amo::path p1("./../111/./.\\222/333/////.\\444..555/../../../666/../");
        p1.canonicalize(false);
        p1.remove_dot_dot();
        EXPECT_EQ(p1.to_linux_string(), "./../111/");
        
        p1.reset_path("../../234");
        p1.remove_dot_dot();
        EXPECT_EQ(p1.to_linux_string(), "../../234");
        
        p1.reset_path("../../234/..");
        p1.remove_dot_dot();
        EXPECT_EQ(p1.to_linux_string(), "../..");
        
        p1.reset_path("../../234/../234/../../");
        p1.remove_dot_dot();
        EXPECT_EQ(p1.to_linux_string(), "../../../");
        
        p1.reset_path("./sddf/../../233/../234/../../");
        p1.remove_dot_dot();
        EXPECT_EQ(p1.to_linux_string(), "./../../");
        
    }
    
}

TEST(path, get_short_path_name) {
    {
    
        amo::path p1(amo::path::fullAppName());
        p1.get_short_path_name();
        
    }
    
}


TEST(path, get_long_path_name) {
    {
    
        amo::path p1(amo::path::fullAppName());
        p1.get_short_path_name();
        EXPECT_EQ(p1.get_long_path_name().to_linux_string(),
                  amo::path::fullAppName().to_linux_string());
    }
    
}


TEST(path, make_pretty) {
    {
    
        amo::path p1(amo::path::fullAppName());
        std::string str = p1.to_linux_string();
        amo::string_utils::to_lower(str);
        p1.make_pretty();
        EXPECT_EQ(p1.to_linux_string(),  str);
    }
    
}



TEST(path, work_path) {
    {
    
        std::cout << "work path: " << amo::path::work_path().to_linux_string() <<
                  std::endl;
    }
    
}

TEST(path, is_file_spec) {
    {
        amo::path p1(amo::path::fullAppName());
        EXPECT_EQ(p1.is_file_spec(), !win_or_linux());
        // linux always true
        EXPECT_EQ(p1.filename().is_file_spec(), true);
        p1 = p1.parent();
        EXPECT_EQ(p1.is_file_spec(), !win_or_linux());
    }
    
}
TEST(path, is_root) {
    amo::path p1;
    EXPECT_EQ(false, p1.is_root());
    p1 = (amo::path::fullAppDir());
    EXPECT_EQ(false, p1.is_root());
    
    EXPECT_EQ(true, p1.root_directory().is_root());
    EXPECT_EQ(true, p1.root_path().is_root());
    EXPECT_EQ(true, amo::path::fullAppName().exists());
}

TEST(path, is_same_root) {
    amo::path p1 = amo::path::fullAppDir();
    amo::path p2 = amo::path::fullAppName();
    EXPECT_EQ(p1.is_same_root(p2), true);
    
    EXPECT_EQ(true, amo::path::fullAppName().exists());
}


TEST(path, transfer) {
    {
        amo::path p1 = amo::path::fullAppDir();
        int count = 0;
        p1.transfer([&](amo::path & p) {
            ++count;
            
        }, false);
        
        std::cout << "total files : " << count << std::endl;
    }
    
    
    {
        int count = 0;
        amo::u8path p1 = amo::u8path::fullAppDir();
        p1 = p1.parent();
        p1.transfer([&](amo::u8path & p) {
            ++count;
            
        }, true);
        
        std::cout << "total files : " << count << std::endl;
    }
}

TEST(path, file_exists) {
    amo::path p;
    EXPECT_EQ(false, p.file_exists());
    EXPECT_EQ(true, amo::path::fullAppName().file_exists());
}

TEST(path, fullAppDir) {
    amo::path p = amo::path::fullAppDir();
    EXPECT_EQ(true, p.exists());
    EXPECT_EQ(true, amo::path::fullAppName().exists());
}


TEST(path, getExeDir) {
    amo::path p(amo::path::getExeDir());
    EXPECT_EQ(p.to_linux_string(), amo::path::fullAppDir().to_linux_string());
    
}

TEST(path, appName) {
    amo::path p = amo::path::appName();
    EXPECT_EQ(p.to_linux_string(),
              amo::path(amo::path::getExeName()).to_linux_string());
              
}

TEST(path, getExeName) {
    amo::path p(amo::path::getExeName());
    EXPECT_EQ(p.to_linux_string(),
              amo::path(amo::path::appName()).to_linux_string());
              
}

TEST(path, fullAppName) {
    amo::path p = amo::path::fullAppName();
    EXPECT_EQ(p.to_linux_string(),
              amo::path(amo::path::getFullExeName()).to_linux_string());
              
}

TEST(path, getFullExeName) {
    amo::path p(amo::path::getFullExeName());
    EXPECT_EQ(p.to_linux_string(),
              amo::path(amo::path::fullAppName()).to_linux_string());
              
}

TEST(path, fullPathInAppDir) {
    amo::path p = amo::path::fullPathInAppDir("aaa/bbb/ccc");
    EXPECT_EQ(p.to_linux_string(),
              amo::path(amo::path::getFullPathInExeDir("aaa/bbb/ccc",
                        false)).to_linux_string());
                        
}

TEST(path, getFullPathInExeDir) {
    amo::path p(amo::path::getFullPathInExeDir("aaa/bbb/ccc", false));
    EXPECT_EQ(p.to_linux_string(),
              amo::path(amo::path::fullPathInAppDir("aaa/bbb/ccc")).to_linux_string());
              
}

//TEST(path, exists) {
//	amo::path p;
//	EXPECT_EQ(false, p.exists());
//
//	EXPECT_EQ(true, amo::path::fullAppName().exists());
//}


TEST(path, exists) {
    amo::path p;
    EXPECT_EQ(false, p.exists());
    
    EXPECT_EQ(true, amo::path::fullAppName().exists());
}


TEST(path, normalize) {
    amo::path p3 = amo::path::fullAppName();
    std::string strr = p3.normalize().to_linux_string();
    p3.reset_path("./sddf/../../233/../234/../../");
    std::string strr2 = p3.normalize() .to_linux_string();
    std::cout << strr << std::endl;
    std::cout << strr2 << std::endl;
    
    std::string str = "adfa/../dfasdf/";
    p3.reset_path(str);
    std::cout << p3.normalize().to_linux_string() << std::endl;
    str = "../../234";
    p3.reset_path(str);
    std::cout << p3.normalize().to_linux_string() << std::endl;
    str = "../../234/..";
    p3.reset_path(str);
    std::cout << p3.normalize().to_linux_string() << std::endl;
    str = "../../234/../234/../../";
    p3.reset_path(str);
    std::cout << p3.normalize().to_linux_string() << std::endl;
    str = "./sddf/../../233/../234/../../";
    p3.reset_path(str);
    std::cout << p3.normalize().to_linux_string() << std::endl;
    str = "/../233.中/../234/../../..";
    p3.reset_path(str);
    std::cout << p3.normalize().to_linux_string() << std::endl;
    str = "/../2/../234/../../";
    p3.reset_path(str);
    std::cout << p3.normalize().to_linux_string() << std::endl;
    str = "./../../";
    p3.reset_path(str);
    std::cout << p3.normalize().to_linux_string() << std::endl;
    
}

TEST(path, create_file) {

    {
        amo::path p("./");
        p.append("aaa/bbb/ccc/ddd/test.cc");
        EXPECT_EQ(true, p.create_file());
        p.reset_path("aaa/bbb/ccc/ddd/99/test.cc");
        EXPECT_EQ(true, p.create_file());
        p.reset_path("aaa/bbb/ccc/ddd/88/test.cc");
        EXPECT_EQ(true, p.create_file());
    }
    {
        amo::u8path p("./");
        p.append(u8"aaa/bbb/ccc/ddd/u8/test.cc");
        EXPECT_EQ(true, p.create_file());
        p.reset_path(u8"aaa/bbb/ccc/ddd/u8/99/test.cc");
        EXPECT_EQ(true, p.create_file());
        p.reset_path(u8"aaa/bbb/ccc/ddd/u8/88/test.cc");
        EXPECT_EQ(true, p.create_file());
        p.reset_path(u8"aaa/bbb/ccc/ddd/u8/" + get_u8path_str());
        EXPECT_EQ(true, p.create_file());
    }
    
}


TEST(path, create_directory) {

    {
        amo::path p;
        p.append("aaa/bbb/ccc/ddd/");
        EXPECT_EQ(true, p.create_directory());
        p.reset_path("aaa/bbb/ccc/ddd/99/");
        EXPECT_EQ(true, p.create_directory());
        p.reset_path("aaa/bbb/ccc/ddd/88/");
        EXPECT_EQ(true, p.create_directory());
    }
    
    {
        amo::u8path p;
        p.append(u8"aaa/中文目录/bbb/ccc/ddd/u8/");
        EXPECT_EQ(true, p.create_directory());
        p.reset_path(u8"aaa/中文目录/bbb/ccc/ddd/u8/99");
        EXPECT_EQ(true, p.create_directory());
        p.reset_path(u8"aaa/中文目录/bbb/ccc/ddd/u8/88/");
        EXPECT_EQ(true, p.create_directory());
        p.reset_path(u8"aaa/中文目录/bbb/ccc/ddd/u8/" + get_u8path_dir());
        EXPECT_EQ(true, p.create_directory());
        
        p.reset_path(u8"./" + get_u8path_dir());
        EXPECT_EQ(true, p.create_directory());
        
    }
    
}


TEST(path, space) {

    {
        amo::path p("./");
        std::cout << p.capacity_size() << std::endl;
        std::cout << p.free_size() << std::endl;
        std::cout << p.available_size() << std::endl;
    }
}

TEST(path, is_directory) {

    {
        amo::path p("./");
        EXPECT_EQ(p.is_directory(), true);
        p.reset_path("aaa/bbb/ccc/ddd/");
        $linux({ p.create_directory(); });
        EXPECT_EQ(true, p.is_directory());
        p.reset_path("aaa/bbb/ccc/ddd/99/");
        $linux({ p.create_directory(); });
        EXPECT_EQ(true, p.is_directory());
        p.reset_path("aaa/bbb/ccc/ddd/88/");
        $linux({ p.create_directory(); });
        EXPECT_EQ(true, p.is_directory());
    }
    
    {
        amo::u8path p;
        p.append(u8"aaa/中文目录/bbb/ccc/ddd/u8/");
        $linux({ p.create_directory(); });
        EXPECT_EQ(true, p.is_directory());
        p.reset_path(u8"aaa/中文目录/bbb/ccc/ddd/u8/99");
        $linux({ p.create_directory(); });
        EXPECT_EQ(true, p.is_directory());
        p.reset_path(u8"aaa/中文目录/bbb/ccc/ddd/u8/88/");
        $linux({ p.create_directory(); });
        EXPECT_EQ(true, p.is_directory());
        p.reset_path(u8"aaa/中文目录/bbb/ccc/ddd/u8/" + get_u8path_dir());
        $linux({ p.create_directory(); });
        EXPECT_EQ(true, p.is_directory());
    }
}


TEST(path, is_regular) {

    {
        amo::path p;
        p.append("aaa/bbb/ccc/ddd/test.cc");
        p.create_file();
        std::cout << p.raw_string() << std::endl;
        std::cout << p.absolute().raw_string() << std::endl;
        EXPECT_EQ(true, p.is_regular());
        p.reset_path("aaa/bbb/ccc/ddd/99/test.cc");
        p.create_file();
        EXPECT_EQ(true, p.is_regular());
        p.reset_path("aaa/bbb/ccc/ddd/88/test.cc");
        p.create_file();
        EXPECT_EQ(true, p.is_regular());
        EXPECT_EQ(true, amo::path::fullAppName().is_regular());
    }
    {
        amo::u8path p;
        p.append(u8"aaa/bbb/ccc/ddd/u8//234/111test.cc");
        p.create_file();
        EXPECT_EQ(true, p.is_regular());
        
        p.reset_path(u8"aaa/bbb/ccc/ddd/u8/99/test.cc");
        p.create_file();
        EXPECT_EQ(true, p.is_regular());
        
        p.reset_path(u8"aaa/bbb/ccc/ddd/u8/88/test.cc");
        p.create_file();
        EXPECT_EQ(true, p.is_regular());
        
        p.reset_path(u8"aaa/bbb/ccc/ddd/u8/" + get_u8path_str());
        p.create_file();
        EXPECT_EQ(true, p.is_regular());
        EXPECT_EQ(true, amo::u8path::fullAppName().is_regular());
    }
    
}



TEST(path, equal_to) {

    {
        amo::path p("./");
        amo::path p2("./././\\");
        EXPECT_EQ(true, p.equal_to(p2));
        p2.reset_path(amo::path::fullAppDir());
        EXPECT_EQ(true, p.equal_to(p2));
    }
    
    
    {
        amo::u8path p("./");
        amo::u8path p2("./././\\");
        EXPECT_EQ(true, p.equal_to(p2));
        p2.reset_path(amo::u8path::fullAppDir());
        EXPECT_EQ(true, p.equal_to(p2));
    }
    
}

TEST(path, basename) {

    {
        amo::path p("././aaa.cc");
        EXPECT_EQ("aaa", p.basename());
        p.reset_path("aaaa");
        EXPECT_EQ("aaaa", p.basename());
        
        p.reset_path("aaaa/././.\\.\\bbb");
        EXPECT_EQ("bbb", p.basename());
        p.reset_path("aaaa/././.\\.\\bbb.b");
        EXPECT_EQ("bbb", p.basename());
    }
    
    
    {
        amo::u8path p(u8"././中文.cc");
        
        EXPECT_EQ(u8"中文", p.basename());
        
        p.reset_path(u8"aaaa");
        EXPECT_EQ(u8"aaaa", p.basename());
        
        p.reset_path(u8"aaaa/././.\\.\\中文");
        EXPECT_EQ(u8"中文", p.basename());
        p.reset_path(u8"中文/././.\\.\\中文.b");
        EXPECT_EQ(u8"中文", p.basename());
    }
}


TEST(path, change_extension) {

    {
        amo::path p("././aaa.cc");
        EXPECT_EQ(".aa", p.change_extension(".aa").extension().to_string());
        p.reset_path("aaaa");
        EXPECT_EQ(".aa", p.change_extension(".aa").extension().to_string());
    }
}

TEST(path, copy_to) {

    {
        amo::path p("./aaa/");
        EXPECT_EQ(true, p.copy_to("./bbb"));
    }
    /*{
        amo::path p("./aaa/");
        EXPECT_EQ(true, p.copy_to("./eee"));
    }*/
    {
        amo::path p("./aaa/bbb/ccc/ddd/u8/99/test.cc");
        EXPECT_EQ(true, p.copy_to("./aaa/bbb/ccc/ddd/u8/99/test3.cc3"));
    }
}


TEST(path, move_to) {

    {
        amo::path p("./aaa/bbb");
        EXPECT_EQ(true, p.move_to("./ccc/bbb"));
        
    }
    
    /*   {
           amo::path p("./eee/");
           EXPECT_EQ(true, p.move_to("C:/eee"));
    
       }*/
    
    {
        amo::u8path p(u8"./aaa/中文目录");
        EXPECT_EQ(true, p.move_to("./ccc/中文目录"));
        
    }
    {
        amo::path p("./ccc/bbb/ccc/ddd/u8/99/test.cc");
        EXPECT_EQ(true, p.move_to("./ccc/bbb/ccc/ddd/u8/99/test4.cc3"));
    }
}

TEST(path, rename) {

    {
        amo::path p("./ccc");
        EXPECT_EQ(true, p.rename("./ddd"));
    }
    {
        amo::path p("./ddd/bbb/ccc/ddd/u8/99/test4.cc3");
        EXPECT_EQ(true, p.rename("./ddd/bbb/ccc/ddd/u8/99/test5.cc3"));
    }
}

TEST(path, remove) {

    {
        amo::path p("./aaa");
        EXPECT_EQ(true, p.remove());
    }
    
    {
        amo::path p("./bbb");
        EXPECT_EQ(false, p.remove());
    }
    
    {
        amo::path p("./ccc");
        EXPECT_EQ(true, p.remove());
    }
    
    {
        amo::path p("./ddd");
        EXPECT_EQ(false, p.remove());
    }
    {
        amo::path p(get_ansi_path_dir());
        EXPECT_EQ(!win_or_linux(), p.exists());
        EXPECT_EQ(true, p.remove());
    }
    {
        amo::u8path p(get_u8path_dir());
        EXPECT_EQ(true, p.remove());
    }
}


TEST(path, remove_all) {

    {
        amo::path p("./aaa");
        EXPECT_EQ(true, p.remove_all());
    }
    
    {
        amo::path p("./bbb");
        EXPECT_EQ(true, p.remove_all());
    }
    
    {
        amo::path p("./ccc");
        EXPECT_EQ(true, p.remove_all());
    }
    
    {
        amo::path p("./ddd");
        EXPECT_EQ(true, p.remove_all());
    }
}

