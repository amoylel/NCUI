// Created by amoylel on 6/22/2017.
// Copyright (c) 2017 amoylel. All rights reserved.

#ifndef AMO_CONTEXTMENUHANDLER_H__
#define AMO_CONTEXTMENUHANDLER_H__

#include "handler/CefHeader.hpp"

#include "handler/HandlerDelegate.hpp"

namespace amo {

    /*!
     * @class	ContextMenuHandlerDelegate
     *
     * @brief	继承该类处理菜单事件，该类中的方法会在UI线程中被调用.
     */
    
    class ContextMenuHandlerDelegate {
    public:
        ContextMenuHandlerDelegate() {}
        virtual ~ContextMenuHandlerDelegate() {}
        
        virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                         CefRefPtr<CefFrame> frame,
                                         CefRefPtr<CefContextMenuParams> params,
                                         CefRefPtr<CefMenuModel> model) {
                                         
        }
        
        virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefRefPtr<CefContextMenuParams> params,
                                          int command_id,
                                          CefContextMenuHandler::EventFlags event_flags) {
            return false;
        }
        
        virtual void OnContextMenuDismissed(CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefFrame> frame) {
                                            
        }
        
        
    };
    
    /*!
     * @class	ContextMenuHandler
     *
     * @brief	菜单事件处理类，该类中的方法会在UI线程中被调用.
     */
    
    class ContextMenuHandler : public CefContextMenuHandler
        , public HandlerDelegate < ContextMenuHandlerDelegate > {
    public:
        ContextMenuHandler();
        
        virtual ~ContextMenuHandler();
        
        IMPLEMENT_REFCOUNTING(ContextMenuHandler);
        
        /*!
         * @fn	virtual void ContextMenuHandler::OnBeforeContextMenu(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefFrame> frame,
         * 		CefRefPtr<CefContextMenuParams> params,
         * 		CefRefPtr<CefMenuModel> model);
         *
         * @brief	菜单显示前触发该函数，用于定制菜单
         * 			B UI.
         *
         * @param	browser	The browser.
         * @param	frame  	The frame.
         * @param	params 	A variable-length parameters list containing parameters.
         * @param	model  	The model.
         */
        
        virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                         CefRefPtr<CefFrame> frame,
                                         CefRefPtr<CefContextMenuParams> params,
                                         CefRefPtr<CefMenuModel> model) override;
                                         
        /*!
         * @fn	virtual bool ContextMenuHandler::OnContextMenuCommand(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefFrame> frame,
         * 		CefRefPtr<CefContextMenuParams> params,
         * 		int command_id,
         * 		EventFlags event_flags);
         *
         * @brief	执行菜单命令时触发
         * 			B UI.
         *
         * @param	browser	   	The browser.
         * @param	frame	   	The frame.
         * @param	params	   	A variable-length parameters list containing parameters.
         * @param	command_id 	Identifier for the command.
         * @param	event_flags	The event flags.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefRefPtr<CefContextMenuParams> params,
                                          int command_id,
                                          EventFlags event_flags) override;
                                          
        /*!
         * @fn	virtual void ContextMenuHandler::OnContextMenuDismissed(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefFrame> frame);
         *
         * @brief	菜单事件完成时回调包括空菜单或菜单命令被选中后回调
         * 			B UI.
         *
         * @param	browser	The browser.
         * @param	frame  	The frame.
         */
        
        virtual void OnContextMenuDismissed(CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefFrame> frame) override;
                                            
    };
}
#endif // AMO_CONTEXTMENUHANDLER_H__
