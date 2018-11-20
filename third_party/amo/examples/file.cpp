#include <iostream>

#include <gtest/gtest.h>

#include <amo/file.hpp>
#include <amo/path.hpp>

namespace  {
    static std::string ansi_file_path3() {
        return "ansi_file_path3.txt";
    }
    
    static std::string ansi_file_path2() {
        return "中文路徑중국어 경로語のパスالمسار الصينية file.txt";
    }
    
    static std::string utf8_file_path1() {
        return u8"utf8_path1.txt";
    }
    
    static std::string utf8_file_path5() {
        return u8"中文路徑중국어 경로語のパスالمسار الصينية file.txt";
    }
    
    
}
TEST(file, constructor) {
    {
        amo::file f1(ansi_file_path3());
        EXPECT_EQ(f1.is_exists(), false);
        f1.path().create_file();
        EXPECT_EQ(f1.is_exists(), true);
        EXPECT_EQ(f1.name() == ansi_file_path3(), true);
        
    }
    
    {
        amo::u8file f1 = amo::u8path(utf8_file_path5());
        EXPECT_EQ(f1.is_exists(), false);
        f1.path().create_file();
        EXPECT_EQ(f1.is_exists(), true);
        EXPECT_EQ(f1.name() == utf8_file_path5(), true);
    }
    
    
}

TEST(file, name) {
    {
        amo::file f1(ansi_file_path3());
        EXPECT_EQ(f1.name() == ansi_file_path3(), true);
    }
    
    {
        amo::u8file f1 = amo::u8path(utf8_file_path5());
        EXPECT_EQ(f1.name() == utf8_file_path5(), true);
    }
}

TEST(file, size) {
    {
        amo::file f1(ansi_file_path3());
        EXPECT_EQ(f1.size(), 0);
    }
    
    {
        amo::u8file f1 = amo::u8path(utf8_file_path5());
        EXPECT_EQ(f1.size(), 0);
    }
}


TEST(file, is_file) {
    {
        amo::file f1(ansi_file_path3());
        EXPECT_EQ(f1.is_file(), true);
    }
    
    {
        amo::u8file f1 = amo::u8path(utf8_file_path5());
        EXPECT_EQ(f1.is_file(), true);
    }
}


TEST(file, is_picture) {
    {
        amo::file f1(ansi_file_path3());
        EXPECT_EQ(f1.is_picture(), false);
        f1 = amo::path("./aa.jpg");
        EXPECT_EQ(f1.is_picture(), false);
        f1.path().create_file();
        EXPECT_EQ(f1.is_picture(), false);
    }
    
    {
        amo::u8file f1 = amo::u8path(utf8_file_path5());
        EXPECT_EQ(f1.is_picture(), false);
    }
}

TEST(file, file_type) {
    {
        amo::file f1(ansi_file_path3());
        EXPECT_EQ(f1.file_type(), "txt");
        f1 = amo::path("aa.jpg");
        EXPECT_EQ(f1.file_type(), "jpg");
    }
    
    {
        amo::u8file f1 = amo::u8path(utf8_file_path5());
        EXPECT_EQ(f1.file_type(), "txt");
    }
}

TEST(file, copy_to) {
    {
        amo::file f1(ansi_file_path3());
        f1.copy_to(L"file_test_str1.txt");
        EXPECT_EQ(amo::path(L"./file_test_str1.txt").exists(), true);
        
    }
    
    {
        amo::u8file f1 = amo::u8path(utf8_file_path5());
        f1.copy_to(L"file_test_str2.txt");
        EXPECT_EQ(amo::u8path(L"./file_test_str2.txt").exists(), true);
    }
}

TEST(file, rename) {
    {
        amo::file f1(ansi_file_path3());
        f1.rename(L"file_test_str3.txt");
        EXPECT_EQ(amo::path(L"./file_test_str3.txt").exists(), true);
        
    }
    
    {
        amo::u8file f1 = amo::u8path(utf8_file_path5());
        f1.rename(L"file_test_str4.txt");
        EXPECT_EQ(amo::u8path(L"./file_test_str4.txt").exists(), true);
    }
}

TEST(file, get_filestream) {
    {
        amo::file f1(ansi_file_path3());
        auto ptr = f1.get_filestream(std::ios::out | std::ios::binary);
        ptr->write("1234567890");
        ptr->close();
        ptr.reset();
        EXPECT_EQ(f1.size(), 10);
    }
    
}

TEST(file, remove) {
    {
        amo::file f1(ansi_file_path3());
        EXPECT_EQ(f1.is_exists(), true);
        
        f1.remove();
        EXPECT_EQ(f1.is_exists(), false);
        
        f1 = amo::path(L"./file_test_str1.txt");
        EXPECT_EQ(f1.is_exists(), true);
        f1.remove();
        EXPECT_EQ(f1.is_exists(), false);
        
        f1 = amo::path(L"./file_test_str3.txt");
        EXPECT_EQ(f1.is_exists(), true);
        f1.remove();
        EXPECT_EQ(f1.is_exists(), false);
        f1 = amo::path(L"./aa.jpg");
        EXPECT_EQ(f1.is_exists(), true);
        f1.remove();
        EXPECT_EQ(f1.is_exists(), false);
        
    }
    
    {
        amo::u8file f1 = amo::u8path(utf8_file_path5());
        EXPECT_EQ(f1.is_exists(), false);
        f1.path().create_file();
        EXPECT_EQ(f1.is_exists(), true);
        f1.remove();
        EXPECT_EQ(f1.is_exists(), false);
        
        f1 = amo::u8path(L"./file_test_str2.txt");
        EXPECT_EQ(f1.is_exists(), true);
        f1.remove();
        EXPECT_EQ(f1.is_exists(), false);
        
        f1 = amo::u8path(L"./file_test_str4.txt");
        EXPECT_EQ(f1.is_exists(), true);
        f1.remove();
        EXPECT_EQ(f1.is_exists(), false);
        
    }
}
