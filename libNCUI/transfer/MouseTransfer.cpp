#include "stdafx.h"
#include "transfer/MouseTransfer.h"


namespace amo {

    MouseTransfer::MouseTransfer()
        : ClassTransfer("mouse") {
        pt = { 0 };
        ::GetCursorPos(&pt);
    }
    
    Any MouseTransfer::leftClick(IPCMessage::SmartType msg) {
        return click(msg);
    }
    
    Any MouseTransfer::leftDown(IPCMessage::SmartType msg) {
        return  SendMouseEvent(msg, MOUSEEVENTF_LEFTDOWN);
    }
    
    Any MouseTransfer::leftUp(IPCMessage::SmartType msg) {
        return  SendMouseEvent(msg, MOUSEEVENTF_LEFTUP);
    }
    
    Any MouseTransfer::leftDoubleClick(IPCMessage::SmartType msg) {
        click(msg);
        click(msg);
        return Undefined();
    }
    
    Any MouseTransfer::rightClick(IPCMessage::SmartType msg) {
        return SendMouseEvent(msg, MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP);
    }
    
    Any MouseTransfer::rightDown(IPCMessage::SmartType msg) {
        return SendMouseEvent(msg, MOUSEEVENTF_RIGHTDOWN);
    }
    
    Any MouseTransfer::rightUp(IPCMessage::SmartType msg) {
        return SendMouseEvent(msg, MOUSEEVENTF_RIGHTUP);
    }
    
    Any MouseTransfer::middleClick(IPCMessage::SmartType msg) {
        return SendMouseEvent(msg, MOUSEEVENTF_MIDDLEDOWN | MOUSEEVENTF_MIDDLEUP);
    }
    
    Any MouseTransfer::moveTo(IPCMessage::SmartType msg) {
        return setCursorPos(msg);
    }
    
    Any MouseTransfer::moveR(IPCMessage::SmartType msg) {
    
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        int x = 0;
        int y = 0;
        
        if (args->getValue(0).type() == AnyValueType<amo::u8json>::value) {
            amo::u8json json = args->getJson(0);
            x = json.getInt("x");
            y = json.getInt("y");
            
        } else  if (args->getValue(0).type() == AnyValueType<int>::value
                    && args->getValue(1).type() == AnyValueType<int>::value) {
            x = args->getInt(0);
            y = args->getInt(1);
        } else {
            return Undefined();
        }
        
        POINT pt = { 0 };
        ::GetCursorPos(&pt);
        pt.x += x;
        pt.y += y;
        ::SetCursorPos(pt.x, pt.y);
        
        return Undefined();
    }
    
    Any MouseTransfer::mouseWheel(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        int nDelta = args->getInt(0);
        return SendMouseEvent(msg, MOUSEEVENTF_WHEEL, nDelta);
    }
    
    Any MouseTransfer::saveMousePos(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        
        if (args->getArgsSize() == 0) {
            ::GetCursorPos(&pt);
        } else {
        
            int x = 0;
            int y = 0;
            
            if (args->getValue(0).type() == AnyValueType<amo::u8json>::value) {
                amo::u8json json = args->getJson(0);
                x = json.getInt("x");
                y = json.getInt("y");
                
            } else if (args->getValue(0).type() == AnyValueType<int>::value
                       && args->getValue(1).type() == AnyValueType<int>::value) {
                x = args->getInt(0);
                y = args->getInt(1);
            }
            
            pt.x = x;
            pt.y = y;
        }
        
        return Undefined();
    }
    
    Any MouseTransfer::restoreMousePos(IPCMessage::SmartType msg) {
        msg->getArgumentList()->setValue(0, (int)pt.x);
        msg->getArgumentList()->setValue(1, (int)pt.y);
        return moveTo(msg);
    }
    
