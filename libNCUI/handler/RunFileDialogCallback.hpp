// Created by amoylel on 06/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_RUNFILEDIALOGCALLBACK_H__
#define AMO_RUNFILEDIALOGCALLBACK_H__

#include <utility>
#include <vector>
#include "handler/CefHeader.hpp"
#include "ipc/Any.hpp"
#include "transfer/BrowserTransferMgr.h"

namespace amo {
    class RunFileDialogCalllback : public CefRunFileDialogCallback {
    public:
        RunFileDialogCalllback(int nBrowserID,
                               int64_t nFrameID,
                               int nCallbackID)
            : m_nBrowserID(nBrowserID)
            , m_nFrameID(nFrameID)
            , m_nCallbackID(nCallbackID) {
            
        }
        
        
        virtual void OnFileDialogDismissed(int selected_accept_filter,
                                           const std::vector<CefString>& file_paths) override {
            for (auto& p : file_paths) {
                m_vecFilePaths.push_back(p.ToString());
            }
            
            Any ret = m_vecFilePaths;
            BrowserTransferMgr::getInstance()->returnAsyncResult(m_nBrowserID,
                    ret,
                    m_nCallbackID,
                    m_nFrameID);
        }
        
        /*
        #if CHROME_VERSION_BUILD >= 2704
        
        
        		virtual void OnFileDialogDismissed(int selected_accept_filter,
        			const std::vector<CefString>& file_paths) override {
        			for (auto& p : file_paths) {
        				m_vecFilePaths.push_back(p.ToString());
        			}
        
        			Any ret = m_vecFilePaths;
        			BrowserTransferMgr::getInstance()->returnAsyncResult(m_nBrowserID,
        				ret,
        				m_nCallbackID,
        				m_nFrameID);
        		}
        
        #else
                virtual void OnFileDialogDismissed(CefRefPtr<CefBrowserHost> browser_host,
                                                   const std::vector<CefString>& file_paths)  override {
                    for (auto& p : file_paths) {
                        m_vecFilePaths.push_back(p.ToString());
                    }
        
                    Any ret = m_vecFilePaths;
                    BrowserTransferMgr::getInstance()->returnAsyncResult(m_nBrowserID,
                            ret,
                            m_nCallbackID,
                            m_nFrameID);
                }
        #endif*/
        
        
        
        
        
        
        IMPLEMENT_REFCOUNTING(RunFileDialogCalllback);
        
    private:
        std::vector<Any> m_vecFilePaths;
        int m_nBrowserID;
        int64_t m_nFrameID;
        int m_nCallbackID;
    };
}


#endif // AMO_RUNFILEDIALOGCALLBACK_H__
