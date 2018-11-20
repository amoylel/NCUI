// Created by 苏元海 on 08/17/2016.
// Copyright (c) 2016 amoylel. All rights reserved.

#ifndef IME_PROGRAM_OPTIONS_HPP__
#define IME_PROGRAM_OPTIONS_HPP__

#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include <sstream>

#include <amo/config.hpp>
#include <amo/unordered_map.hpp>
#include <amo/functional.hpp>

#include <amo/json.hpp>

#include <amo/string/string_utils.hpp>

namespace amo {
    namespace po = boost::program_options;
    
    inline std::pair<std::string, std::string> at_option_parser(
        std::string const&s) {
        if ('@' == s[0]) {
            return std::make_pair(std::string("config-file"), s.substr(1));
        } else  {
            /*  do {
            	  if (amo::string_utils::begin_with(s, "--")) {
            		  std::vector<std::string > vec = amo::string_utils::split(s, "=");
            
            		  if (vec.size() != 2) {
            			  break;
            		  }
            
            		  return{ vec[0], vec[1] };
            	  }
              } while (false);*/
            
            
            return std::pair<std::string, std::string>();
        }
    }
    
    class program_options {
    public:
        program_options(const std::string name = "options") : m_desc(name) {
            m_allow_unregistered = false;
        }
        
        ~program_options() { }
        
        po::options_description_easy_init add_options() {
            
            
            return m_desc.add_options();
        }
        
        program_options& add(const program_options& rhs) {
            m_desc.add(rhs.desc());
            return *this;
        }
        
        program_options& add(const  po::options_description& rhs) {
            m_desc.add(rhs);
            return *this;
        }
        
        po::options_description& desc() {
            return m_desc;
        }
        
        const po::options_description& desc() const {
            return m_desc;
        }
        
        po::positional_options_description& p() {
            return m_p;
        }
        
        size_t count(const std::string& key) {
            return vm.count(key);
        }
        
        po::variables_map& variables_map() {
            return vm;
        }
        
        void allow_unregistered(bool allow) {
            m_allow_unregistered = allow;
        }
        
        
        bool allow_unregistered() const {
            return m_allow_unregistered;
        }
        const po::variable_value& operator[](const std::string& name) {
            return vm[name];
        }
        
        virtual bool parse_string_line(const std::string& str) {
            try {
                vm.clear();
                boost::char_separator<char> sep(" \n\r");
                boost::tokenizer<boost::char_separator<char> > tok(str, sep);
                std::vector<std::string> args;
                
                std::copy(tok.begin(), tok.end(), back_inserter(args));
                
                //po::store(po::command_line_parser(args).options(m_desc).run(), vm);
                
                
                if (allow_unregistered()) {
                    po::store(po::command_line_parser(args)
                              .options(m_desc)
                              .positional(m_p)
                              .allow_unregistered()
                              .extra_parser(at_option_parser)
                              .run(), vm);
                } else {
                    po::store(po::command_line_parser(args)
                              .options(m_desc)
                              .positional(m_p)
                              .extra_parser(at_option_parser)
                              .run(), vm);
                }
                
                return on_parse_completed(vm);
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                return false;
            }
        }
        
        virtual bool parse_command_line(int argc, char** argv) {
        
            try {
                /*po::store(po::command_line_parser(argc, argv).options(desc)
                	.extra_parser(at_option_parser).run(), vm);*/
                
                /* po::store(po::command_line_parser(argc,
                                                   argv).options(desc).extra_parser(at_option_parser).run(), vm);*/
                
                if (allow_unregistered()) {
                    po::store(po::command_line_parser(argc, argv)
                              .options(m_desc)
                              .positional(m_p)
                              .allow_unregistered()
                              .extra_parser(at_option_parser)
                              .run(), vm);
                } else {
                    po::store(po::command_line_parser(argc, argv)
                              .options(m_desc)
                              .positional(m_p)
                              .extra_parser(at_option_parser)
                              .run(), vm);
                }
                
                po::notify(vm);
                
                if (vm.count("config-file")) {
                    std::string config_file = vm["config-file"].as<std::string>();
                    
                    std::ifstream ifs(config_file.c_str());
                    
                    if (!ifs) {
                        std::cout << "could not open the config file:" << config_file << std::endl;
                        return false;
                    }
                    
                    std::stringstream ss;
                    ss << ifs.rdbuf();
                    
                    boost::char_separator<char> sep(" \n\r");
                    std::string sstr = ss.str();
                    
                    boost::tokenizer<boost::char_separator<char> > tok(sstr, sep);
                    std::vector<std::string> args;
                    
                    std::copy(tok.begin(), tok.end(), back_inserter(args));
                    
                    po::store(po::command_line_parser(args).options(m_desc).run(), vm);
                    po::notify(vm);
                }
                
                return on_parse_completed(vm);
            } catch (std::exception& e) {
                std::cout << "parse failed: " << e.what() << std::endl;
                return false;
            }
        }
        
        
        virtual bool on_parse_completed(po::variables_map& vm) {
            return true;
        }
        
        virtual amo::json to_json() const {
            return ToJson(vm);
        }
        
        static amo::json ToJson(const amo::po::variables_map& vm) {
            amo::json json;
            
            for (auto iter = vm.begin(); iter != vm.end(); ++iter) {
                const  auto& type = iter->second.value().type();
                const std::string& name = iter->first;
                
                if (get_type_info<bool>() == type) {
                    json.put(name, iter->second.as<bool>());
                } else if (get_type_info<int>()  == type) {
                    json.put(name, iter->second.as<int>());
                } else if (get_type_info<double>() == type) {
                    json.put(name, iter->second.as<double>());
                } else if (get_type_info<std::string>()  == type) {
                    json.put(name, iter->second.as<std::string>());
                } else if (get_type_info<std::vector<int> >()  == type) {
                    std::vector<int> vec = iter->second.as<std::vector<int> >();
                    amo::json arr(false);
                    
                    for (auto& p : vec) {
                        arr.push_back(p);
                    }
                    
                    json.put(name, arr);
                } else if (get_type_info<std::vector<std::string> >()  == type) {
                    std::vector<std::string> vec = iter->second.as<std::vector<std::string> >();
                    amo::json arr(false);
                    
                    for (auto& p : vec) {
                        arr.push_back(p);
                    }
                    
                    json.put(name, arr);
                }
                
            }
            
            return json;
        }
        
        template<typename T>
        static const boost::typeindex::type_info& get_type_info() {
            return boost::typeindex::type_id<T>().type_info();
            //return boost::typeindex::type_id<ValueType>().type_info();
        }
        friend std::ostream& operator<<(std::ostream& o, const program_options& parser);
    protected:
        po::options_description m_desc;
        po::positional_options_description m_p;
        po::variables_map vm;
        bool m_allow_unregistered;  // 是否允许未知选项
    };
    
    inline std::ostream& operator<<(std::ostream& o,
                                    const program_options& parser) {
        return o << parser.m_desc;
    }
}


#endif // IME_PROGRAM_OPTIONS_HPP__
