// Created by amoylel on 6/22/2017.
// Copyright (c) 2017 amoylel. All rights reserved.

#ifndef AMO_DRAGHANDLER_H__
#define AMO_DRAGHANDLER_H__

#include "handler/CefHeader.hpp"

#include "handler/HandlerDelegate.hpp"
namespace amo {

    /*!
     * @class	DragHandlerDelegate
     *
     * @brief	拖拽事件处理类，该类中的方法会在UI线程中被调用.
     */
    
    
    class DragHandlerDelegate {
    public:
        DragHandlerDelegate() {}
        
        virtual ~DragHandlerDelegate() {}
        
        virtual bool OnDragEnter(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefDragData> dragData,
                                 CefDragHandler::DragOperationsMask mask) {
            return false;
        }
        
#if CHROME_VERSION_BUILD >= 2704
        virtual void OnDraggableRegionsChanged(
            CefRefPtr<CefBrowser> browser,
            const std::vector<CefDraggableRegion>& regions) {}
#endif
            
            
    };
    
    /*!
     * @class	DragHandler
     *
     * @brief	拖拽事件处理类，该类中的方法会在UI线程中被调用.
     */
    
    class DragHandler : public CefDragHandler
        , public HandlerDelegate < DragHandlerDelegate > {
    public:
        DragHandler() {}
        
        virtual ~DragHandler() {}
        
        /*!
         * @fn	virtual bool DragHandler::OnDragEnter(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefDragData> dragData,
         * 		DragOperationsMask mask);
         *
         * @brief	当有外部拖拽事件进入浏览器窗口时触发
         * 			B UI.
         *
         * @param	browser 	The browser.
         * @param	dragData	Information describing the drag.
         * @param	mask		The mask.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool OnDragEnter(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefDragData> dragData,
                                 DragOperationsMask mask) override;
                                 
#if CHROME_VERSION_BUILD >= 2704
        /*!
        * @fn	virtual void DragHandler::OnDraggableRegionsChanged(
        * 		CefRefPtr<CefBrowser> browser,
        * 		const std::vector<CefDraggableRegion>& regions);
        *
        * @brief	Executes the draggable regions changed action.
        *
        * @param	browser	The browser.
        * @param	regions	The regions.
        */
        
        virtual void OnDraggableRegionsChanged(
            CefRefPtr<CefBrowser> browser,
            const std::vector<CefDraggableRegion>& regions) override;
#endif
            
            
        IMPLEMENT_REFCOUNTING(DragHandler);
        
    };
    
}
#endif // AMO_DRAGHANDLER_H__
