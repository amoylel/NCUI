
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


namespace amo {
    class json;
}

int main(int argc, char** argv) {
    //try {
    //
    //
    //    std::wbuffer_convert<std::codecvt_utf8<wchar_t>> conv(std::cout.rdbuf());
    //    std::wostream out(&conv);
    //
    //    out.imbue(std::locale(out.getloc(),
    //                          new std::time_put_byname<wchar_t>("ja_JP")));
    //
    //    std::time_t t = std::time(NULL);
    //    out << std::put_time(std::localtime(&t), L"%A %c") << '\n';
    //
    //    std::wcout << "User-preferred locale setting is " << std::locale().name().c_str() << '\n';
    //    // 在启动时，全局本地环境是 "C" 本地环境
    //    std::wcout << 1000.01 << '\n';
    //    // 以用户偏好的本地环境替换 C++ 本地环境和 C 本地环境
    //    std::locale::global(std::locale(""));
    //    // 将来的宽字符输出使用新的全局本地环境
    //    std::wcout.imbue(std::locale());
    //    // 再次输出同一数字
    //    std::wcout << 1000.01 << '\n';
    //    const wchar_t* strzh = L"中文abc";
    //    std::locale lc("zh_CN");
    //    std::locale::global(lc);
    //    std::wcout << L"Zhong text is: " << strzh << std::endl;
    //} catch (const std::exception& e) {
    //    std::cout << e.what() << std::endl;
    //}
    //
    /*   std::locale lc("zh_CN");
       std::locale::global(lc);*/
    amo::path::set_work_path_to_app_path();
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    system("pause");
    return 0;
}