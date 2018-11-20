#include "stdafx.h"

#include "node/CefCallbackHandler.h"
#include "handler/ClientHandler.h"
#include "node/NodeHandlerHelper.h"
namespace amo {



    CefCallbackHandler::CefCallbackHandler(int64_t nObjectID)
        : NodeContextMenuHandler(m_pHelper)
        , NodeDialogHandler(m_pHelper)
        , NodeDisplayHandler(m_pHelper)
        , NodeDownloadHandler(m_pHelper)
        , NodeDragHandler(m_pHelper)
        , NodeFocusHandler(m_pHelper)
        , NodeGeolocationHandler(m_pHelper)
        , NodeJSDialogHandler(m_pHelper)
        , NodeKeyboardHandler(m_pHelper)
        , NodeLifeSpanHandler(m_pHelper)
        , NodeLoadHandler(m_pHelper)
        , NodeRenderHandler(m_pHelper)
        , NodeRequestHandler(m_pHelper) {
        m_pHelper.reset(new NodeHandlerHelper());
        m_pHelper->setObjectID(nObjectID);
    }
    
    void CefCallbackHandler::registerHandlerDelegate(CefRefPtr<amo::ClientHandler> pClientHandler) {
        pClientHandler->RegisterContextMenuHandlerDelegate(this, 0);
        pClientHandler->RegisterDialogHandlerDelegate(this, 0);
        pClientHandler->RegisterDisplayHandlerDelegate(this, 0);
        pClientHandler->RegisterDownloadHandlerDelegate(this, 0);
        pClientHandler->RegisterDragHandlerDelegate(this, 0);
        pClientHandler->RegisterFocusHandlerDelegate(this, 0);
        pClientHandler->RegisterGeolocationHandlerDelegate(this, 0);
        pClientHandler->RegisterJSDialogHandlerDelegate(this, 0);
        pClientHandler->RegisterKeyboardHandlerDelegate(this, 0);
        pClientHandler->RegisterLifeSpanHandlerDelegate(this, 0);
        pClientHandler->RegisterLoadHandlerDelegate(this, 0);
        pClientHandler->RegisterRenderHandlerDelegate(this, 0);
        pClientHandler->RegisterRequestHandlerDelegate(this, 0);
        
    }
    
    void CefCallbackHandler::unregisterHandlerDegate(CefRefPtr<ClientHandler> pClientHandler) {
        pClientHandler->UnregisterContextMenuHandlerDelegate(this);
        pClientHandler->UnregisterDialogHandlerDelegate(this);
        pClientHandler->UnregisterDisplayHandlerDelegate(this);
        pClientHandler->UnregisterDownloadHandlerDelegate(this);
        pClientHandler->UnregisterDragHandlerDelegate(this);
        pClientHandler->UnregisterFocusHandlerDelegate(this);
        pClientHandler->UnregisterGeolocationHandlerDelegate(this);
        pClientHandler->UnregisterJSDialogHandlerDelegate(this);
        pClientHandler->UnregisterKeyboardHandlerDelegate(this);
        pClientHandler->UnregisterLifeSpanHandlerDelegate(this);
        pClientHandler->UnregisterLoadHandlerDelegate(this);
        pClientHandler->UnregisterRenderHandlerDelegate(this);
        pClientHandler->UnregisterRequestHandlerDelegate(this);
    }
    
    
    
}

