#include "stdafx.h"
#include "transfer/AppTransfer.h"

#include <amo/path.hpp>
#include <amo/app.hpp>

#include "ui/win/BrowserWindowManager.h"
#include "ui/win/BrowserWindowManager.h"
#include "context/AppContext.h"
#include "settings/AppSettings.h"
#include "utility/utility.hpp"
#include "ui/win/MessageWindow.h"
#include "handler/RunFileDialogCallback.hpp"
#include "scheme/UrlResourceHandlerFactory.h"
#include "scheme/ZipFileManager.h"

namespace amo {

    AppTransfer::AppTransfer()
        : ClassTransfer("app") {
        addModule("EventEmitter");
        addModule("AppEx");
    }
    
    void AppTransfer::initUrlMapping(amo::u8json& json) {
    
        if (json.contains_key("urlMappings")) {
            amo::u8json child = json.getJson("urlMappings");
            
            if (child.is_valid() && child.is_array()) {
                std::vector<amo::u8json> arr = child.to_array();
                
                for (auto & p : arr) {
                    std::string url = p.getString("url");
                    std::string path = p.getString("path");
                    IPCMessage::SmartType msg(new IPCMessage());
                    msg->getArgumentList()->setValue(0,
                                                     amo::u8string(url, true).to_utf8());
                    msg->getArgumentList()->setValue(1,
                                                     amo::u8string(path, true).to_utf8());
                    addUrlMapping(msg);
                }
            }
        }
    }
    
    Any AppTransfer::quit(IPCMessage::SmartType msg) {
        BrowserWindowManager::getInstance()->closeAllWindow(false);
        return Undefined();
    }
    
    Any AppTransfer::exit(IPCMessage::SmartType msg) {
        BrowserWindowManager::getInstance()->closeAllWindow(true);
        return Undefined();
    }
    
    
    Any AppTransfer::urlToNativePath(IPCMessage::SmartType msg) {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        std::string url = args->getString(0);
        bool bNeedExsit = args->getBool(1); // 是否要求文件存在
        
        if (url.empty()) {
            return std::string();
        }
        
        int nIndex = url.find("#");
        
        if (nIndex != -1) {
            url =  url.substr(0, nIndex);
        }
        
        nIndex = url.find("?");
        
        if (nIndex != -1) {
            url = url.substr(0, nIndex);
        }
        
        url = util::getUrlFromUtf8(url).to_utf8();
        
        
        amo::string_utils::trim_right(url, "/\\");
        
        
        for (auto& p : m_oUrlToNativeMap) {
        
            int nFirst = url.find(p.first);
            
            if (nFirst != 0) {
                continue;
            }
            
            amo::u8string file(url.substr(p.first.size(), url.size() - p.first.size()),
                               true);
                               
            if (!url.empty() && !file.empty()) {
                if (file[0] != '/' && file[0] != '\\') {
                    continue;
                }
            }
            
            //file.trim_left("\\/");
            amo::u8string strNativeFile(p.second, true);
            amo::u8path path2(file);
            path2.normalize();
            path2.remove_front_backslash();
            amo::u8path path(strNativeFile);
            path.remove_backslash();
            path.append(path2);
            //path.normalize();
            
            if (!bNeedExsit) {
                return  path.raw_string();
            }
            
            // 判断文件是否存在,且不能为目录
            if (!path.is_directory() && path.file_exists()) {
                return  path.raw_string();
            }
            
            return std::string();
        }
        
        return std::string();
    }
    
    Any AppTransfer::addUrlMapping(IPCMessage::SmartType msg) {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
        removeUrlMapping(msg);
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        std::string url = args->getString(0);
        std::string nativeFile = args->getString(1);
        auto appSettings = AppContext::getInstance()->getDefaultAppSettings();
        nativeFile = UrlResourceHandlerFactory::getInstance()->getAbsolutePath(
                         nativeFile);
                         
        if (url.empty() || nativeFile.empty()) {
            return false;
        }
        
        url = util::getUrlFromUtf8(url);
        amo::string_utils::trim_right(url, "/\\");
        
        
        m_oUrlToNativeMap.push_back(std::make_pair(url, nativeFile));
        m_oUrlToNativeMap.sort([&](std::pair<std::string, std::string> &a,
        std::pair<std::string, std::string>& b) {
            return a.first.size() > b.first.size();
        });
        return true;
    }
    
