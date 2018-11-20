// Created by amoylel on 7/02/2017.
// Copyright (c) 2017 amoylel. All rights reserved.
#ifndef AMO_DUMMYCLIENTHANDLER_H__
#define AMO_DUMMYCLIENTHANDLER_H__

#include <memory>

#include "handler/CefHeader.hpp"

#include <amo/pipe.hpp>
#include <amo/singleton.hpp>
#include "handler/MessageRouterBrowserSide.h"

namespace amo {


    class ContextMenuHandler;
    class DialogHandler;
    class DisplayHandler;
    class DownloadHandler;
    class DragHandler;
    class FocusHandler;
    class GeolocationHandler;
    class JSDialogHandler;
    class KeyboardHandler;
    class LifeSpanHandler;
    class LoadHandler;
    class RenderHandler;
    class RequestHandler;
    class MessageRouterBrowserSide;
    
    class ContextMenuHandlerDelegate;
    class DialogHandlerDelegate;
    class DisplayHandlerDelegate;
    class DownloadHandlerDelegate;
    class DragHandlerDelegate;
    class FocusHandlerDelegate;
    class GeolocationHandlerDelegate;
    class JSDialogHandlerDelegate;
    class KeyboardHandlerDelegate;
    class LifeSpanHandlerDelegate;
    class LoadHandlerDelegate;
    class RenderHandlerDelegate;
    class RequestHandlerDelegate;
    class MessageRouterBrowserSide;
    class MessageHandlerDelegate;
    class ClientHandlerDelegate;
    
    class DummyClientHandler : public amo::singleton < DummyClientHandler > {
    public:
        typedef std::set<MessageRouterBrowserSide::Handler*> MessageHandlerSet;
    public:
        DummyClientHandler();
        virtual ~DummyClientHandler();
        
        virtual CefRefPtr<ContextMenuHandler> GetContextMenuHandler();
        virtual CefRefPtr<DialogHandler> GetDialogHandler();
        virtual CefRefPtr<DisplayHandler> GetDisplayHandler();
        virtual CefRefPtr<DownloadHandler> GetDownloadHandler();
        virtual CefRefPtr<DragHandler> GetDragHandler();
        virtual CefRefPtr<FocusHandler> GetFocusHandler();
        virtual CefRefPtr<GeolocationHandler> GetGeolocationHandler();
        virtual CefRefPtr<JSDialogHandler> GetJSDialogHandler();
        virtual CefRefPtr<KeyboardHandler> GetKeyboardHandler();
        virtual CefRefPtr<LifeSpanHandler> GetLifeSpanHandler();
        virtual CefRefPtr<LoadHandler> GetLoadHandler();
        virtual CefRefPtr<RenderHandler> GetRenderHandler();
        virtual CefRefPtr<RequestHandler> GetRequestHandler();
        
        bool RegisterDelegate(ClientHandlerDelegate* delegate);
        bool RegisterContextMenuHandlerDelegate(ContextMenuHandlerDelegate* delegate);
        void UnregisterContextMenuHandlerDelegate(ContextMenuHandlerDelegate* delegate);
        bool RegisterDialogHandlerDelegate(DialogHandlerDelegate* delegate);
        void UnregisterDialogHandlerDelegate(DialogHandlerDelegate* delegate);
        bool RegisterDisplayHandlerDelegate(DisplayHandlerDelegate* delegate);
        void UnregisterDisplayHandlerDelegate(DisplayHandlerDelegate* delegate);
        bool RegisterDownloadHandlerDelegate(DownloadHandlerDelegate* delegate);
        void UnregisterDownloadHandlerDelegate(DownloadHandlerDelegate* delegate);
        bool RegisterDragHandlerDelegate(DragHandlerDelegate* delegate);
        void UnregisterDragHandlerDelegate(DragHandlerDelegate* delegate);
        bool RegisterFocusHandlerDelegate(FocusHandlerDelegate* delegate);
        void UnregisterFocusHandlerDelegate(FocusHandlerDelegate* delegate);
        bool RegisterGeolocationHandlerDelegate(GeolocationHandlerDelegate* delegate);
        void UnregisterGeolocationHandlerDelegate(GeolocationHandlerDelegate* delegate);
        bool RegisterJSDialogHandlerDelegate(JSDialogHandlerDelegate* delegate);
        void UnregisterJSDialogHandlerDelegate(JSDialogHandlerDelegate* delegate);
        bool RegisterKeyboardHandlerDelegate(KeyboardHandlerDelegate* delegate);
        void UnregisterKeyboardHandlerDelegate(KeyboardHandlerDelegate* delegate);
        bool RegisterLifeSpanHandlerDelegate(LifeSpanHandlerDelegate* delegate);
        void UnregisterLifeSpanHandlerDelegate(LifeSpanHandlerDelegate* delegate);
        bool RegisterLoadHandlerDelegate(LoadHandlerDelegate* delegate);
        void UnregisterLoadHandlerDelegate(LoadHandlerDelegate* delegate);
        bool RegisterRenderHandlerDelegate(RenderHandlerDelegate* delegate);
        void UnregisterRenderHandlerDelegate(RenderHandlerDelegate* delegate);
        bool RegisterRequestHandlerDelegate(RequestHandlerDelegate* delegate);
        void UnregisterRequestHandlerDelegate(RequestHandlerDelegate* delegate);
        bool RegisterMesssageHandlerDelegate(MessageHandlerDelegate* delegate, bool first = false);
        void UnregisterMesssageHandlerDelegate(MessageHandlerDelegate* delegate);
        
        MessageHandlerSet& GetMessageHandlerDelegates();
        
    private:
    
        CefRefPtr<ContextMenuHandler> m_pContextMenuHandler;
        CefRefPtr<DialogHandler> m_pDialogHandler;
        CefRefPtr<DisplayHandler> m_pDisplayHandler;
        CefRefPtr<DownloadHandler> m_pDownloadHandler;
        CefRefPtr<DragHandler> m_pDragHandler;
        CefRefPtr<FocusHandler> m_pFocusHandler;
        CefRefPtr<GeolocationHandler> m_pGeolocationHandler;
        CefRefPtr<JSDialogHandler> m_pJSDialogHandler;
        CefRefPtr<KeyboardHandler> m_pKeyboardHandler;
        CefRefPtr<LifeSpanHandler> m_pLifeSpanHandler;
        CefRefPtr<LoadHandler> m_pLoadHandler;
        CefRefPtr<RenderHandler> m_pRenderHandler;
        CefRefPtr<RequestHandler> m_pRequestHandler;
        
        MessageHandlerSet m_MessageSet;
    };
    
}
#endif // AMO_DUMMYCLIENTHANDLER_H__
