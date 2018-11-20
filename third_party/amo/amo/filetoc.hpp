// Created by amoylel on 07/07/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_FILETOC_8E765103_9F10_4F6D_AEF1_1F037C593FEA_HPP__
#define AMO_FILETOC_8E765103_9F10_4F6D_AEF1_1F037C593FEA_HPP__



#include <amo/json.hpp>
#include <amo/json_helper.hpp>
#include <amo/filestream.hpp>
#include <amo/path.hpp>
#include <amo/logger.hpp>


static const amo::string SINGLE_HEADER_TEMPLATE_HPP("\
#ifndef AMO_{uuid_lines}_HPP\n\
#define AMO_{uuid_lines}_HPP\n\
\n\
#include <string>\n\
#include <unordered_map>\n\
\n\
{namespace_begin}\n\
\n\
    {source_lines}\n\
\n\
    static const std::unordered_map<std::string, std::string> name_to_path_map = {\n\
        {name_to_path_lines}\n\
    };\n\
\n\
    static const std::unordered_map<std::string, const char*> name_to_var_map = {\n\
        {var_lines}\n\
    };\n\
\n\
	static const std::unordered_map<std::string, int> name_to_len_map = {\n\
        {len_lines}\n\
    };\n\
\n\
	static const std::unordered_map<std::string,  std::string> path_to_name_map = {\n\
        {path_to_name_lines}\n\
    };\n\
{namespace_end}\n\
#endif AMO_{uuid_lines}_HPP\n\n\
", false);

static const amo::string HEADER_TEMPLATE_H("\
#ifndef AMO_{uuid_lines}_H\n\
#define AMO_{uuid_lines}_H\n\
\n\
#include <string>\n\
#include <unordered_map>\n\
\n\
{namespace_begin}\n\
\n\
    {native_lines}\n\
\n\
    extern std::unordered_map<std::string, std::string> name_to_path_map;\n\
\n\
    extern std::unordered_map<std::string, const char*> name_to_var_map;\n\
\n\
	extern std::unordered_map<std::string, int> name_to_len_map;\n\
\n\
    extern std::unordered_map<std::string, std::string> path_to_name_map;\n\
{namespace_end}\n\n\
#endif AMO_{uuid_lines}_H\n\
", false);

static const amo::string SOURCE_TEMPLATE_CPP("\
#include \"stdafx.h\"\n\
#include <string>\n\
\n\
#include <unordered_map>\n\
\n\
{namespace_begin}\n\
\n\
    {source_lines}\n\
\n\
    extern std::unordered_map<std::string, std::string> name_to_path_map = {\n\
        {name_to_path_lines}\n\
    };\n\
\n\
    extern std::unordered_map<std::string, const char*> name_to_var_map = {\n\
        {var_lines}\n\
    };\n\
\n\
	extern std::unordered_map<std::string, int> name_to_len_map = {\n\
        {len_lines}\n\
    };\n\
\n\
	extern std::unordered_map<std::string,  std::string> path_to_name_map = {\n\
        {path_to_name_lines}\n\
    };\n\
{namespace_end}\n\
", false);


static const amo::string  NATIVE_DECLARATION("extern const char {var_name}[];\n\n    ", false);					// 文件变量声明
static const amo::string  SOURCE_DECLARATION("extern const char {var_name}[] = {data};\n\n    ", false);			// 文件数据定义
static const amo::string  SINGLE_SOURCE_DECLARATION("static const char {var_name}[] = {data};\n\n    ", false);			// 单个文件数据定义

static const amo::string  NAME_TO_LEN_DECLARATION("{\"{var_name}\", {length}},\n        ", false);
static const amo::string  NAME_TO_VAR_DECLARATION("{\"{var_name}\", {var_name}},\n        ", false);
static const amo::string  NAME_TO_PATH_DECLARATION("{\"{var_name}\", \"{file_path}\"},\n        ", false);
static const amo::string  PATH_TO_NAME_DECLARATION("{\"{file_path}\", \"{var_name}\"},\n        ", false);

namespace amo {