    Any MouseTransfer::lockMouse(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any MouseTransfer::unlockMouse(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any MouseTransfer::waitClick(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any MouseTransfer::getLastClick(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any MouseTransfer::getCursorPos(IPCMessage::SmartType msg) {
        POINT pt = { 0 };
        ::GetCursorPos(&pt);
        amo::u8json json;
        json.put("x", (int)pt.x);
        json.put("y", (int)pt.y);
        return json;
        
    }
    
    Any MouseTransfer::setCursorPos(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        int x = 0;
        int y = 0;
        
        if (args->getValue(0).type() == AnyValueType<amo::u8json>::value) {
            amo::u8json json = args->getJson(0);
            x = json.getInt("x");
            y = json.getInt("y");
            
        } else  if (args->getValue(0).type() == AnyValueType<int>::value
                    && args->getValue(1).type() == AnyValueType<int>::value) {
            x = args->getInt(0);
            y = args->getInt(1);
        } else {
            return Undefined();
        }
        
        ::SetCursorPos(x, y);
        return Undefined();
    }
    
    Any MouseTransfer::screenToClient(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        auto manager = BrowserWindowManager::getInstance();
        std::shared_ptr<LocalWindow> pWindow;
        
        int nBrowserID = args->getInt(IPCArgsPosInfo::BrowserID);
        
        int x = 0;
        int y = 0;
        
        if (args->getValue(0).is<int>()) {
            pWindow = manager->findWindow(nBrowserID);
            x = args->getInt(0);
            y = args->getInt(1);
        } else {
            std::string strID = args->getString(0);
            pWindow = manager->findWindow(strID);
            x = args->getInt(1);
            y = args->getInt(2);
        }
        
        if (!pWindow) {
            return Undefined();
        }
        
        amo::u8json json;
        POINT pt = { x, y };
        ::ScreenToClient(pWindow->GetHWND(), &pt);
        json.put("x", (int)pt.x);
        json.put("y", (int)pt.y);
        return json;
    }
    
    Any MouseTransfer::clientToScreen(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        
        auto manager = BrowserWindowManager::getInstance();
        std::shared_ptr<LocalWindow> pWindow;
        
        int nBrowserID = args->getInt(IPCArgsPosInfo::BrowserID);
        
        int x = 0;
        int y = 0;
        
        if (args->getValue(0).is<int>()) {
            pWindow = manager->findWindow(nBrowserID);
            x = args->getInt(0);
            y = args->getInt(1);
        } else {
            std::string strID = args->getString(0);
            pWindow = manager->findWindow(strID);
            x = args->getInt(1);
            y = args->getInt(2);
        }
        
        amo::u8json json;
        
        if (!pWindow) {
            return Undefined();
        }
        
        POINT pt = { x, y };
        ::ClientToScreen(pWindow->GetHWND(), &pt);
        json.put("x", (int)pt.x);
        json.put("y", (int)pt.y);
        return json;
    }
    
    Any MouseTransfer::click(IPCMessage::SmartType msg) {
        return  SendMouseEvent(msg, MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP);
    }
    
    Any MouseTransfer::hideCursor(IPCMessage::SmartType msg) {
        ::ShowCursor(FALSE);
        return Undefined();
    }
    
    Any MouseTransfer::showCursor(IPCMessage::SmartType msg) {
        ::ShowCursor(TRUE);
        return Undefined();
    }
    
    Any MouseTransfer::SendMouseEvent(IPCMessage::SmartType msg, uint32_t dwFlags,
                                      int mouseData) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        
        int x = args->getInt(0);
        int y = args->getInt(1);
        
        POINT pt = { 0 };
        ::GetCursorPos(&pt);
        
        if (!args->isValid(0)) {
            x = pt.x;
        }
        
        if (!args->isValid(1)) {
            y = pt.y;
        }
        
        if (mouseData != 0) {
            x = pt.x;
            y = pt.y;
        }
        
        INPUT input;
        input.type = INPUT_MOUSE;
        input.mi.mouseData = mouseData;
        input.mi.dx = static_cast<long>(65535.0
                                        / (GetSystemMetrics(SM_CXSCREEN) - 1) * x);
        input.mi.dy = static_cast<long>(65535.0
                                        / (GetSystemMetrics(SM_CYSCREEN) - 1) * y);
                                        
                                        
                                        
        input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE
                           | MOUSEEVENTF_MOVE
                           | dwFlags;
        input.mi.time = 0; // 不设置为0的话显示器会黑屏
        SendInput(1, &input, sizeof(INPUT));
        return Undefined();
    }
    
}