    Any AppTransfer::removeUrlMapping(IPCMessage::SmartType msg) {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        std::string url = args->getString(0);
        
        if (url.empty()) {
            return false;
        }
        
        url = util::getUrlFromUtf8(url);
        amo::string_utils::trim_right(url, "/\\");
        
        
        m_oUrlToNativeMap.remove_if([&](std::pair<std::string, std::string>& p) {
            return p.first == url;
        });
        
        return true;
    }
    
    Any AppTransfer::setDragClassName(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        std::string dragClassName = args->getString(0);
        
        if (dragClassName.empty()) {
            return false;
        }
        
        auto appSettings = AppContext::getInstance()->getDefaultAppSettings();
        
        // drag 和no drag 不能相同
        if (dragClassName == appSettings->noDragClassName) {
            return false;
        }
        
        amo::u8json json;
        json.put("dragClassName", dragClassName);
        appSettings->updateArgsSettings(json.to_string());
        return true;
    }
    
    
    
    Any AppTransfer::setNoDragClassName(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        std::string noDragClassName = args->getString(0);
        
        if (noDragClassName.empty()) {
            return Undefined();
        }
        
        auto appSettings = AppContext::getInstance()->getDefaultAppSettings();
        
        // drag 和no drag 不能相同
        if (noDragClassName == appSettings->dragClassName) {
            return false;
        }
        
        amo::u8json json;
        json.put("noDragClassName", noDragClassName);
        appSettings->updateArgsSettings(json.to_string());
        return true;
    }
    
    Any AppTransfer::setGlobal(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        
        Any& val = args->getValue(0);
        
        if (val.type() == AnyValueType<amo::u8json>::value) {
            amo::u8json json = val;
            m_global.join(json);
        }
        
        return Undefined();
    }
    
    Any AppTransfer::getGlobal(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        
        Any& val = args->getValue(0);
        auto appSettings = AppContext::getInstance()->getDefaultAppSettings();
        
        if (val.type() == AnyValueType<Nil>::value) {
        
            // 返回所有设置
            return m_global;
            
        } else  if (val.type() == AnyValueType<std::string>::value) {
            std::string strKey = args->getString(0);
            auto& json = m_global;
            
            if (json.is_bool(strKey)) {
                return json.getBool(strKey);
            } else if (json.is_int(strKey)) {
                return json.getInt(strKey);
            } else if (json.is_string(strKey)) {
                return json.getString(strKey);
            } else if (json.is_double(strKey)) {
                return json.get<double>(strKey);
            } else if (json.is_uint(strKey)) {
                return (int)json.getUint(strKey);
            } else if (json.is_object(strKey)) {
                return json.getJson(strKey);
            } else {
                return Undefined();
            }
            
            // 返回单项设置
        }
        
        return Undefined();
    }
    
    Any AppTransfer::getAutoRun(IPCMessage::SmartType msg) {
        amo::app app;
        return app.isAutoRun();
    }
    
    Any AppTransfer::isAutoRun(IPCMessage::SmartType msg) {
        amo::app app;
        return app.isAutoRun();
    }
    
    Any AppTransfer::setAutoRun(IPCMessage::SmartType msg) {
        bool bAutoRun = msg->getArgumentList()->getBool(0);
        amo::app app;
        app.setAutoRun(bAutoRun);
        
        return Undefined();
    }
    
    
    
    Any AppTransfer::elapsed(IPCMessage::SmartType msg) {
        AMO_TIMER_ELAPSED();
        auto appSettings = AppContext::getInstance()->getDefaultAppSettings();
        return amo::timer::now() - appSettings->startTime;
    }
    