    class file2c {
    public:
        class file_item : public json_object {
        public:
            file_item() {
                length = 0;
            }
            AMO_ENTITY_ARGS_GET_BEGIN(file_item)
            AMO_ENTITY_ARGS_GET(var_name)
            AMO_ENTITY_ARGS_GET(file_path)
            AMO_ENTITY_ARGS_GET(data)
            AMO_ENTITY_ARGS_GET(length)
            AMO_ENTITY_ARGS_GET_END()
            
            AMO_ENTITY_ARGS_SET_BEGIN(file_item)
            AMO_ENTITY_ARGS_SET(var_name)
            AMO_ENTITY_ARGS_SET(file_path)
            AMO_ENTITY_ARGS_SET(data)
            AMO_ENTITY_ARGS_SET(length)
            AMO_ENTITY_ARGS_SET_END()
        public:
            std::string var_name;				// 变量名
            std::string file_path;				// 文件路径
            std::string data;					// 转换后的原始数据
            int length;							// 长度
        };
        
        class settings : public json_object {
        public:
            settings() {
                header_only = false;
                is_utf8 = false;
            }
            AMO_ENTITY_ARGS_GET_BEGIN(settings)
            AMO_ENTITY_ARGS_GET(output_path)
            AMO_ENTITY_ARGS_GET(header_only)
            AMO_ENTITY_ARGS_GET(name_space)
            AMO_ENTITY_ARGS_GET(files)
            AMO_ENTITY_ARGS_GET(is_utf8)
            AMO_ENTITY_ARGS_GET_END()
            
            AMO_ENTITY_ARGS_SET_BEGIN(settings)
            AMO_ENTITY_ARGS_SET(header_only)
            AMO_ENTITY_ARGS_SET(output_path)
            AMO_ENTITY_ARGS_SET(name_space)
            AMO_ENTITY_ARGS_SET(files)
            AMO_ENTITY_ARGS_SET(is_utf8)
            AMO_ENTITY_ARGS_SET_END()
        public:
            amo::json files;					// 需要转换的文件路径
            bool header_only;					// 是否只生成头文件，默认为false
            std::string output_path;			// 输出文件路径
            std::string name_space;				// 名字空间名称
            bool is_utf8;						// 配置文件是否为utf8编码
            
        };
    public:
        file2c(const amo::json & settings) {
            amo::json json = settings;
            m_settings = settings::fromJson(json);
            
            if (m_settings->output_path.empty()) {
                $cerr("in valid output path");
            }
        }
        
        
        void operator()() {
            return build();
        }
        
        void build() {
            if (m_settings->output_path.empty()) {
                return;
            }
            
            to_binary_string();
            
            if (m_settings->header_only) {
                make_single_header_file();
            } else {
                make_header_file();
                make_source_file();
            }
            
            return;
        }
        
