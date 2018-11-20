// Created by amoylel on 07/25/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_CEFCALLBACKHANDLER_H__
#define AMO_CEFCALLBACKHANDLER_H__




#include <memory>
#include "handler/CefHeader.hpp"

#include "NodeContextMenuHandler.h"
#include "NodeDialogHandler.h"
#include "NodeDisplayHandler.h"
#include "NodeDownloadHandler.h"
#include "NodeDragHandler.h"
#include "NodeFocusHandler.h"
#include "NodeGeolocationHandler.h"
#include "NodeJSDialogHandler.h"
#include "NodeKeyboardHandler.h"
#include "NodeLifeSpanHandler.h"
#include "NodeLoadHandler.h"
#include "NodeRenderHandler.h"
#include "NodeRequestHandler.h"


namespace amo {

    class UIMessageEmitter;
    class ClientHandler;
    class NodeHandlerHelper;
    
    class CefCallbackHandler
        : public NodeContextMenuHandler
        , public NodeDialogHandler
        , public NodeDisplayHandler
        , public NodeDownloadHandler
        , public NodeDragHandler
        , public NodeFocusHandler
        , public NodeGeolocationHandler
        , public NodeJSDialogHandler
        , public NodeKeyboardHandler
        , public NodeLifeSpanHandler
        , public NodeLoadHandler
        , public NodeRenderHandler
        , public NodeRequestHandler
        
    {
    public:
        CefCallbackHandler(int64_t nObjectID);
        
        void registerHandlerDelegate(CefRefPtr<ClientHandler> pClientHandler);
        void unregisterHandlerDegate(CefRefPtr<ClientHandler> pClientHandler);
        
        
    protected:
    private:
        std::shared_ptr<NodeHandlerHelper> m_pHelper;
    };
}


#endif // AMO_CEFCALLBACKHANDLER_H__
