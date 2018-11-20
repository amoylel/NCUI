#include "stdafx.h"
#include "RceditTransfer.h"
#include <amo/loader.hpp>
#include <amo/path.hpp>
#include <string.h>
#include "ResourceUpdater.h"
#include "../NCUI/resource.h"
#include "BinResourceUpdater.h"

namespace {


    class StringLoader {
    public:
        StringLoader(HINSTANCE hInstance) {
            m_hInstance = hInstance;
        }
        std::wstring load(UINT id) {
            wchar_t str[4096] = { 0 };
            ::LoadStringW(m_hInstance, id, str, 4096);
            return str;
        }
    private:
        HINSTANCE m_hInstance;
    };
    
    bool print_error(const char* message) {
        fprintf(stderr, "Fatal error: %s\n", message);
        return 1;
    }
    bool parse_version_string(const wchar_t* str, unsigned short *v1, unsigned short *v2, unsigned short *v3, unsigned short *v4) {
        *v1 = *v2 = *v3 = *v4 = 0;
        return (swscanf_s(str, L"%hu.%hu.%hu.%hu", v1, v2, v3, v4) == 4) ||
               (swscanf_s(str, L"%hu.%hu.%hu", v1, v2, v3) == 3) ||
               (swscanf_s(str, L"%hu.%hu", v1, v2) == 2) ||
               (swscanf_s(str, L"%hu", v1) == 1);
    }
    
    
    int wmain3() {
    
        int argc = 3;
        const wchar_t* t1 = L"NCUI.exe";
#if _DEBUG
        const wchar_t* t2 = L"NCUIDemo.exe";
#else
        const wchar_t* t2 = L"NCUIDemo.exe";
#endif
        
        const wchar_t* t3 = L"--set-resource-string";
        const wchar_t* argv[3];
        argv[0] = t1;
        argv[1] = t2;
        argv[2] = t3;
        bool loaded = false;
        amo::loader loader;
        amo::u8path p(amo::u8path::getExeDir());
        amo::u8path dist("cef.exe");
        dist = p.append_c(dist);
        
        amo::u8path src(amo::u8path::getFullExeName());
        src.copy_to(dist);
        
        p.append("rcedit.dll");
        bool bOk = loader.load(p.generic_wstring());
        
        amo::shared_ptr<amo::ResourceUpdater> updater(new amo::ResourceUpdater());
        
        
        
        for (int i = 1; i < argc; ++i) {
            if (wcscmp(argv[i], L"--set-version-string") == 0 ||
                    wcscmp(argv[i], L"-svs") == 0) {
                if (argc - i < 3) {
                    return print_error("--set-version-string requires 'Key' and 'Value'");
                }
                
                const wchar_t* key = argv[++i];
                const wchar_t* value = argv[++i];
                
                if (!updater->SetVersionString(key, value)) {
                    return print_error("Unable to change version string");
                }
                
            } else if (wcscmp(argv[i], L"--set-file-version") == 0 ||
                       wcscmp(argv[i], L"-sfv") == 0) {
                if (argc - i < 2) {
                    return print_error("--set-file-version requires a version string");
                }
                
                unsigned short v1, v2, v3, v4;
                
                if (!parse_version_string(argv[++i], &v1, &v2, &v3, &v4)) {
                    return print_error("Unable to parse version string for FileVersion");
                }
                
                if (!updater->SetFileVersion(v1, v2, v3, v4)) {
                    return print_error("Unable to change file version");
                }
                
                if (!updater->SetVersionString(L"FileVersion", argv[i])) {
                    return print_error("Unable to change FileVersion string");
                }
                
            } else if (wcscmp(argv[i], L"--set-product-version") == 0 ||
                       wcscmp(argv[i], L"-spv") == 0) {
                if (argc - i < 2) {
                    return print_error("--set-product-version requires a version string");
                }
                
                unsigned short v1, v2, v3, v4;
                
                if (!parse_version_string(argv[++i], &v1, &v2, &v3, &v4)) {
                    return print_error("Unable to parse version string for ProductVersion");
                }
                
                if (!updater->SetProductVersion(v1, v2, v3, v4)) {
                    return print_error("Unable to change product version");
                }
                
                if (!updater->SetVersionString(L"ProductVersion", argv[i])) {
                    return print_error("Unable to change ProductVersion string");
                }
                
            } else if (wcscmp(argv[i], L"--set-icon") == 0 ||
                       wcscmp(argv[i], L"-si") == 0) {
                if (argc - i < 2) {
                    return print_error("--set-icon requires path to the icon");
                }
                
                if (!updater->SetIcon(argv[++i])) {
                    return print_error("Unable to set icon");
                }
            } else if (wcscmp(argv[i], L"--set-resource-string") == 0 ||
                       wcscmp(argv[i], L"--srs") == 0) {
                /*if (argc - i < 3)
                return print_error("--set-resource-string requires int 'Key' and string 'Value'");
                
                const wchar_t* key = argv[++i];*/
                
                /*	const wchar_t* key = L"104";
                if (swscanf_s(key, L"%d", &key_id) != 1)
                return print_error("Unable to parse id");
                */
                unsigned int key_id = 104;
                
                
                //const wchar_t* value = argv[++i];
                const wchar_t* value = L"chrome://version";
                
                if (!updater->ChangeString(key_id, value)) {
                    return print_error("Unable to change string");
                }
            } else {
                if (loaded) {
                    return print_error("Unexpected trailing arguments");
                }
                
                loaded = true;
                
                if (!updater->Load(amo::u8string(dist.c_str(), true).to_unicode().c_str())) {
                    return print_error("Unable to load file");
                }
                
            }
        }
        
        //updater->ChangeString(IDS_SIZE, L"640,480");
        updater->SetVersionString(L"ProductName", L"产品名称名称名称");
        
        /* if (!updater->ChangeString(IDS_NORMAL, L"true")) {
        	 return print_error("Unable to change string");
         }*/
        
        if (!loaded) {
            return print_error("You should specify a exe/dll file");
        }
        
        if (!updater->Commit()) {
            DWORD err = GetLastError();
            return print_error("Unable to commit changes");
        }
        
        
        return 0;
    }
    
}





