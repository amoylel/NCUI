#include <iostream>

#include <gtest/gtest.h>

#include <amo/filestream.hpp>


namespace {

    static std::string ansi_file_path1() {
        return "ansi_path1.txt";
    }
    
    static std::string ansi_file_path2() {
        return "中文路徑중국어 경로語のパスالمسار الصينية Chinese path2.txt";
    }
    
    static std::string utf8_file_path1() {
        return u8"utf8_path1.txt";
    }
    
    static std::string utf8_file_path2() {
        return u8"中文路徑중국어 경로語のパスالمسار الصينية Chinese path2.txt";
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

TEST(filestream, constructor) {

    // write
    {
        amo::filestream  fs1(ansi_file_path1(), std::ios::out);
        EXPECT_EQ(fs1.is_open(), true);
        
        amo::filestream fs2(ansi_file_path2(), std::ios::out);
        EXPECT_EQ(fs2.is_open(), true_or_false());
        
        amo::u8filestream  fs3(utf8_file_path1(), std::ios::out);
        EXPECT_EQ(fs3.is_open(), true);
        
        amo::u8filestream fs4(utf8_file_path2(), std::ios::out);
        EXPECT_EQ(fs4.is_open(), true);
    }
    // read
    {
        amo::filestream  fs1(ansi_file_path1(), std::ios::in);
        EXPECT_EQ(fs1.is_open(), true);
        
        amo::filestream fs2(ansi_file_path2(), std::ios::in);
        EXPECT_EQ(fs2.is_open(), true_or_false());
        
        amo::u8filestream  fs3(utf8_file_path1(), std::ios::in);
        EXPECT_EQ(fs3.is_open(), true);
        
        amo::u8filestream fs4(utf8_file_path2(), std::ios::in);
        EXPECT_EQ(fs4.is_open(), true);
    }
}
TEST(filestream, close) {
    amo::u8filestream fs4(utf8_file_path2(), std::ios::in);
    EXPECT_EQ(fs4.is_open(), true);
    fs4.close();
    EXPECT_EQ(fs4.is_open(), false);
}



TEST(filestream, write) {
    {
        amo::u8filestream fs(utf8_file_path2(), std::ios::out);
        EXPECT_EQ(fs.is_open(), true);
        fs.write("1234567890");
        std::string str = "1234567890";
        fs.write(str.c_str(), str.size());
    }
    
    {
        amo::u8filestream fs(utf8_file_path1(), std::ios::out);
        EXPECT_EQ(fs.is_open(), true);
        fs.write(u8"中文路徑중국어 경로語のパスالمسار الصينية Chinese path2.txt");
    }
}
TEST(filestream, read_all) {
    {
        amo::u8filestream fs(utf8_file_path1(), std::ios::in);
        EXPECT_EQ(fs.is_open(), true);
        std::string str = fs.read_all();
        EXPECT_EQ(str, utf8_file_path2());
    }
    
    {
        amo::u8filestream fs(utf8_file_path2(), std::ios::in);
        EXPECT_EQ(fs.is_open(), true);
        std::string str =  fs.read_all();
        EXPECT_EQ(str, "12345678901234567890");
    }
}

TEST(filestream, read_some) {
    amo::u8filestream fs(utf8_file_path2(), std::ios::in);
    EXPECT_EQ(fs.is_open(), true);
    std::string str(10, 0);
    fs.read_some((char*)str.c_str(), 10);
    EXPECT_EQ(str, "1234567890");
    
}
TEST(filestream, read_all_bytes) {
    {
        amo::u8filestream fs(utf8_file_path2(), std::ios::in);
        EXPECT_EQ(fs.is_open(), true);
        std::vector<int8_t> vec = fs.read_all_bytes();
        EXPECT_EQ(std::string((char*)vec.data(), vec.size()), "12345678901234567890");
    }
    
    {
        amo::u8filestream fs(utf8_file_path2(), std::ios::in);
        EXPECT_EQ(fs.is_open(), true);
        std::vector<int8_t> vec;
        fs.read_all_bytes(vec);
        EXPECT_EQ(std::string((char*)vec.data(), vec.size()), "12345678901234567890");
    }
}

TEST(filestream, read_line) {
    {
        amo::u8filestream fs(utf8_file_path2(), std::ios::in);
        EXPECT_EQ(fs.is_open(), true);
        std::string str;
        fs.read_line(str);
        EXPECT_EQ(str, "12345678901234567890");
    }
    
    
    {
        amo::u8filestream fs(utf8_file_path2(), std::ios::in);
        EXPECT_EQ(fs.is_open(), true);
        std::string str = fs.read_line();
        EXPECT_EQ(str, "12345678901234567890");
    }
}
TEST(filestream, get_path) {
    {
        amo::u8filestream fs(utf8_file_path2(), std::ios::in);
        EXPECT_EQ(fs.is_open(), true);
        std::string u8_path = fs.get_path().generic_u8string();
        EXPECT_EQ(u8_path, utf8_file_path2());
    }
}
TEST(filestream, eof) {
    amo::u8filestream fs4(utf8_file_path2(), std::ios::in);
    EXPECT_EQ(fs4.is_open(), true);
    
    EXPECT_EQ(fs4.eof(), false);
    
    char a[1] = { 0 };
    
    while (fs4.read_some(a, 1)) {
    
    }
    
    EXPECT_EQ(fs4.eof(), true);
}

TEST(filestream, remove) {

    amo::path(ansi_file_path1()).remove();
    amo::path(ansi_file_path2()).remove();
    amo::u8path(utf8_file_path1()).remove();
    amo::u8path(utf8_file_path2()).remove();
}
