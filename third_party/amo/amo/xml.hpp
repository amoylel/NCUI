// Created by amoylel on 11/09/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_XML_HPP__
#define AMO_XML_HPP__


#include <string>
#include <sstream>
#include <fstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace amo {
    using namespace boost::property_tree;
    class xml {
    public:
        static xml from_file(const std::string& file) {
        
            /*  ptree* tree = new ptree;
              ptree* root = new ptree;
              xml_parser::read_xml(file, *tree);
              *root = tree->get_child("file");*/
            
            
            ptree pt;
            boost::property_tree::xml_parser::read_xml(file, pt);
            
            return xml(pt);
        }
        
    public:
        xml() {
        
        }
        
        xml(const std::string& str) {
        }
        
        xml(const ptree& pt_)
            : pt(pt_) {
            
        }
        
        std::string getString(const std::string& key, const std::string& default_val = "") {
            if (pt.count(key) < 1) {
                return default_val;
            }
            
            return pt.get<std::string>(key);
        }
        
        template<typename T>
        T value() {
            try {
            
                return boost::lexical_cast<T>(pt.data());
            } catch (const std::exception& e) {
                std::string error = e.what();
                return T();
            }
            
        }
        
        std::string value() {
            return pt.data();
        }
        
        // 获取所有
        template<typename T>
        std::vector<T> get(const std::string& key) {
            std::vector<T> vec;
            
            try {
                for (ptree::iterator it = pt.begin(); it != pt.end(); it++) {
                    ptree& node = it->second;
                    std::string name = it->first;
                    
                    if (name == key) {
                        vec.push_back(boost::lexical_cast<T>(node.data()));
                    }
                }
                
                return vec;
            } catch (const std::exception& e) {
                std::string error = e.what();
            }
            
            
            return vec;
        }
        
        // 获取所有
        template<>
        std::vector<xml> get<xml>(const std::string& key) {
            std::vector<xml> vec;
            
            for (ptree::iterator it = pt.begin(); it != pt.end(); it++) {
                ptree& node = it->second;
                std::string name = it->first;
                
                if (name == key) {
                    vec.push_back(xml(node));
                }
            }
            
            return vec;
        }
        
        //获取一项
        template<typename T>
        T one(const std::string& key) {
            for (ptree::iterator it = pt.begin(); it != pt.end(); it++) {
                ptree& node = it->second;
                std::string name = it->first;
                
                if (name == key) {
                    return node.get<T>(key);
                }
            }
            
            return T();
        }
        
        // 获取一项
        template<>
        xml one<xml>(const std::string& key) {
            for (ptree::iterator it = pt.begin(); it != pt.end(); it++) {
                ptree& node = it->second;
                std::string name = it->first;
                
                if (name == key) {
                    return (xml(node));
                }
            }
            
            return xml();
        }
        
        // 获取所有包含某属性的节点
        std::vector<xml> attrs(const std::string& key) {
            std::vector<xml> vec;
            
            for (ptree::iterator it = pt.begin(); it != pt.end(); it++) {
                ptree& node = it->second;
                std::string not_found = "9e965168-2d60-4db7-b549-85c8bd4de277";
                std::string attrValue = node.get<std::string>(std::string("<xmlattr>.") + key, not_found);
                
                if (attrValue != not_found) {
                    vec.push_back(xml(node));
                }
            }
            
            return vec;
        }
        
        // 获取所有包含某属性的节点,且值与所给值相等
        std::vector<xml> attrs(const std::string& key, const std::string& value) {
            std::vector<xml> vec;
            
            
            for (ptree::iterator it = pt.begin(); it != pt.end(); it++) {
                ptree& node = it->second;
                
                std::string not_found = "9e965168-2d60-4db7-b549-85c8bd4de277";
                std::string attrValue = node.get<std::string>(std::string("<xmlattr>.") + key, not_found);
                
                if (attrValue == value) {
                    vec.push_back(xml(node));
                }
            }
            
            return vec;
        }
        
        // 获取所有包含某属性的节点
        xml attr(const std::string& key) {
            for (ptree::iterator it = pt.begin(); it != pt.end(); it++) {
                ptree& node = it->second;
                std::string not_found = "9e965168-2d60-4db7-b549-85c8bd4de277";
                std::string attrValue = node.get<std::string>(std::string("<xmlattr>.") + key, not_found);
                
                if (attrValue != not_found) {
                    return xml(node);
                }
            }
            
            return xml();
        }
        
        // 获取所有包含某属性的节点,且值与所给值相等
        xml attr(const std::string& key, const std::string& value) {
            for (ptree::iterator it = pt.begin(); it != pt.end(); it++) {
                ptree& node = it->second;
                
                std::string not_found = "9e965168-2d60-4db7-b549-85c8bd4de277";
                std::string attrValue = node.get<std::string>(std::string("<xmlattr>.") + key, not_found);
                
                if (attrValue == value) {
                    return xml(node);
                }
            }
            
            return xml();
        }
        
        std::string getStringByAttr(const std::string& key,
                                    const std::string& attr,
                                    const std::string& default_val = "") {
            for (ptree::iterator it = pt.begin(); it != pt.end(); it++) {
                ptree& node = it->second;
                std::string name = it->first;
                std::string value = node.data();
                
                std::string attrValue = node.get<std::string>(std::string("<xmlattr>.") + key);
                
                if (attrValue == attr) {
                    return value;
                }
            }
            
            return default_val;
            
        }
        
        xml getChild(const std::string& key) {
            return xml(pt.get_child(key));
        }
        
    private:
        ptree pt;
        
    };
}

#endif // AMO_XML_HPP__