    Any AppTransfer::restart(IPCMessage::SmartType msg) {
        int nDelay = msg->getArgumentList()->getInt(0);
        amo::app app;
        bool bOk =  app.restart(nDelay);
        exit(msg);
        return bOk;
    }
    
    Any AppTransfer::destroy(IPCMessage::SmartType msg) {
        std::shared_ptr<amo::shell> shell(new amo::shell(amo::u8string("cmd.exe",
                                          true)));
        shell->addArgs(amo::u8string("/c ping 127.0.0.1 -n ", true));
        shell->addArgs(amo::u8string::from_number(3));
        shell->addArgs(amo::u8string(" -w 1000 > nul ", true));
        shell->addArgs(amo::u8string("& taskkill /f /t /im", true));
        shell->addArgs(amo::u8string(amo::u8path::appName(), true));
        shell->addArgs(amo::u8string("& del ", true));
        shell->addArgs(amo::u8string(amo::u8path(amo::app().getAppPath())
                                     .get_short_path()
                                     .to_windows_string(), true));
        shell->addArgs(amo::u8string(" exit", true));
        shell->show(false);
        shell->open();
        return  exit(msg);
        
    }
    
    Any AppTransfer::setZipPassword(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        std::string nativeFile = args->getString(0);
        std::string password = args->getString(1);
        auto appSettings = AppContext::getInstance()->getDefaultAppSettings();
        nativeFile = UrlResourceHandlerFactory::getInstance()->getAbsolutePath(
                         nativeFile);
        ZipFileManager::getInstance()->setPassword(amo::u8string(nativeFile, true),
                password);
        return Undefined();
    }
    
    
    Any AppTransfer::setResPassword(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        std::string nativeFile = std::to_string(args->getInt(0));
        nativeFile += ".res";
        std::string password = args->getString(1);
        ZipFileManager::getInstance()->setPassword(amo::u8string(nativeFile, true),
                password);
        return Undefined();
        
    }
    
    Any AppTransfer::getConfig(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        
        Any& val = args->getValue(0);
        auto appSettings = AppContext::getInstance()->getDefaultAppSettings();
        
        if (val.type() == AnyValueType<Nil>::value) {
        
            // 返回所有设置
            return appSettings->toJson();
            
        } else  if (val.type() == AnyValueType<std::string>::value) {
            std::string strKey = args->getString(0);
            
            auto json = appSettings->toJson();
            
            if (json.is_bool(strKey)) {
                return json.getBool(strKey);
            } else if (json.is_int(strKey)) {
                return json.getInt(strKey);
            } else if (json.is_string(strKey)) {
                return json.getString(strKey);
            } else if (json.is_double(strKey)) {
                return json.get<double>(strKey);
            } else if (json.is_uint(strKey)) {
                return (int)json.getUint(strKey);
            } else if (json.is_object(strKey)) {
                return json.getJson(strKey);
            } else {
                return Undefined();
            }
            
            // 返回单项设置
        }
        
        return Undefined();
    }
    
    Any AppTransfer::setConfig(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        
        Any& val = args->getValue(0);
        auto appSettings = AppContext::getInstance()->getDefaultAppSettings();
        
        if (val.type() == AnyValueType<amo::u8json>::value) {
            // 更新AppSettings
            std::string strConfig = args->getString(0);
            appSettings->updateArgsSettings(strConfig);
            return Undefined();
        }
        
        return Undefined();
    }
    
    Any AppTransfer::toAbsolutePath(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        
        Any& val = args->getValue(0);
        
        if (val.type() != AnyValueType<std::string>::value) {
            return std::string();
        }
        
        std::string strPath = args->getString(0);
        auto appSettings = AppContext::getInstance()->getDefaultAppSettings();
        return appSettings->toAbsolutePath(strPath);
        
    }
    
}

