// Created by amoylel on 30/03/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_FILE2_654AB7F2_58C2_4F7D_95B9_D7771569C549_HPP__
#define AMO_FILE2_654AB7F2_58C2_4F7D_95B9_D7771569C549_HPP__






#include <amo/path.hpp>
#include <amo/string.hpp>
#include <fstream>
#include <amo/utility.hpp>

namespace amo {
    class File2 {
    public:
    
        /*	ios::in		为输入(读)而打开文件
        	ios::out	为输出(写)而打开文件
        	ios::ate	初始位置:文件尾
        	ios::app	所有输出附加在文件末尾
        	ios::trunc	如果文件已存在则先删除该文件
        	ios::binary	二进制方式*/
        File2(const std::string& name,
              std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
            : m_path(name) {
            if (mode & std::ios_base::in) {
                // 如果是读文件，那么在文件存在的情况下打开文件
                if (is_exists() && is_file()) {
                    ifs.reset(new std::fstream(m_path.c_str(), mode));
                }
            } else {
                // 如果是写文件，不管文件是否存在，都先打开
                ifs.reset(new std::fstream(m_path.c_str(), mode));
            }
            
        }
        
        File2(const amo::path& p,
              std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
            : m_path(p) {
            if (mode & std::ios_base::in) {
                // 如果是读文件，那么在文件存在的情况下打开文件
                if (is_exists() && is_file()) {
                    ifs.reset(new std::fstream(m_path.c_str(), mode));
                }
            } else {
                // 如果是写文件，不管文件是否存在，都先打开
                ifs.reset(new std::fstream(m_path.c_str(), mode));
            }
        }
        
        std::string name() const {
            amo::path p = m_path;
            p.strip_path().remove_extension();
            return p.c_str();
        }
        
        ~File2() {
            close();
        }
        
        uint64_t size() const {
        
            WIN32_FILE_ATTRIBUTE_DATA fad;
            uint64_t nSize = 0;
            
            if (GetFileAttributesExA(m_path.c_str(), GetFileExInfoStandard, &fad)) {
                nSize += ((ULONG64)fad.nFileSizeHigh & 0xffffffff) << 32 |
                         (ULONG64)fad.nFileSizeLow;
            }
            
            return nSize;
            
            /* std::ifstream ifs(m_path.c_str());
            
             if (!ifs.is_open()) {
                 return 0;
             }
            
            
             ifs.seekg(0, std::ios_base::end);
             std::streampos sp = ifs.tellg();
             return sp;*/
        }
        
        bool is_exists() const {
            return m_path.file_exists();
        }
        
        bool is_file() const {
            return true;
            //TODO: 这里有问题
            return m_path.is_file_spec();
        }
        
        
        // 解密
        void decrypt() {
        
        }
        
        // 加密
        void encrypt() {
        
        }
        
        bool is_open() {
            if (!ifs) {
                return false;
            }
            
            return ifs->is_open();
        }
        
        std::string read_header() {
            if (!ifs) {
                return "";
            }
            
            return "";
        }
        
        bool is_picture()   {
            std::string type = get_type(true);
            
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
        
        std::string get_type(bool bReal = false)  {
            if (!bReal) {
                return amo::string_utils::trim_left_copy(m_path.find_extension(), ".");
            }
            
            static std::unordered_map<std::string, std::string> map = {
                {"ffd8ffe000104a464946", "jpg"}, //JPEG (jpg)
                {"89504e470d0a1a0a0000", "png"}, //PNG (png)
                {"47494638396126026f01", "gif"}, //GIF (gif)
                {"49492a00227105008037", "tif"}, //TIFF (tif)
                {"424d228c010000000000", "bmp"}, //16色位图(bmp)
                {"424d8240090000000000", "bmp"}, //24位位图(bmp)
                {"424d8e1b030000000000", "bmp"}, //256色位图(bmp)
                {"41433130313500000000", "dwg"}, //CAD (dwg)
                {"3c21444f435459504520", "html"}, //HTML (html)
                {"3c21646f637479706520", "htm"}, //HTM (htm)
                {"48544d4c207b0d0a0942", "css"}, //css
                {"696b2e71623d696b2e71", "js"}, //js
                {"7b5c727466315c616e73", "rtf"}, //Rich Text Format (rtf)
                {"38425053000100000000", "psd"}, //Photoshop (psd)
                {"46726f6d3a203d3f6762", "eml"}, //Email [Outlook Express 6] (eml)
                {"d0cf11e0a1b11ae10000", "doc"}, //MS Excel 注意:word、msi 和 excel的文件头一样
                {"d0cf11e0a1b11ae10000", "vsd"}, //Visio 绘图
                {"5374616E64617264204A", "mdb"}, //MS Access (mdb)
                {"252150532D41646F6265", "ps"},
                {"255044462d312e350d0a", "pdf"}, //Adobe Acrobat (pdf)
                {"2e524d46000000120001", "rmvb"}, //rmvb/rm相同
                {"464c5601050000000900", "flv"}, //flv与f4v相同
                {"00000020667479706d70", "mp4"},
                {"49443303000000002176", "mp3"},
                {"000001ba210001000180", "mpg"}, //
                {"3026b2758e66cf11a6d9", "wmv"}, //wmv与asf相同
                {"52494646e27807005741", "wav"}, //Wave (wav)
                {"52494646d07d60074156", "avi"},
                {"4d546864000000060001", "mid"}, //MIDI (mid)
                {"504b0304140000000800", "zip"},
                {"526172211a0700cf9073", "rar"},
                {"235468697320636f6e66", "ini"},
                {"504b03040a0000000000", "jar"},
                {"4d5a9000030000000400", "exe"},//可执行文件
                {"3c25402070616765206c", "jsp"},//jsp文件
                {"4d616e69666573742d56", "mf"},//MF文件
                {"3c3f786d6c2076657273", "xml"},//xml文件
                {"494e5345525420494e54", "sql"},//xml文件
                {"7061636b616765207765", "java"},//java文件
                {"406563686f206f66660d", "bat"},//bat文件
                {"1f8b0800000000000000", "gz"},//gz文件
                {"6c6f67346a2e726f6f74", "properties"},//bat文件
                {"cafebabe0000002e0041", "class"},//bat文件
                {"49545346030000006000", "chm"},//bat文件
                {"04000000010000001300", "mxp"},//bat文件
                {"504b0304140006000800", "docx"},//docx文件
                {"d0cf11e0a1b11ae10000", "wps"},//WPS文字wps、表格et、演示dps都是一样的
                {"6431303a637265617465", "torrent"}
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
        
        // 打开文件
        void open(std::ios_base::openmode mode = std::ios_base::in |
                  std::ios_base::out) {
            if (!ifs) {
                ifs.reset(new std::fstream());
            }
            
            ifs->open(m_path.c_str(), mode);
        }
        
        void close() {
            if (ifs) {
                ifs->close();
            }
            
            ifs.reset();
        }
        
        // 写入文件
        void write(const std::string& str) {
            if (!ifs) {
                return;
            }
            
            ifs->write(str.c_str(), str.size());
        }
        
        void write(const char* str, int nSize) {
            if (!ifs) {
                return;
            }
            
            ifs->write(str, nSize);
        }
        void write(const std::vector<int8_t>& vec) {
            if (!ifs) {
                return;
            }
            
            ifs->write((char*)vec.data(), vec.size());
        }
        // 读取整个文件
        std::string read() {
            std::ifstream ifs(m_path.c_str());
            std::stringstream buffer;
            buffer << ifs.rdbuf();
            return buffer.str();
        }
        
        /*!
         * @fn	int file::read(char* buffer, int nCount)
         *
         * @brief	不能读在正使用的文件。。.
         *
         * @param [in,out]	buffer	If non-null, the buffer.
         * @param	nCount		  	Number of.
         *
         * @return	An int.
         */
        
        int read(char* buffer, int nCount) {
            if (!ifs) {
                return 0;
            }
            
            if (ifs->eof()) {
                return 0;
            }
            
            ifs->read(buffer, nCount);
            int nBytes = ifs->gcount();
            
            
            
            return nBytes;
        }
        
        int readsome(char* buffer, int nCount) {
            if (!ifs) {
                return 0;
            }
            
            if (ifs->eof()) {
                return 0;
            }
            
            return ifs->readsome(buffer, nCount);
        }
        
        std::vector<int8_t> read_all_bytes() {
            std::vector<int8_t> vec;
            std::ifstream ifs(m_path.c_str());
            std::filebuf* pbuf = ifs.rdbuf();
            // get file size using buffer's members
            std::size_t size = pbuf->pubseekoff(0, ifs.end, ifs.in);
            vec.resize(size);
            pbuf->sgetn((char*)vec.data(), size);
            ifs.close();
            return vec;
        }
        
        bool read_all_bytes(std::vector<int8_t>& vec) {
            std::ifstream ifs(m_path.c_str());
            std::filebuf* pbuf = ifs.rdbuf();
            // get file size using buffer's members
            std::size_t size = pbuf->pubseekoff(0, ifs.end, ifs.in);
            vec.resize(size);
            pbuf->sgetn((char*)vec.data(), size);
            ifs.close();
            return true;
        }
        // 按行读取
        std::vector<std::string> read_all_lines() {
            std::vector<std::string> vec;
            
            char str[4096] = { 0 };
            std::ifstream ifs(m_path.c_str());
            
            while (ifs.getline(str, 4096)) {
                vec.push_back(str);
                memset(str, 0, 4096);
            }
            
            
            return std::move(vec);
        }
        
        // 读取一行数据
        bool read_line(std::string & val) {
            char str[4096] = { 0 };
            
            if (ifs->getline(str, 4096)) {
                val = str;
                return true;
            }
            
            return false;
        }
        
        // 不能判断是空行还是失败
        std::string read_line() {
            char str[4096] = { 0 };
            
            if (ifs->getline(str, 4096)) {
                return str;
            }
            
            return "";
        }
        
        
        // 删除文件
        bool remove() {
            close();
            bool bOk =  removeFile(m_path);
            
            if (bOk) {
                m_path = amo::path();
            }
            
            return bOk;
        }
        
        // 移动文件
        bool move_to(const amo::path& to) {
            close();
            return moveFile(to, m_path);
        }
        
        //复制文件
        bool copy_to(const amo::path& to) {
            close();
            return copyFile(to, m_path);
        }
        
        // 重命名文件
        bool rename(const amo::path& to) {
            close();
            
            bool bOk =  renameFile(to, m_path);
            
            if (bOk) {
                m_path = to;
            }
            
            return bOk;
        }
        amo::path& get_path() {
            return m_path;
        }
        
        const amo::path& get_path() const {
            return m_path;
        }
    private:
        bool renameFile(const amo::path& to, const amo::path& from) {
            return moveFile(to, from);
        }
        
        bool moveFile(const amo::path& to, const amo::path& from) {
            return ::MoveFileA(from.c_str(), to.c_str()) != FALSE;
        }
        
        // 删除文件
        bool removeFile(const amo::path& from) {
        
            return ::DeleteFileA(from.c_str()) != FALSE;
        }
        
        bool copyFile(const amo::path& to, const amo::path& from) {
            return ::CopyFileA(from.c_str(), to.c_str(), FALSE) != FALSE;
        }
        
    private:
        amo::path m_path;
        std::shared_ptr<std::fstream> ifs;
        
    };
}


#endif //AMO_FILE2_654AB7F2_58C2_4F7D_95B9_D7771569C549_HPP__