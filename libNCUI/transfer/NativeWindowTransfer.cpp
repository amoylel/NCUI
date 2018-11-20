#include "stdafx.h"
#include "transfer/NativeWindowTransfer.h"
#include "ui/win/NativeWindow.h"
#include "ui/win/BrowserWindowManager.h"
#include <memory>
#include "settings/NativeWindowSettings.h"

namespace amo {

    NativeWindowTransfer::NativeWindowTransfer()
        : ClassTransfer("NativeWindow") {
        addModule("EventEmitter");
    }
    
    Any NativeWindowTransfer::onCreateClass(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        amo::u8string str(args->getString(0), true);
        
        auto manager = BrowserWindowManager::getInstance();
        std::shared_ptr<NativeWindowSettings> pSettings(new NativeWindowSettings());
        pSettings->updateArgsSettings(str);
        std::shared_ptr<NativeWindow> window;
        window = manager->createNativeWindow(pSettings)->toNativeWindow();
        
        window->setFrameID(args->getInt64(IPCArgsPosInfo::FrameID));
        window->setBrowserID(args->getInt(IPCArgsPosInfo::BrowserID));
        
        window->registerFunction();
        addTransfer(window);
        return  window->getFuncMgr().toSimplifiedJson();
    }
    
}

