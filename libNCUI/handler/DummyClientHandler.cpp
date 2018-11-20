#include "stdAfx.h"
#include "handler/DummyClientHandler.h"

#include "handler/HandlerDelegate.hpp"
#include "handler/ContextMenuHandler.h"
#include "handler/DialogHandler.h"
#include "handler/DisplayHandler.h"
#include "handler/DownloadHandler.h"
#include "handler/DragHandler.h"
#include "handler/GeolocationHandler.h"
#include "handler/KeyboardHandler.h"
#include "handler/LifeSpanHandler.h"
#include "handler/LoadHandler.h"
#include "handler/RenderHandler.h"
#include "handler/RequestHandler.h"
#include "handler/FocusHandler.h"
#include "handler/JSDialogHandler.h"
#include "handler/MessageHandlerDelegate.hpp"
#include "handler/MessageRouterBrowserSide.h"
#include "handler/ClientHandler.h"


namespace amo {
    CefRefPtr<RequestHandler> DummyClientHandler::GetRequestHandler() {
        return m_pRequestHandler.get();
    }
    
    bool DummyClientHandler::RegisterDelegate(ClientHandlerDelegate* delegate) {
        return false;
    }
    
    bool DummyClientHandler::RegisterContextMenuHandlerDelegate(ContextMenuHandlerDelegate* delegate) {
        if (!m_pContextMenuHandler) {
            return false;
        }
        
        return m_pContextMenuHandler->RegisterDelegate(delegate);
    }
    
    void DummyClientHandler::UnregisterContextMenuHandlerDelegate(ContextMenuHandlerDelegate* delegate) {
        if (!m_pContextMenuHandler) {
            return;
        }
        
        m_pContextMenuHandler->UnregisterDelegate(delegate);
    }
    
    bool DummyClientHandler::RegisterDialogHandlerDelegate(DialogHandlerDelegate* delegate) {
        if (!m_pDialogHandler) {
            return false;
        }
        
        return m_pDialogHandler->RegisterDelegate(delegate);
    }
    
    void DummyClientHandler::UnregisterDialogHandlerDelegate(DialogHandlerDelegate* delegate) {
        if (!m_pDialogHandler) {
            return;
        }
        
        m_pDialogHandler->UnregisterDelegate(delegate);
    }
    
    bool DummyClientHandler::RegisterDisplayHandlerDelegate(DisplayHandlerDelegate* delegate) {
        if (!m_pDisplayHandler) {
            return false;
        }
        
        return m_pDisplayHandler->RegisterDelegate(delegate);
    }
    
    void DummyClientHandler::UnregisterDisplayHandlerDelegate(DisplayHandlerDelegate* delegate) {
        if (!m_pDisplayHandler) {
            return;
        }
        
        m_pDisplayHandler->UnregisterDelegate(delegate);
    }
    
    bool DummyClientHandler::RegisterDownloadHandlerDelegate(DownloadHandlerDelegate* delegate) {
        if (!m_pDownloadHandler) {
            return false;
        }
        
        return m_pDownloadHandler->RegisterDelegate(delegate);
    }
    
    void DummyClientHandler::UnregisterDownloadHandlerDelegate(DownloadHandlerDelegate* delegate) {
        if (!m_pDownloadHandler) {
            return;
        }
        
        m_pDownloadHandler->UnregisterDelegate(delegate);
    }
    
    bool DummyClientHandler::RegisterDragHandlerDelegate(DragHandlerDelegate* delegate) {
        if (!m_pDragHandler) {
            return false;
        }
        
        return m_pDragHandler->RegisterDelegate(delegate);
    }
    
    void DummyClientHandler::UnregisterDragHandlerDelegate(DragHandlerDelegate* delegate) {
        if (!m_pDragHandler) {
            return;
        }
        
        m_pDragHandler->UnregisterDelegate(delegate);
    }
    
    bool DummyClientHandler::RegisterFocusHandlerDelegate(FocusHandlerDelegate* delegate) {
        if (!m_pFocusHandler) {
            return false;
        }
        
        return m_pFocusHandler->RegisterDelegate(delegate);
    }
    
    void DummyClientHandler::UnregisterFocusHandlerDelegate(FocusHandlerDelegate* delegate) {
        if (!m_pFocusHandler) {
            return;
        }
        
        m_pFocusHandler->UnregisterDelegate(delegate);
    }
    
    bool DummyClientHandler::RegisterGeolocationHandlerDelegate(GeolocationHandlerDelegate* delegate) {
        if (!m_pGeolocationHandler) {
            return false;
        }
        
        return m_pGeolocationHandler->RegisterDelegate(delegate);
    }
    
    void DummyClientHandler::UnregisterGeolocationHandlerDelegate(GeolocationHandlerDelegate* delegate) {
        if (!m_pGeolocationHandler) {
            return;
        }
        
        m_pGeolocationHandler->UnregisterDelegate(delegate);
    }
    
    bool DummyClientHandler::RegisterJSDialogHandlerDelegate(JSDialogHandlerDelegate* delegate) {
        if (!m_pJSDialogHandler) {
            return false;
        }
        
        return m_pJSDialogHandler->RegisterDelegate(delegate);
    }
    
    void DummyClientHandler::UnregisterJSDialogHandlerDelegate(JSDialogHandlerDelegate* delegate) {
        if (!m_pJSDialogHandler) {
            return;
        }
        
        m_pJSDialogHandler->UnregisterDelegate(delegate);
    }
    
    bool DummyClientHandler::RegisterKeyboardHandlerDelegate(KeyboardHandlerDelegate* delegate) {
        if (!m_pKeyboardHandler) {
            return false;
        }
        
        return m_pKeyboardHandler->RegisterDelegate(delegate);
    }
    
