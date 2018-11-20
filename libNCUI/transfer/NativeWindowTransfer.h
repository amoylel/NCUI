// Created by amoylel on 06/30/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NATIVEWINDOWTRANSFER_H__
#define AMO_NATIVEWINDOWTRANSFER_H__

#include <memory>
#include <amo/singleton.hpp>

#include "transfer/ClassTransfer.hpp"
#include "ui/win/NativeWindowProxy.h"

namespace amo {

    class NativeWindowTransfer
        : public ClassTransfer
        , virtual public NativeWindowProxy
        , public amo::singleton<NativeWindowTransfer> {
    public:
    
        NativeWindowTransfer();
        
        virtual Any onCreateClass(IPCMessage::SmartType msg) override;
        
        
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(NativeWindowTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getName, TransferExecNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setName, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(close, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isFocused, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(show, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(showInactive, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(hide, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isVisible, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(maximize, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(unmaximize, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isMaximized, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(minimize, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(restore, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isMinimized, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setFullScreen, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isFullScreen, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setWindowRect, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getWindowRect, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setSize, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getSize, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setMinimumSize, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getMinimumSize, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setMaximumSize, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getMaximumSize, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setResizable, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isResizable, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setMovable, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isMovable, TransferExecSync)
        
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setMinimizable, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isMinimizable, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setMaximizable, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isMaximizable, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setFullScreenable, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isFullScreenable, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setClosable, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isClosable, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setAlwaysOnTop, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isAlwaysOnTop, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(center, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setPosition, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getPosition, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setTitle, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getTitle, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(flashFrame, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(topmost, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isTopmost, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setHasShadow, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(hasShadow, TransferExecSync)
        
        
        AMO_CEF_MESSAGE_TRANSFER_FUNC(toggleFullScreen, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(toggleVisible, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(showModal, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setIcon, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setTheme, TransferExecNormal)
        
        AMO_CEF_MESSAGE_TRANSFER_END()
        
        
    private:
    };
}

#endif // AMO_NATIVEWINDOWTRANSFER_H__