amo::Any amo::RceditTransfer::commit(IPCMessage::SmartType msg) {

    m_pUpdater.reset(new amo::ResourceUpdater());
    std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
    
    
    // 如果没有输入参数，那么直接使用默认参数生成文件
    amo::u8string strConfigFile(getDefaultFileSettings(msg).As<std::string>(), true);
    amo::u8string strConfigAppSettings(getDefaultAppSettings(msg).As<std::string>(),
                                       true);
    amo::u8string strConfigBrowserSettinggs(getDefaultBrowserSettings(
            msg).As<std::string>(), true);
    amo::u8string strConfigSplashSettinggs(getDefaultSplashSettings(
            msg).As<std::string>(), true);
            
            
    int argsSize = args->getArgsSize();
    
    if (argsSize == 4) {
        strConfigFile = amo::u8string(args->getString(0), true);
        strConfigAppSettings = amo::u8string(args->getString(1), true);
        strConfigBrowserSettinggs = amo::u8string(args->getString(2), true);
        strConfigSplashSettinggs = amo::u8string(args->getString(3), true);
    }
    
    if (argsSize > 0 && argsSize < 4) {
        return false;
    }
    
    
    
    amo::u8string strConfig = strConfigFile;
    amo::u8json oConfig(strConfig.str());
    
    if (!oConfig.is_valid()) {
        return false;
    }
    
    if (oConfig.find_member("OriginalFileName") == oConfig.end()) {
        return false;
    }
    
    std::set<std::string> oVersionSet;
    oVersionSet.insert("CompanyName");
    oVersionSet.insert("FileDescription");
    oVersionSet.insert("FileVersion");
    oVersionSet.insert("InternalName");
    oVersionSet.insert("LegalCopyright");
    oVersionSet.insert("OriginalFileName");
    oVersionSet.insert("ProductName");
    oVersionSet.insert("ProductVersion");
    
    
    // 将当前程序复制一份出来
    amo::loader loader;
    amo::u8path p(amo::u8path::getExeDir());
    std::string dist_file_name = oConfig.get<std::string>("OriginalFileName");
    amo::u8path dist(dist_file_name);
    dist = p.append_c(dist);
    amo::u8path src(amo::u8path::getFullExeName());
    src.copy_to(dist);
    m_pUpdater->Load(amo::u8string(dist.c_str(), true).to_unicode().c_str());
    
    
    amo::u8string strIcon(oConfig.getString("Icon"), true);
    amo::u8path iconPath(strIcon);
    
    if (iconPath.exists() && iconPath.is_file()) {
        m_pUpdater->SetIcon(strIcon.to_unicode().c_str());
    }
    
    // 设置版本信息
    for (auto iter = oConfig.begin(); iter != oConfig.end(); ++iter) {
    
    
        std::string name = iter->name.GetString();
        
        if (!iter->value.IsString()) {
            continue;
        }
        
        std::string value = iter->value.GetString();
        
        if (oVersionSet.find(name) == oVersionSet.end()) {
            continue;
        }
        
        if (name == "FileVersion") {
            unsigned short v1, v2, v3, v4;
            
            if (!parse_version_string(amo::u8string(value, true).to_unicode().c_str(), &v1, &v2, &v3, &v4)) {
                continue;
            }
            
            m_pUpdater->SetFileVersion(v1, v2, v3, v4);
            
        } else if (name == "ProductVersion") {
            unsigned short v1, v2, v3, v4;
            
            if (!parse_version_string(amo::u8string(value, true).to_unicode().c_str(), &v1, &v2, &v3, &v4)) {
                continue;
            }
            
            m_pUpdater->SetProductVersion(v1, v2, v3, v4);
        }
        
        m_pUpdater->SetVersionString(amo::u8string(name, true).to_unicode().c_str(),
                                     amo::u8string(value, true).to_unicode().c_str());
    }
    
    // 设置启动信息 AppSettings
    m_pUpdater->ChangeString(IDS_APP_SETTINGS,
                             strConfigAppSettings.to_unicode().c_str());
    // 界面信息 BrowserSettings
    m_pUpdater->ChangeString(IDS_BROWSER_SETTINGS,
                             strConfigBrowserSettinggs.to_unicode().c_str());
    // 启动画面 SplashSettings
    m_pUpdater->ChangeString(IDS_SPLASH_SETTINGS,
                             strConfigSplashSettinggs.to_unicode().c_str());
    bool bOk =  m_pUpdater->Commit();
    
    if (!bOk) {
        return false;
    }
    
    //
    if (!oConfig.contains_key("resources")) {
        return bOk;
    }
    
    amo::u8json resources = oConfig.getJson("resources");
    
    if (!resources.is_array()) {
        return false;
    }
    
    std::vector<amo::u8json> arr = resources.to_array();
    BinResourceUpdater binUpdater(dist.to_string());
    
    for (auto& p : arr) {
        if (!p.is_object()) {
            return false;
        }
        
        std::string path = p.getString("path");
        int id = p.getInt("id", 131);
        binUpdater.updateZipResource(path, id);
    }
    
    return true;
}