        // 将文件转换为二进制字符串
        void to_binary_string() {
        
            std::vector<amo::json> vec = m_settings->files.to_array();
            
            for (auto& p : vec) {
                $cdevel("parse file:{}",	p.to_string());
                std::shared_ptr<file_item> item(new file_item());
                
                if (p.is_string()) {
                    item->file_path = p.get<std::string>();
                } else if (p.is_object()) {
                    item->var_name = p.getString("name");
                    item->file_path = p.getString("path");
                    
                } else {
                    $cerr("unsupported type:{} ", p.to_string());
                    continue;
                }
                
                if (item->file_path.empty()) {
                    $cerr("empty file path");
                    continue;
                }
                
                
                amo::path path(amo::string(item->file_path, m_settings->is_utf8));
                
                if (!path.exists()) {
                    $cerr("file is not exists:{} ", item->file_path);
                    continue;
                }
                
                std::string file_path = item->file_path;
                amo::filestream ifs(file_path, std::ios::in | std::ios::binary);
                
                char a = '0';
                std::stringstream stream;
                stream << "{ ";
                /*      amo::path p2("D:\\");
                      p2.append(item->file_path);
                      p2.parent().create_directory();
                
                      amo::path p3("D:\\111d");
                      p3.append(item->file_path);
                      p3.parent().create_directory();*/
                
                while (ifs.read_some(&a, 1)) {
                    stream << int((uint8_t)a) << ",";
                    item->length += 1;
                    
                }
                
                stream << "0 }";
                
                
                path.remove_extension();
                $windows({
                    path.canonicalize(true);
                });
                $welse({
                    path.canonicalize(false);
                });
                
                
                std::string var_name = path.string();
                amo::string_utils::trim(var_name, " ");
                amo::string_utils::replace(var_name, "\\", "_");
                amo::string_utils::replace(var_name, "/", "_");
                amo::string_utils::replace(var_name, ".", "_");
                
                if (item->var_name.empty()) {
                    item->var_name = var_name;
                    
                }
                
                if (item->var_name.empty()) {
                    $cerr("empty var name:{} ", path.to_string());
                    
                    continue;
                }
                
                
                bool invalid_val_name = false;
                
                for (size_t i = 0; i < item->var_name.size(); ++i) {
                    auto& p = item->var_name[i];
                    
                    if (i == 0 && (p >= '0' && p <= '9')) {
                        $cerr("only [a-zA-Z_] supported at first char:{} ", item->var_name);
                        invalid_val_name = true;
                        break;
                    }
                    
                    if ((p >= '0' && p <= '9')
                            || (p >= 'A' && p <= 'Z')
                            || (p >= 'a' && p <= 'z')
                            || p == '_') {
                            
                    } else {
                        $cerr("only [a-zA-Z_] supported:{} ", item->var_name);
                        invalid_val_name = true;
                        break;
                    }
                }
                
                if (invalid_val_name) {
                    $cerr("invalid var name:{} ", item->var_name);
                    continue;
                }
                
                std::string data = stream.str();
                
                item->data = stream.str();
                
                m_items.push_back(item);
                
            }
            
        }
        
        // 生成头文件内容
        void make_header_file() {
            amo::json json;
            std::pair<std::string, std::string> name_space = parse_namespace();
            json.put("namespace_begin", name_space.first);
            json.put("namespace_end", name_space.second);
            std::string uuid_lines = amo::uuid().to_string();
            amo::string_utils::replace(uuid_lines, "-", "_");
            amo::string_utils::to_upper(uuid_lines);
            
            json.put("uuid_lines", uuid_lines);
            
            std::string native_lines;
            
            for (auto& p : m_items) {
                native_lines += NATIVE_DECLARATION.format(p->toJson());
                
            }
            
            json.put("native_lines", native_lines);
            
            
            
            
            amo::string sb = HEADER_TEMPLATE_H.format(json);
            
            
            amo::path outPath(amo::string(m_settings->output_path, m_settings->is_utf8));
            outPath.rename_extension(".h");
            outPath.parent().create_directory();
            std::ofstream ofs(outPath.c_str(), std::ios::out | std::ios::binary);
            ofs << sb.str();
            ofs.close();
        }
        
