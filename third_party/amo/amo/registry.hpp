// Created by amoylel on 10/31/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_REGISTRY_HPP__
#define AMO_REGISTRY_HPP__

#include <string>
#include <windows.h>
#include <sstream>

#include <amo/utility.hpp>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
namespace amo {
    class registry_key {
    
    };
    
    class registry {
    public:
        const static registry_key ClassesRoot;
    public:
        registry(HKEY root)
            : m_hRoot(root) {
            m_hKey = NULL;
            m_flag = 0;
        }
    public:
    
        bool create(const std::string& pszSubKey) {
            LONG lResult = ::RegCreateKeyA(m_hRoot, pszSubKey.c_str(), &m_hKey);
            
            if (lResult != ERROR_SUCCESS) {
                return false;
            }
            
            return true;
        }
        
        // 打开注册表
        bool open(const std::string& keyName, REGSAM flag, bool auto_create = false) {
            if (is_open() && m_flag == flag) {
                return true;
            }
            
            close();
            
            LONG lResult = ::RegOpenKeyExA(m_hRoot, keyName.c_str(), 0, flag, &m_hKey);
            
            if (lResult != ERROR_SUCCESS) {
            
                m_hKey = NULL;
                m_flag = 0;
                
                if (auto_create) {
                    create(keyName);
                    return open(keyName, flag, false);
                }
                
                return false;
            }
            
            m_flag = flag;
            return true;
        }
        
        bool is_open() const {
            return m_hKey != NULL;
        }
        
        void close() {
            if (m_hKey == NULL) {
                return;
            }
            
            RegCloseKey(m_hKey); //关闭注册表
            m_hKey = NULL;
            m_flag = 0;
        }
        
        bool contains_key(const std::string& keyName) {
            bool bOk =  open(keyName, KEY_READ);
            
            if (bOk) {
                close();
                return true;
            }
            
            return false;
        }
        
        template<typename R>
        R read(const std::string& keyName, const std::string& valueName) {
            return read<R>(keyName, valueName, R());
        }
        
        template<typename R>
        R read(const std::string& keyName, const std::string& valueName,
               R defaultValue) {
            R retval = defaultValue;
            
            do {
                if (!open(keyName, KEY_READ)) {
                    break;
                }
                
                DWORD dwType = REG_SZ;
                DWORD dwSize = 1024;
                char data[1024] = { 0 };
                
                long lRet = RegQueryValueExA(m_hKey,
                                             valueName.c_str(),
                                             NULL,
                                             &dwType,
                                             (LPBYTE)data,
                                             &dwSize);
                                             
                if (lRet != ERROR_SUCCESS) {
                    break;
                }
                
                retval = amo::bytes_to_int<R>((const unsigned char*)data, is_little_endian());
                
                /* std::stringstream stream;
                 stream << data;
                 stream.flush();
                 stream >> retval;*/
                
            } while (false);
            
            close();
            return retval;
        }
        
        template<>
        std::string read< std::string>(const std::string& keyName,
                                       const std::string& valueName,
                                       std::string defaultValue) {
            std::string retval = defaultValue;
            
            do {
                if (!open(keyName, KEY_READ)) {
                    break;
                }
                
                DWORD dwType = REG_SZ;
                DWORD dwSize = 1024;
                char data[1024] = { 0 };
                
                long lRet = RegQueryValueExA(m_hKey,
                                             valueName.c_str(),
                                             NULL,
                                             &dwType,
                                             (LPBYTE)data,
                                             &dwSize);
                                             
                if (lRet != ERROR_SUCCESS) {
                    break;
                }
                
                std::stringstream stream;
                stream << data;
                retval = stream.str();
                
            } while (false);
            
            close();
            return retval;
        }
        
        template<typename T>
        bool write(const std::string & keyName, const std::string& valueName,
                   const T & value, bool auto_create = false) {
                   
            bool bSuccess = false;
            
            do {
                if (!open(keyName, KEY_WRITE, auto_create)) {
                    break;
                }
                
                std::stringstream stream;
                stream << value;
                std::string key_val = stream.str();
                long lRet = RegSetValueExA(m_hKey,
                                           valueName.c_str(),
                                           0,
                                           REG_SZ,
                                           (BYTE *)key_val.c_str(),
                                           key_val.size());		//添加或者修改注册表项
                                           
                                           
                                           
                if (lRet != ERROR_SUCCESS) {
                    break;
                }
                
                bSuccess = true;
                
                
            } while (false);
            
            close();
            return bSuccess;
        }
        
        
        bool remove(const std::string& keyName) {
        
            bool bSuccess = false;
            
            do {
                long lRet = SHDeleteKeyA(m_hRoot, keyName.c_str());
                
                if (lRet != ERROR_SUCCESS) {
                    break;
                }
                
                bSuccess = true;
            } while (false);
            
            return bSuccess;
            
        }
        
        bool remove(const std::string& keyName, const std::string& valueName) {
            bool bSuccess = false;
            
            do {
                if (!open(keyName, KEY_WRITE)) {
                    break;
                }
                
                
                long lRet = RegDeleteValueA(m_hKey,  valueName.c_str());		//删除注册表项
                
                if (lRet != ERROR_SUCCESS) {
                    break;
                }
                
                bSuccess = true;
                
                
            } while (false);
            
            close();
            return bSuccess;
        }
        
    private:
    
        HKEY m_hRoot;
        HKEY m_hKey;
        REGSAM m_flag;
        
    };
}
#endif // AMO_REGISTRY_HPP__