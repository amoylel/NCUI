// Created by amoylel on 06/12/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_BROWSERMANAGER_HPP__
#define AMO_BROWSERMANAGER_HPP__

#include <unordered_map>
#include "handler/CefHeader.hpp"


namespace amo {
    /*!
     * @class	BrowserManager
     *
     * @brief	管理所有的浏览器.
     *
     * @tparam	cef_process_id_t	进程类型.
     */
    template<cef_process_id_t>
    class BrowserManager {
    public:
    
        /*!
         * @fn	static CefRefPtr<CefBrowser> BrowserManager::GetAnyBrowser()
         *
         * @brief	获取一个Browserr 任意一个都可以.
         *
         * @return	any browser.
         */
        static CefRefPtr<CefBrowser> GetAnyBrowser() {
            auto iter =  m_oBrowserMap.begin();
            
            if (iter == m_oBrowserMap.end()) {
                return NULL;
            }
            
            return iter->second;
        }
        
        
        /*!
         * @fn	static CefRefPtr<CefBrowser> BrowserManager::GetBrowserByID(int nBrowserID)
         *
         * @brief	通过浏览器ID查找浏览器CefBrowser.
         *
         * @param	nBrowserID	Identifier for the browser.
         *
         * @return	The browser by identifier.
         */
        static CefRefPtr<CefBrowser> GetBrowserByID(int nBrowserID) {
            if (nBrowserID <= 0) {
                return NULL;
            }
            
            CefRefPtr<CefBrowser> pBrowser;
            auto iter = m_oBrowserMap.find(nBrowserID);
            
            if (iter == m_oBrowserMap.end()) {
                return pBrowser;
            }
            
            return iter->second;
        }
        
        /*!
         * @fn	static CefRefPtr<CefBrowser> BrowserManager::GetBrowserByFrmeID(int nFrameID)
         *
         * @brief	通过CefFrame ID查找CefBrowser.
         *
         * @param	nFrameID	Identifier for the frame.
         *
         * @return	The browser by frme identifier.
         */
        static CefRefPtr<CefBrowser> GetBrowserByFrmeID(int nFrameID) {
            if (nFrameID <= 0) {
                return NULL;
            }
            
            for (auto& p : m_oBrowserMap) {
                if (p.second->GetFrame(nFrameID)) {
                    return p.second;
                }
            }
            
            return NULL;
        }
        
        /*!
         * @fn	static CefRefPtr<CefFrame> BrowserManager::GetFrameByID(int nFrameID)
         *
         * @brief	通过CefFrame ID 查找CefFrame.
         *
         * @param	nFrameID	Identifier for the frame.
         *
         * @return	The frame by identifier.
         */
        static CefRefPtr<CefFrame> GetFrameByID(int nFrameID) {
            if (nFrameID <= 0) {
                return NULL;
            }
            
            for (auto& p : m_oBrowserMap) {
                CefRefPtr<CefFrame> pFrame = p.second->GetFrame(nFrameID);
                
                if (pFrame) {
                    return pFrame;
                }
            }
            
            return NULL;
        }
        
        /*!
         * @fn	static void BrowserManager::RemoveBrowserByID(int nBrowserID)
         *
         * @brief   通过CefBrowser ID 移除 CefBrowser.
         * 			一般在DoClose函数中移除管理
         *
         * @param	nBrowserID	Identifier for the browser.
         */
        static void RemoveBrowserByID(int nBrowserID) {
            m_oBrowserMap.erase(nBrowserID);
        }
        
        /*!
         * @fn	static void BrowserManager::RegisterBrowser(CefRefPtr<CefBrowser> pBrowser)
         *
         * @brief	添加CefBrowser到管理器中.
         * 			一般在OnAfterCreate函数中添加管理
         *
         * @param	pBrowser	The browser.
         */
        static void RegisterBrowser(CefRefPtr<CefBrowser> pBrowser) {
            $clog(amo::cdevel << func_orient << "11111" << pBrowser->GetIdentifier() << amo::endl;);
            m_oBrowserMap[pBrowser->GetIdentifier()] = pBrowser;
        }
        
        /*!
         * @fn	static std::unordered_map<int, CefRefPtr<CefBrowser> >& BrowserManager::GetAllBrowser()
         *
         * @brief	获取管理器中的所有CefBrowser.
         *
         * @return	all browser.
         */
        static std::unordered_map<int, CefRefPtr<CefBrowser> >& GetAllBrowser() {
            return m_oBrowserMap;
        }
        
        /*! @brief	The browser map. */
        static std::unordered_map<int, CefRefPtr<CefBrowser> > m_oBrowserMap;
    };
    
    /*! @brief	初始化静态变量. */
    std::unordered_map<int, CefRefPtr<CefBrowser> >
    BrowserManager<PID_BROWSER>::m_oBrowserMap;
    /*! @brief	初始化静态变量. */
    std::unordered_map<int, CefRefPtr<CefBrowser> >
    BrowserManager<PID_RENDERER>::m_oBrowserMap;
}

#endif // AMO_BROWSERMANAGER_HPP__

