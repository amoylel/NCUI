// Created by amoylel on 10/10/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_BASIC_FILE_0389519A_BAB9_473D_B7A4_9D28CB0737BB_HPP__
#define AMO_BASIC_FILE_0389519A_BAB9_473D_B7A4_9D28CB0737BB_HPP__



#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <memory>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <amo/windows.hpp>
#include <amo/type_traits.hpp>
#include <amo/utility.hpp>
#include <amo/filestream.hpp>
#include <amo/path.hpp>
#include <amo/string.hpp>




namespace amo {
    template<typename CodingType>
    class basic_file {
    public:
    public:
        typedef amo::basic_path<CodingType> path_type;
    public:
        static StringCodingType get_coding_type() {
            return CodingType::coding_type;
        }
        
        static bool is_utf8() {
            return CodingType::coding_type == StringCodingType::CODING_TYPE_UTF8;
        }
    public:
        /*	ios::in		为输入(读)而打开文件
        ios::out	为输出(写)而打开文件
        ios::ate	初始位置:文件尾
        ios::app	所有输出附加在文件末尾
        ios::trunc	如果文件已存在则先删除该文件
        ios::binary	二进制方式*/
        basic_file(const path_type& p)
            : m_path(p) {
            
        }
        
        basic_string<CodingType> name() const {
            return basic_string<CodingType>(m_path.filename().generic_string(), is_utf8());
        }
        
        ~basic_file() {
        
        }
        
        std::shared_ptr<amo::basic_filestream<CodingType> > get_filestream(
            std::ios::openmode mode = std::ios::in | std::ios::out) {
            return std::make_shared<amo::basic_filestream<CodingType> >
                   (m_path, mode);
        }
        
        uint64_t size() const {
        
            return boost::filesystem::file_size(m_path.filesystem_path());
        }
        
        bool is_exists() const {
            return m_path.exists();
        }
        
        bool is_file() const {
            return m_path.is_file();
        }
        
        
        
        bool is_picture() {
            std::string type = file_type(true);
            
            if (type == "jpg"
                    || type == "png"
                    || type == "gif"
                    || type == "tif"
                    || type == "bmp"
                    || type == "dwg") {
                return true;
            }
            
            return false;
        }
        
