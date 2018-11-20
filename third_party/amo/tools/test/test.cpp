// test.cpp : 定义控制台应用程序的入口点。
//



#include <gtest/gtest.h>
#include  <amo/path.hpp>

#ifdef OS_WIN
#ifdef _DEBUG
#pragma comment(lib, "gtestd.lib")
#pragma comment(lib, "gtest_maind.lib")
#else
#pragma comment(lib, "gtest.lib")
#pragma comment(lib, "gtest_main.lib")
#endif

#endif // OS_WIN
//
//#ifdef WIN32
////#pragma execution_character_set("utf-8")
//#include <windows.h>
//#endif


#include <boost/lexical_cast.hpp>
#include <sstream>
#include <iostream>
#include <string>


//
//static std::locale  get_system_locale() {
//    std::string code_page = boost::locale::util::get_system_locale();
//    return boost::locale::generator().generate(code_page);
//}
//
//static std::string ansi_to_utf8(const std::string& src) {
//    std::string from_encoding = std::use_facet<boost::locale::info>
//                                (get_system_locale()).encoding();
//    return  boost::locale::conv::between(src, "UTF-8", from_encoding);
//}
//
//static std::wstring ansi_to_wide(const std::string& src) {
//    return  boost::locale::conv::to_utf<wchar_t>(src, get_system_locale());
//}
//
//static std::wstring utf8_to_wide(const std::string& src) {
//    return  boost::locale::conv::utf_to_utf<wchar_t>(src);
//}
//
//static std::string utf8_to_ansi(const std::string& src) {
//    std::string from_encoding = std::use_facet<boost::locale::info>
//                                (get_system_locale()).encoding();
//    return  boost::locale::conv::from_utf(src, from_encoding);
//}
//
//static std::string wide_to_ansi(const std::wstring& src) {
//    std::string from_encoding = std::use_facet<boost::locale::info>
//                                (get_system_locale()).encoding();
//    return boost::locale::conv::from_utf(src, from_encoding);
//}
//
//static std::string wide_to_utf8(const std::wstring& src) {
//    return boost::locale::conv::utf_to_utf<char>(src);
//}




//static std::wstring utf8_to_wide(const std::string& utf8string);
//static std::string utf8_to_ansi(const std::string& utf8string);
//static std::string wide_to_utf8(const std::wstring& widestring);
//static std::string wide_to_ansi(const std::wstring& widestring);
//static std::wstring ansi_to_wide(const std::string& ansistring);
//static std::string ansi_to_utf8(const std::string& ansistring);
//
//static std::string utf8_to_string(const std::string& utf8string,
//                                  StringCodingType codingtype);
//static std::string ansi_to_string(const std::string& ansistring,
//                                  StringCodingType codingtype);
//static std::string wide_to_string(const std::wstring& widestring,
//                                  StringCodingType codingtype);
//static std::wstring string_to_wide(const std::string& anystring,
//                                   StringCodingType codingtype);
//static std::string string_to_utf8(const std::string& anystring,
//                                  StringCodingType codingtype);
//static std::string string_to_ansi(const std::string& anystring,
//                                  StringCodingType codingtype);

std::wstring toutf(const std::string & src) {

    std::string strCodePage = boost::locale::util::get_system_locale();
    std::locale loc = boost::locale::generator().generate(strCodePage);
    std::cout << "\n  charset of std::locale(\"\") is "
              << std::use_facet<boost::locale::info>(loc).encoding()
              << std::endl;
    //std::locale loc = boost::locale::generator().generate("");
    std::wstring dst = boost::locale::conv::to_utf<wchar_t>(src, loc);
    return dst;
}

int main(int argc, char *argv[]) {
    try {
        amo::path::set_work_path_to_app_path();
        
        /*auto cccc =    std::locale::global(std::locale("GBK"));
        auto stttttt = cccc.name();*/
        std::string strCodePage = boost::locale::util::get_system_locale();
        std::cout << strCodePage << std::endl;
        
        std::locale loc = boost::locale::generator().generate(strCodePage);
        
        /*codepage = "CP" + boost::lexical_cast<std::string>(cpinfo.CodePage);*/
        
        //std::locale loc = boost::locale::generator().generate("");
        
        std::wstring a = L"1: Five Chinese words[白日依山尽]_by macro L";
        std::wstring b = toutf(
                             std::string("2: Five Chinese words[黄河入海流]_by boost to_utf"));
        std::wstring c = L"3: Five Chinese words[欲穷千里目]_by macro L";
        std::wcout.imbue(std::locale(""));
        std::wcout << L"白日依山尽" << std::endl;
        
        std::wcout << a << std::endl;
        std::wcout << b << std::endl;
        std::wcout << c << std::endl;
        
        std::cout << "\n  charset of std::locale(\"\") is "
                  << std::use_facet<boost::locale::info>(loc).encoding()
                  << std::endl;
                  
                  
                  
        std::string source = "BIG5字符串";
        std::string u8source = u8"UTF8字符串";
        std::string sr = std::use_facet<boost::locale::info>
                         (loc).encoding();
        std::string s = boost::locale::conv::between(source, "UTF-8", sr);
        
        std::wstring dst = boost::locale::conv::to_utf<wchar_t>(source, sr);
        std::string u8dst = boost::locale::conv::to_utf<char>(source, sr);
        
        {
            std::wstring  ss1 = boost::locale::conv::utf_to_utf<wchar_t>(dst);
            std::wstring  ss2 = boost::locale::conv::utf_to_utf<wchar_t>(u8dst);
            std::wcout << ss1 << std::endl;
            std::wcout << ss2 << std::endl;
        }
        {
            std::string  ss1 = boost::locale::conv::from_utf(dst, sr);
            std::string  ss2 = boost::locale::conv::from_utf(u8dst, sr);
            std::cout << ss1 << std::endl;
            std::cout << ss2 << std::endl;
        }
        std::cout << amo::string_utils::utf8_to_ansi(s) << std::endl;
        
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}