    void DummyClientHandler::UnregisterKeyboardHandlerDelegate(KeyboardHandlerDelegate* delegate) {
        if (!m_pKeyboardHandler) {
            return;
        }
        
        m_pKeyboardHandler->UnregisterDelegate(delegate);
    }
    
    bool DummyClientHandler::RegisterLifeSpanHandlerDelegate(LifeSpanHandlerDelegate* delegate) {
        if (!m_pLifeSpanHandler) {
            return false;
        }
        
        return m_pLifeSpanHandler->RegisterDelegate(delegate);
    }
    
    void DummyClientHandler::UnregisterLifeSpanHandlerDelegate(LifeSpanHandlerDelegate* delegate) {
        if (!m_pLifeSpanHandler) {
            return;
        }
        
        m_pLifeSpanHandler->UnregisterDelegate(delegate);
    }
    
    bool DummyClientHandler::RegisterLoadHandlerDelegate(LoadHandlerDelegate* delegate) {
        if (!m_pLoadHandler) {
            return false;
        }
        
        return m_pLoadHandler->RegisterDelegate(delegate);
    }
    
    void DummyClientHandler::UnregisterLoadHandlerDelegate(LoadHandlerDelegate* delegate) {
        if (!m_pLoadHandler) {
            return;
        }
        
        m_pLoadHandler->UnregisterDelegate(delegate);
    }
    
    bool DummyClientHandler::RegisterRenderHandlerDelegate(RenderHandlerDelegate* delegate) {
        if (!m_pRenderHandler) {
            return false;
        }
        
        return m_pRenderHandler->RegisterDelegate(delegate);
    }
    
    void DummyClientHandler::UnregisterRenderHandlerDelegate(RenderHandlerDelegate* delegate) {
        if (!m_pRenderHandler) {
            return;
        }
        
        m_pRenderHandler->UnregisterDelegate(delegate);
    }
    
    bool DummyClientHandler::RegisterRequestHandlerDelegate(RequestHandlerDelegate* delegate) {
        if (!m_pRequestHandler) {
            return false;
        }
        
        return m_pRequestHandler->RegisterDelegate(delegate);
    }
    
    void DummyClientHandler::UnregisterRequestHandlerDelegate(RequestHandlerDelegate* delegate) {
        if (!m_pRequestHandler) {
            return;
        }
        
        m_pRequestHandler->UnregisterDelegate(delegate);
    }
    
    bool DummyClientHandler::RegisterMesssageHandlerDelegate(MessageHandlerDelegate* delegate,
            bool first) {
        return m_MessageSet.insert(delegate).second;
    }
    
    void DummyClientHandler::UnregisterMesssageHandlerDelegate(MessageHandlerDelegate* delegate) {
        m_MessageSet.erase(delegate);
    }
    
    amo::DummyClientHandler::MessageHandlerSet& DummyClientHandler::GetMessageHandlerDelegates() {
        return m_MessageSet;
    }
    
    CefRefPtr<RenderHandler> DummyClientHandler::GetRenderHandler() {
        return m_pRenderHandler.get();
    }
    
    CefRefPtr<LoadHandler> DummyClientHandler::GetLoadHandler() {
        return m_pLoadHandler.get();
    }
    
    CefRefPtr<LifeSpanHandler> DummyClientHandler::GetLifeSpanHandler() {
        return m_pLifeSpanHandler.get();
    }
    
    CefRefPtr<KeyboardHandler> DummyClientHandler::GetKeyboardHandler() {
        return m_pKeyboardHandler.get();
    }
    
    CefRefPtr<JSDialogHandler> DummyClientHandler::GetJSDialogHandler() {
        return m_pJSDialogHandler.get();
    }
    
    CefRefPtr<GeolocationHandler> DummyClientHandler::GetGeolocationHandler() {
        return m_pGeolocationHandler.get();
    }
    
    CefRefPtr<FocusHandler> DummyClientHandler::GetFocusHandler() {
        return m_pFocusHandler.get();
    }
    
    CefRefPtr<DragHandler> DummyClientHandler::GetDragHandler() {
        return m_pDragHandler.get();
    }
    
    CefRefPtr<DownloadHandler> DummyClientHandler::GetDownloadHandler() {
        return m_pDownloadHandler.get();
    }
    
    CefRefPtr<DisplayHandler> DummyClientHandler::GetDisplayHandler() {
        return m_pDisplayHandler.get();
    }
    
    CefRefPtr<DialogHandler> DummyClientHandler::GetDialogHandler() {
        return m_pDialogHandler.get();
    }
    
    CefRefPtr<ContextMenuHandler> DummyClientHandler::GetContextMenuHandler() {
        return m_pContextMenuHandler.get();
    }
    
    DummyClientHandler::DummyClientHandler() {
        m_pContextMenuHandler = new ContextMenuHandler();
        m_pDialogHandler = new DialogHandler();
        m_pDisplayHandler = new DisplayHandler();
        m_pDownloadHandler = new DownloadHandler();
        m_pDragHandler = new DragHandler();
        m_pFocusHandler = new FocusHandler();
        m_pGeolocationHandler = new GeolocationHandler();
        m_pJSDialogHandler = new JSDialogHandler();
        m_pKeyboardHandler = new KeyboardHandler();
        m_pLifeSpanHandler = new LifeSpanHandler();
        m_pLoadHandler = new LoadHandler();
        m_pRenderHandler = new RenderHandler();
        m_pRequestHandler = new RequestHandler();
    }
    
    DummyClientHandler::~DummyClientHandler() {
    
    }
    
}