amo::RceditTransfer::RceditTransfer()
    : ClassTransfer("rcedit") {
    m_pUpdater.reset(new amo::ResourceUpdater());
    
    
}



amo::RceditTransfer::~RceditTransfer() {
    m_pUpdater.reset();
}

amo::Any amo::RceditTransfer::loadDiskSettings(IPCMessage::SmartType msg) {
    std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
    
    if (args->getArgsSize() == 0) {
        return false;
    }
    
    amo::u8string strPath(args->getString(0), true);
    amo::u8path p(strPath);
    
    if (p.file_exists()) {
        std::ifstream ifs(p.c_str());
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        std::string strJson(buffer.str());
        m_oSettings = amo::u8json(strJson);
        
        if (!m_oSettings.is_valid()) {
            m_oSettings = amo::u8json();
            return false;
        }
    }
    
    return true;
}

amo::Any amo::RceditTransfer::getDefaultFileSettings(IPCMessage::SmartType
        msg) {
    std::set<std::string> oVersionSet;
    oVersionSet.insert("CompanyName");
    oVersionSet.insert("FileDescription");
    oVersionSet.insert("FileVersion");
    oVersionSet.insert("InternalName");
    oVersionSet.insert("LegalCopyright");
    oVersionSet.insert("OriginalFilenName");
    oVersionSet.insert("ProductName");
    oVersionSet.insert("ProductVersion");
    amo::u8json json;
    json.put("CompanyName", u8"NCUI");
    json.put("FileDescription", u8"NCUI演示程序");
    json.put("FileVersion", u8"1.0.0.0");
    json.put("InternalName", u8"NCUIDemo.exe");
    json.put("LegalCopyright", u8"Copyright (C) 2017");
    json.put("OriginalFileName", u8"NCUIDemo.exe");
    json.put("ProductName", u8"NCUI演示程序");
    json.put("ProductVersion", u8"1.0.0.0");
    json.put("Icon", "");
    amo::u8json fileJson = m_oSettings.get_child("fileSettings");
    
    if (fileJson.is_valid()) {
        json.join(fileJson);
    }
    
    return json.to_utf8();
}

