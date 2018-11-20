// Created by amoylel on 05/22/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_DLLEXTENSIONMANAGER_H__
#define AMO_DLLEXTENSIONMANAGER_H__


#include <string>
#include <unordered_map>
#include <memory>

#include <amo/string.hpp>
#include <amo/singleton.hpp>


#include <regex>
#include <vector>
#include "handler/CefHeader.hpp"


namespace amo {
    class loader;
    
    class FunctionArgs {
    public:
        std::string m_strFunctionName; // 函数名
        std::string m_strRetal;	//返回值
        std::vector<std::string> m_vecArgsList; // 参数列表
        
    };
    class DllFunctionWrapper {
    public:
        DllFunctionWrapper(const std::string& dllName): m_strDllName(dllName) {
        
        }
        
        const std::string& getDllName() const {
            return m_strDllName;
        }
        void setDllName(const std::string& val) {
            m_strDllName = val;
        }
        
        void addArgsList(const std::string& argsList) {
            std::regex retval("\\w+\\b");    // 返回值类型
            std::regex funcName("\\w+\\b");  // 函数名
            std::regex args("\\w+\\b"); // 参数列表
            std::smatch m;
            FunctionArgs func;
            std::string s = argsList;
            
            if (std::regex_search(s, m, retval)) {
                func.m_strRetal = m[0].str();
                s = m.suffix();
            }
            
            if (std::regex_search(s, m, funcName)) {
                func.m_strFunctionName = m[0].str();
                s = m.suffix();
            }
            
            while (std::regex_search(s, m, args)) {
                for (auto x = m.begin(); x != m.end(); x++) {
                    func.m_vecArgsList.push_back(x->str());
                }
                
                s = m.suffix().str();
            }
            
            m_oArgsListMap[func.m_strFunctionName] = func;
        }
        
        FunctionArgs& getArgsList(const std::string& functionName) {
            auto iter = m_oArgsListMap.find(functionName);
            
            if (iter == m_oArgsListMap.end()) {
                iter = m_oArgsListMap.insert(std::make_pair(functionName,
                                             FunctionArgs())).first;
            }
            
            return iter->second;
        }
    private:
        std::unordered_map<std::string, FunctionArgs > m_oArgsListMap; // 参数列表
        std::string m_strDllName;	//dll名称
    };
    
    class DllManagerBase  {
    public:
        DllManagerBase();
        ~DllManagerBase();
        
        std::vector<amo::u8string> exports(const amo::u8string& name);
        /*!
         * @fn	std::shared_ptr<amo::loader> DllExtensionManager::load(const amo::u8string& name);
         *
         * @brief	Loads the given name.
         * 			加载格式1 libraryName
         * 			加载格式2 libraryName.dll
         * 			加载格式3 C:/libraryName[.dll]*
         * 			对动态库的查找通过相对路径和绝对路径两种
         *
         * @param	name	The name to load.
         *
         * @return	An std::shared_ptr&lt;amo::loader&gt;
         */
        std::shared_ptr<amo::loader> load(const amo::u8string& name);
        
        void unload(const amo::u8string& name);
        
        // 获取所给路径的绝对路径
        amo::u8string getFullPath(const amo::u8string& name);
        
        std::shared_ptr<amo::loader> get(const amo::u8string& name);
        
        // 添加后缀名
        amo::u8string addSuffix(const amo::u8string& name);
        
        void addArgsList(const std::string& dllName, const std::string& argsList);
        
        DllFunctionWrapper& getDllFunctionWrapper(const std::string& dllName);
        amo::u8string getExtensionDir() const;
        void setExtensionDir(amo::u8string val);
        bool getRunOnRenderThread() const;
        void setRunOnRenderThread(bool val);
        void clear();
        
        
    private:
        std::unordered_map<amo::u8string, std::shared_ptr<amo::loader> > m_oMap;
        amo::u8string m_strExtensionDir;
        std::unordered_map<amo::u8string, DllFunctionWrapper > m_oDllArgsMap;
        bool m_bRunOnRenderThread;
    };
    
    template<cef_process_id_t>
    class DllManager : public DllManagerBase {
    
    };
    
    template<> class DllManager<PID_RENDERER>
        : public DllManagerBase
        , public singleton<DllManager<PID_RENDERER> > {
    public:
        DllManager() {
            setExtensionDir(amo::u8string("renderer_modules", true));
            setRunOnRenderThread(true);
        }
    };
    
    template<> class DllManager<PID_BROWSER>
        : public DllManagerBase
        , public singleton<DllManager<PID_BROWSER> > {
    public:
        DllManager() {
            setExtensionDir(amo::u8string("browser_modules", true));
            setRunOnRenderThread(false);
            
        }
    };
}
#endif // AMO_DLLEXTENSIONMANAGER_H__