        std::string file_type(bool bReal = false) {
            if (!bReal) {
                std::string str = m_path.extension().generic_string();
                return boost::replace_first_copy(str, ".", "");
                
            }
            
            static std::unordered_map<std::string, std::string> map = {
                { "ffd8ffe000104a464946", "jpg" }, //JPEG (jpg)
                { "89504e470d0a1a0a0000", "png" }, //PNG (png)
                { "47494638396126026f01", "gif" }, //GIF (gif)
                { "49492a00227105008037", "tif" }, //TIFF (tif)
                { "424d228c010000000000", "bmp" }, //16色位图(bmp)
                { "424d8240090000000000", "bmp" }, //24位位图(bmp)
                { "424d8e1b030000000000", "bmp" }, //256色位图(bmp)
                { "41433130313500000000", "dwg" }, //CAD (dwg)
                { "3c21444f435459504520", "html" }, //HTML (html)
                { "3c21646f637479706520", "htm" }, //HTM (htm)
                { "48544d4c207b0d0a0942", "css" }, //css
                { "696b2e71623d696b2e71", "js" }, //js
                { "7b5c727466315c616e73", "rtf" }, //Rich Text Format (rtf)
                { "38425053000100000000", "psd" }, //Photoshop (psd)
                { "46726f6d3a203d3f6762", "eml" }, //Email [Outlook Express 6] (eml)
                { "d0cf11e0a1b11ae10000", "doc" }, //MS Excel 注意:word、msi 和 excel的文件头一样
                { "d0cf11e0a1b11ae10000", "vsd" }, //Visio 绘图
                { "5374616E64617264204A", "mdb" }, //MS Access (mdb)
                { "252150532D41646F6265", "ps" },
                { "255044462d312e350d0a", "pdf" }, //Adobe Acrobat (pdf)
                { "2e524d46000000120001", "rmvb" }, //rmvb/rm相同
                { "464c5601050000000900", "flv" }, //flv与f4v相同
                { "00000020667479706d70", "mp4" },
                { "49443303000000002176", "mp3" },
                { "000001ba210001000180", "mpg" }, //
                { "3026b2758e66cf11a6d9", "wmv" }, //wmv与asf相同
                { "52494646e27807005741", "wav" }, //Wave (wav)
                { "52494646d07d60074156", "avi" },
                { "4d546864000000060001", "mid" }, //MIDI (mid)
                { "504b0304140000000800", "zip" },
                { "526172211a0700cf9073", "rar" },
                { "235468697320636f6e66", "ini" },
                { "504b03040a0000000000", "jar" },
                { "4d5a9000030000000400", "exe" },//可执行文件
                { "3c25402070616765206c", "jsp" },//jsp文件
                { "4d616e69666573742d56", "mf" },//MF文件
                { "3c3f786d6c2076657273", "xml" },//xml文件
                { "494e5345525420494e54", "sql" },//xml文件
                { "7061636b616765207765", "java" },//java文件
                { "406563686f206f66660d", "bat" },//bat文件
                { "1f8b0800000000000000", "gz" },//gz文件
                { "6c6f67346a2e726f6f74", "properties" },//bat文件
                { "cafebabe0000002e0041", "class" },//bat文件
                { "49545346030000006000", "chm" },//bat文件
                { "04000000010000001300", "mxp" },//bat文件
                { "504b0304140006000800", "docx" },//docx文件
                { "d0cf11e0a1b11ae10000", "wps" },//WPS文字wps、表格et、演示dps都是一样的
                { "6431303a637265617465", "torrent" }
            };
            unsigned char buffer[11] = { 0 };
            int num = read((char*)buffer, 10);
            
            if (num < 10) {
                return "";
            }
            
            
            std::string ss = amo::bytes_to_hex_string((unsigned char*)buffer, 10);
            /*   std::cout << "-----------------------" << std::endl;
            
            for (int i = 0; i < 10 ; ++i) {
            int d = buffer[i];
            std::cout << d << std::endl;
            }
            
            std::cout << "-----------------------" << std::endl;*/
            
            auto iter = map.find(ss);
            
            if (iter == map.end()) {
                return "";
            }
            
            return iter->second;
            
            
        }
        
        
        
        //复制文件
        bool copy_to(const amo::basic_string<CodingType>& to) {
            return m_path.copy_to(to.to_wide());
        }
        
        // 移动文件
        bool move_to(const amo::basic_string<CodingType>& to) {
            if (m_path.move_to(to.to_wide())) {
                m_path = to.to_wide();
                return true;
            }
            
            return false;
            
        }
        
        
        
        // 重命名文件
        bool rename(const amo::basic_string<CodingType>& to) {
            if (m_path.rename(to.to_wide())) {
                m_path = to.to_wide();
                return true;
            }
            
            return false;
        }
        
        // 删除文件
        bool remove() {
            return m_path.remove();
        }
        
        amo::basic_string<CodingType> get_path() {
            return amo::string_utils::wide_to_string(m_path.generic_wstring(),
                    get_coding_type());
        }
        
        const  amo::basic_string<CodingType> get_path() const {
            return amo::string_utils::wide_to_string(m_path.generic_wstring(),
                    get_coding_type());
        }
        
        path_type path() const {
            return m_path;
        }
    private:
        int read(char* buffer, int nCount) {
            std::shared_ptr<std::fstream> ifs;
            
            $windows({ ifs.reset(new std::fstream(m_path.generic_wstring(), std::ios::in | std::ios::binary)); })
            $welse({ ifs.reset(new std::fstream(m_path.generic_string(), std::ios::in | std::ios::binary)); })
            
            if (!ifs->is_open()) {
                return 0;
            }
            
            if (ifs->eof()) {
                return 0;
            }
            
            ifs->read(buffer, nCount);
            int nBytes = ifs->gcount();
            
            return nBytes;
        }
    private:
        path_type m_path;
        
    };
}




#endif //AMO_BASIC_FILE_0389519A_BAB9_473D_B7A4_9D28CB0737BB_HPP__