amo::Any amo::RceditTransfer::getDefaultAppSettings(IPCMessage::SmartType msg) {
    HINSTANCE hInst = ::GetModuleHandle(NULL);
    StringLoader strLoader(hInst);
    std::wstring strAppSettings = strLoader.load(IDS_APP_SETTINGS);
    amo::u8json json(amo::u8string(strAppSettings).to_utf8());
    
    if (json.is_valid()) {
        amo::u8json appJson = m_oSettings.get_child("appSettings");
        
        if (appJson.is_valid()) {
            json.join(appJson);
        }
    }
    
    return json.to_utf8();
}

amo::Any amo::RceditTransfer::getDefaultBrowserSettings(
    IPCMessage::SmartType msg) {
    HINSTANCE hInst = ::GetModuleHandle(NULL);
    StringLoader strLoader(hInst);
    std::wstring strBrowserSettings = strLoader.load(IDS_BROWSER_SETTINGS);
	amo::u8json json(amo::u8string(strBrowserSettings).to_utf8());
 
    
    if (json.is_valid()) {
        amo::u8json browserWindowJson = m_oSettings.get_child("browserWindowSettings");
        
        if (browserWindowJson.is_valid()) {
            json.join(browserWindowJson);
        }
    }
    
    return json.to_utf8();
}

amo::Any amo::RceditTransfer::getDefaultSplashSettings(IPCMessage::SmartType
        msg) {
    HINSTANCE hInst = ::GetModuleHandle(NULL);
    StringLoader strLoader(hInst);
    std::wstring strsplashSettings = strLoader.load(IDS_SPLASH_SETTINGS);
	amo::u8json json(amo::u8string(strsplashSettings).to_utf8());
	 
    
    if (json.is_valid()) {
        amo::u8json splashWindowJson = m_oSettings.get_child("splashWindowSettings");
        
        if (splashWindowJson.is_valid()) {
            json.join(splashWindowJson);
        }
    }
    
    return json.to_utf8();
}

amo::Any amo::RceditTransfer::getDefaultSettings(IPCMessage::SmartType msg) {
    amo::u8json json;
    json.put("fileSettings", getDefaultFileSettings(msg).As<amo::u8json>());
    json.put("appSettings", getDefaultAppSettings(msg).As<amo::u8json>());
    json.put("browserWindowSettings",
             getDefaultBrowserSettings(msg).As<amo::u8json>());
    json.put("splashWindowSettings", getDefaultSplashSettings(msg).As<amo::u8json>());
    return json;
}
