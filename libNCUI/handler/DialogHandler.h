// Created by amoylel on 6/22/2017.
// Copyright (c) 2017 amoylel. All rights reserved.

#ifndef AMO_DIALOGHANDLER_H__
#define AMO_DIALOGHANDLER_H__

#include "handler/CefHeader.hpp"

#include "handler/HandlerDelegate.hpp"

namespace amo {

    /*!
     * @class	DialogHandlerDelegate
     *
     * @brief	对话框事件处理类，该类中的方法会在UI线程中被调用.
     */
    
    class DialogHandlerDelegate {
    public:
        DialogHandlerDelegate() {}
        
        virtual ~DialogHandlerDelegate() {}
        
#if CHROME_VERSION_BUILD >= 2272
        virtual bool OnFileDialog(CefRefPtr<CefBrowser> browser,
                                  CefDialogHandler::FileDialogMode mode,
                                  const CefString& title,
                                  const CefString& default_file_path,
                                  const std::vector<CefString>& accept_filters,
                                  int selected_accept_filter,
                                  CefRefPtr<CefFileDialogCallback> callback) {
            return false;
        }
#else
        virtual bool OnFileDialog(CefRefPtr<CefBrowser> browser,
                                  CefDialogHandler::FileDialogMode mode,
                                  const CefString& title,
                                  const CefString& default_file_name,
                                  const std::vector<CefString>& accept_types,
                                  CefRefPtr<CefFileDialogCallback> callback) {
            return false;
        }
        
#endif
        
        
        
        
    };
    
    /*!
     * @class	DialogHandler
     *
     * @brief	对话框事件处理类，该类中的方法会在UI线程中被调用.
     */
    
    class DialogHandler : public CefDialogHandler
        , public HandlerDelegate < DialogHandlerDelegate > {
    public:
        DialogHandler() {}
        
        virtual ~DialogHandler() {}
        
        
#if CHROME_VERSION_BUILD >= 2272
        
        /*!
         * @fn	virtual bool DialogHandler::OnFileDialog(
         * 		CefRefPtr<CefBrowser> browser,
         * 		FileDialogMode mode,
         * 		const CefString& title,
         * 		const CefString& default_file_path,
         * 		const std::vector<CefString>& accept_filters,
         * 		int selected_accept_filter,
         * 		CefRefPtr<CefFileDialogCallback> callback);
         *
         * @brief	页面请求打开文件对话框时触发 包括文件选择、文件保存对话框
         * 			B UI
         *
         * @param	browser				  	The browser.
         * @param	mode				  	The mode.
         * @param	title				  	The title.
         * @param	default_file_path	  	The default file path.
         * @param	accept_filters		  	The accept filters.
         * @param	selected_accept_filter	A filter specifying the selected accept.
         * @param	callback			  	The callback.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool OnFileDialog(CefRefPtr<CefBrowser> browser,
                                  FileDialogMode mode,
                                  const CefString& title,
                                  const CefString& default_file_path,
                                  const std::vector<CefString>& accept_filters,
                                  int selected_accept_filter,
                                  CefRefPtr<CefFileDialogCallback> callback) override;
#else
        virtual bool OnFileDialog(CefRefPtr<CefBrowser> browser,
                                  FileDialogMode mode,
                                  const CefString& title,
                                  const CefString& default_file_name,
                                  const std::vector<CefString>& accept_types,
                                  CefRefPtr<CefFileDialogCallback> callback) override;
#endif
                                  
                                  
        IMPLEMENT_REFCOUNTING(DialogHandler);
    };
}
#endif // AMO_DIALOGHANDLER_H__