        // 生成单个文件
        void make_single_header_file() {
        
            amo::json json;
            std::pair<std::string, std::string> name_space = parse_namespace();
            json.put("namespace_begin", name_space.first);
            json.put("namespace_end", name_space.second);
            
            std::string uuid_lines = amo::uuid().to_string();
            amo::string_utils::replace(uuid_lines, "-", "_");
            amo::string_utils::to_upper(uuid_lines);
            json.put("uuid_lines", uuid_lines);
            
            
            std::string source_lines;
            std::string path_to_name_lines;
            std::string var_lines;
            std::string len_lines;
            std::string name_to_path_lines;
            
            for (auto& p : m_items) {
                amo::json json = p->toJson();
                source_lines += SINGLE_SOURCE_DECLARATION.format(json);
                path_to_name_lines += PATH_TO_NAME_DECLARATION.format(json);
                var_lines += NAME_TO_VAR_DECLARATION.format(json);
                len_lines += NAME_TO_LEN_DECLARATION.format(json);
                name_to_path_lines += NAME_TO_PATH_DECLARATION.format(json);
            }
            
            amo::string_utils::trim_right(path_to_name_lines, ", \n");
            amo::string_utils::trim_right(var_lines, ", \n");
            amo::string_utils::trim_right(len_lines, ", \n");
            amo::string_utils::trim_right(name_to_path_lines, ", \n");
            
            /*  path_to_name_lines += "{\"\", \"\"}";
              var_lines += "{\"\", NULL}";
              len_lines += "{\"\", 0}";
              name_to_path_lines += "{\"\", \"\"}";*/
            
            json.put("source_lines", source_lines);
            json.put("path_to_name_lines", path_to_name_lines);
            json.put("var_lines", var_lines);
            json.put("len_lines", len_lines);
            json.put("name_to_path_lines", name_to_path_lines);
            
            amo::string sb = SINGLE_HEADER_TEMPLATE_HPP.format(json);
            
            
            amo::path outPath(amo::string(m_settings->output_path, m_settings->is_utf8));
            outPath.rename_extension(".hpp");
            outPath.parent().create_directory();
            std::ofstream ofs(outPath.c_str(), std::ios::out | std::ios::binary);
            ofs << sb.str();
            ofs.close();
            
            
        }
        
        // 生成源文件
        void make_source_file() {
            amo::json json;
            std::pair<std::string, std::string> name_space = parse_namespace();
            json.put("namespace_begin", name_space.first);
            json.put("namespace_end", name_space.second);
            
            
            
            std::string source_lines;
            std::string path_to_name_lines;
            std::string var_lines;
            std::string len_lines;
            std::string name_to_path_lines;
            
            for (auto& p : m_items) {
                amo::json json = p->toJson();
                source_lines += SOURCE_DECLARATION.format(json);
                path_to_name_lines += PATH_TO_NAME_DECLARATION.format(json);
                var_lines += NAME_TO_VAR_DECLARATION.format(json);
                len_lines += NAME_TO_LEN_DECLARATION.format(json);
                name_to_path_lines += NAME_TO_PATH_DECLARATION.format(json);
            }
            
            amo::string_utils::trim_right(path_to_name_lines, ", \n");
            amo::string_utils::trim_right(var_lines, ", \n");
            amo::string_utils::trim_right(len_lines, ", \n");
            amo::string_utils::trim_right(name_to_path_lines, ", \n");
            
            /* path_to_name_lines += "{\"\", \"\"}";
             var_lines += "{\"\", NULL}";
             len_lines += "{\"\", 0}";
             name_to_path_lines += "{\"\", \"\"}";*/
            
            json.put("source_lines", source_lines);
            json.put("path_to_name_lines", path_to_name_lines);
            json.put("var_lines", var_lines);
            json.put("len_lines", len_lines);
            json.put("name_to_path_lines", name_to_path_lines);
            
            amo::string sb = SOURCE_TEMPLATE_CPP.format(json);
            
            amo::path outPath(amo::string(m_settings->output_path, m_settings->is_utf8));
            outPath.rename_extension(".cpp");
            outPath.parent().create_directory();
            std::ofstream ofs(outPath.c_str(), std::ios::out | std::ios::binary);
            ofs << sb.str();
            ofs.close();
            
            
        }
        
    protected:
        std::pair<std::string, std::string> parse_namespace() {
            if (m_settings->name_space.empty()) {
                return{};
            }
            
            std::string split_str = ".";
            
            if (m_settings->name_space.find("::") != -1) {
                split_str = "::";
            }
            
            auto vec =  amo::string_utils::split(m_settings->name_space, split_str);
            std::string namespace_begin;
            std::string namespace_end;
            
            for (size_t i = 0; i < vec.size(); ++i) {
                namespace_begin += "namespace " + vec[i] + "{ ";
                
                namespace_end += "}    ";
            }
            
            return{ namespace_begin, namespace_end };
        }
        
    private:
        std::shared_ptr<settings> m_settings;
        std::vector<std::shared_ptr<file_item> > m_items;
    };
}


#endif // AMO_FILETOC_8E765103_9F10_4F6D_AEF1_1F037C593FEA_HPP__
