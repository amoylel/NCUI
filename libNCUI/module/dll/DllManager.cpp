#include "stdafx.h"
#include "module/dll/DllManager.h"
#include <amo/path.hpp>
#include <amo/loader.hpp>

#pragma comment(lib, "Dbghelp.lib")

namespace amo {

    DllManagerBase::DllManagerBase() {
        setRunOnRenderThread(false);
        setExtensionDir(amo::u8string("renderer_modules", true));
    }
    
    DllManagerBase::~DllManagerBase() {
        clear();
    }
    
    std::vector<amo::u8string> DllManagerBase::exports(const amo::u8string& name) {
        std::shared_ptr<amo::loader> ptr = load(name);
        std::vector<amo::u8string> vec;
        
        if (!ptr) {
            return vec;
        }
        
        ptr->exports(vec);
        return vec;
    }
    
    std::shared_ptr<amo::loader> DllManagerBase::load(const amo::u8string& name) {
        amo::u8string path = getFullPath(name);
        
        if (path.empty()) {
            return  std::shared_ptr<amo::loader>();
        }
        
        auto iter = m_oMap.find(path);
        std::shared_ptr<amo::loader> ptr;
        
        if (iter != m_oMap.end()) {
            ptr = iter->second;
        } else {
            ptr.reset(new amo::loader(path));
            
            bool bOk = ptr->load();
            
            if (!bOk) {
                ptr.reset();
            } else {
                m_oMap[path] = ptr;
            }
        }
        
        return ptr;
    }
    
    void DllManagerBase::unload(const amo::u8string& name) {
    
    }
    
    amo::u8string DllManagerBase::getFullPath(const amo::u8string& str) {
        // 添加后缀名
        amo::u8string name = addSuffix(str);
        amo::u8path p(name);
        
        // 判断当前路径是否为绝对路径，如果是那么直接返回
        if (!p.is_relative()) {
            return name;
        }
        
        // 如果不是，那么以当前程序所在目录为当前目录
        amo::u8string exeDir(amo::u8path::getExeDir(), true);
        amo::u8path pa(exeDir);
        
        // 1. 假设所dll在当前目录
        pa.append(name.str());
        $cdevel(pa.generic_ansi_string());
        
        // 如果这个dll存在，那么返回
        if (pa.file_exists()) {
            return amo::u8string(pa.c_str(), true);
        }
        
        pa = amo::u8path(exeDir);
        // 2. 在扩展目录下查找
        pa.append(getExtensionDir().str()).append(name.str());
        $cdevel(pa.generic_ansi_string());
        
        if (pa.file_exists()) {
            return amo::u8string(pa.c_str(), true);
        }
        
        // 3. 到系统目录下查找
        pa = amo::u8path("C:\\windows\\system32\\");
        pa.append(name.str());
        $cdevel(pa.generic_ansi_string());
        
        if (pa.file_exists()) {
            return amo::u8string(pa.c_str(), true);
        }
        
        return amo::u8string("", true);
    }
    
    std::shared_ptr<amo::loader> DllManagerBase::get(const amo::u8string& name) {
        amo::u8string str = getFullPath(name);
        auto iter = m_oMap.find(str);
        return iter->second;
    }
    
    amo::u8string DllManagerBase::addSuffix(const amo::u8string& name) {
        amo::u8string str = name;
        
        if (!str.end_with(amo::u8string(".dll", true))) {
            str += ".dll";
        }
        
        return str;
    }
    
    
    void DllManagerBase::addArgsList(const std::string& dllName,
                                     const std::string& argsList) {
        DllFunctionWrapper& funcWrapper = getDllFunctionWrapper(dllName);
        funcWrapper.addArgsList(argsList);
    }
    
    DllFunctionWrapper& DllManagerBase::getDllFunctionWrapper(
        const std::string& dllName) {
        auto iter = m_oDllArgsMap.find(amo::u8string(dllName, true));
        
        if (iter == m_oDllArgsMap.end()) {
            iter = m_oDllArgsMap.insert(std::make_pair(amo::u8string(dllName, true),
                                        DllFunctionWrapper(dllName))).first;
        }
        
        return iter->second;
    }
    
    amo::u8string DllManagerBase::getExtensionDir() const {
        return m_strExtensionDir;
    }
    
    void DllManagerBase::setExtensionDir(amo::u8string val) {
        m_strExtensionDir = val;
    }
    
    bool DllManagerBase::getRunOnRenderThread() const {
        return m_bRunOnRenderThread;
    }
    
    void DllManagerBase::setRunOnRenderThread(bool val) {
        m_bRunOnRenderThread = val;
        
        if (m_bRunOnRenderThread) {
            setExtensionDir(amo::u8string("renderer_modules", true));
        } else {
            setExtensionDir(amo::u8string("browser_modules", true));
        }
    }
    
    void DllManagerBase::clear() {
        for (auto p : m_oMap) {
            p.second->exec<amo::nil>("unregisterTransfer");
        }
        
        m_oMap.clear();
        m_oDllArgsMap.clear();
    }
    
    
    
}

