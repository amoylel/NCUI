#include "stdafx.h"
#include "ui/win/NativeWindowProxy.h"
#include <amo/string.hpp>

#include "settings/NativeWindowSettings.h"


namespace amo {

    Any NativeWindowProxy::id(IPCMessage::SmartType msg) {
        return getNativeSettings()->id;
    }
    
    Any NativeWindowProxy::getName(IPCMessage::SmartType msg) {
        return getNativeSettings()->name;
    }
    
    Any NativeWindowProxy::setName(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        getNativeSettings()->name = args->getString(0);
        return Undefined();
    }
    
    Any NativeWindowProxy::showTitleBar(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    
    Any NativeWindowProxy::destroy(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        // 255 强制关闭窗口
        ::PostMessage(getNativeHWND(args), WM_CLOSE, 255, 0);
        return Undefined();
    }
    
    
    Any NativeWindowProxy::close(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        ::PostMessage(getNativeHWND(args), WM_CLOSE, 0, 0);
        return Undefined();
    }
    
    Any NativeWindowProxy::focus(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        ::SetFocus(getNativeHWND(args));
        return Undefined();
    }
    
    
    Any NativeWindowProxy::isFocused(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        HWND hWnd = getNativeHWND(args);
        HWND hFocusWnd = ::GetFocus();
        
        while (hFocusWnd != NULL) {
            if (hWnd == hFocusWnd) {
                return true;
            }
            
            hFocusWnd = ::GetParent(hFocusWnd);
        }
        
        return false;
        //return ::GetFocus() == getNativeHWND(args);
    }
    
    Any NativeWindowProxy::show(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        bool bOk = (::ShowWindow(getNativeHWND(args), SW_SHOW) != FALSE);
        ::SetFocus(getNativeHWND(args));
        ::SwitchToThisWindow(getNativeHWND(args), TRUE);
        return bOk;
    }
    
    Any NativeWindowProxy::showInactive(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        return ::ShowWindow(getNativeHWND(args), SW_SHOWNOACTIVATE) != FALSE;
    }
    
    Any NativeWindowProxy::hide(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        return ::ShowWindow(getNativeHWND(args), SW_HIDE) != FALSE;
    }
    
    Any NativeWindowProxy::isVisible(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        return ::IsWindowVisible(getNativeHWND(args)) != FALSE;
    }
    
    Any NativeWindowProxy::maximize(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        ::PostMessage(getNativeHWND(args), WM_SYSCOMMAND, SC_MAXIMIZE, 0);
        return Undefined();
    }
    
    Any NativeWindowProxy::unmaximize(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        
        if (isMaximized(msg)) {
            ::PostMessage(getNativeHWND(args), WM_SYSCOMMAND, SC_RESTORE, 0);
        }
        
        return Undefined();
    }
    
    Any NativeWindowProxy::isMaximized(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        return ::IsZoomed(getNativeHWND(args)) != FALSE;
    }
    
    Any NativeWindowProxy::minimize(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        ::PostMessage(getNativeHWND(args), WM_SYSCOMMAND, SC_MINIMIZE, 0);
        return Undefined();
    }
    
    Any NativeWindowProxy::restore(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        ::PostMessage(getNativeHWND(args), WM_SYSCOMMAND, SC_RESTORE, 0);
        return Undefined();
    }
    
    Any NativeWindowProxy::isMinimized(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        return ::IsIconic(getNativeHWND(args)) != FALSE;
    }
    
    Any NativeWindowProxy::setFullScreen(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    
    Any NativeWindowProxy::isFullScreen(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    
    Any NativeWindowProxy::setWindowRect(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        amo::u8json json(args->getString(0));
        int x = json.getInt("x");
        int y = json.getInt("y");
        int width = json.getInt("width");
        int height = json.getInt("height");
        ::MoveWindow(getNativeHWND(args), x, y, width, height, TRUE);
        return Undefined();
    }
    
    Any NativeWindowProxy::getWindowRect(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        RECT rect = { 0 };
        ::GetWindowRect(getNativeHWND(args), &rect);
        amo::u8json json;
        json.put("x", rect.left);
        json.put("y", rect.top);
        json.put("r", rect.right);
        json.put("b", rect.bottom);
        json.put("width", rect.right - rect.left);
        json.put("height", rect.bottom - rect.top);
        return json;
    }
    
    Any NativeWindowProxy::setSize(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        RECT rect = { 0 };
        ::GetWindowRect(getNativeHWND(args), &rect);
        
        int x = rect.left;
        int y = rect.top;
        int width = args->getInt(0);
        int height = args->getInt(1);
        ::MoveWindow(getNativeHWND(args), x, y, width, height, TRUE);
        return Undefined();
    }
    
    Any NativeWindowProxy::getSize(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        RECT rect = { 0 };
        ::GetWindowRect(getNativeHWND(args), &rect);
        amo::u8json json;
        json.put("width", rect.right - rect.left);
        json.put("height", rect.bottom - rect.top);
        return json;
    }
    
    
    Any NativeWindowProxy::setMinimumSize(IPCMessage::SmartType msg) {
    
        // 由LocalWindow类重载
        return Undefined();
    }
    
    Any NativeWindowProxy::getMinimumSize(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    
    Any NativeWindowProxy::setMaximumSize(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    
    Any NativeWindowProxy::getMaximumSize(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    
    Any NativeWindowProxy::setResizable(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    
    Any NativeWindowProxy::isResizable(IPCMessage::SmartType msg) {
        return  getNativeSettings()->resizable;
    }
    
    Any NativeWindowProxy::setMovable(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        bool moveable = args->getBool(0);
        getNativeSettings()->moveable = moveable;
        return Undefined();
    }
    
    Any NativeWindowProxy::isMovable(IPCMessage::SmartType msg) {
        return  getNativeSettings()->moveable;
    }
    
    Any NativeWindowProxy::setMinimizable(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        bool minimizable = args->getBool(0);
        getNativeSettings()->minimizable = minimizable;
        return Undefined();
    }
    
    Any NativeWindowProxy::isMinimizable(IPCMessage::SmartType msg) {
        return getNativeSettings()->minimizable;
    }
    
    Any NativeWindowProxy::setMaximizable(IPCMessage::SmartType msg) {
        // 由LocalWindow 继承
        return Undefined();
    }
    
    Any NativeWindowProxy::isMaximizable(IPCMessage::SmartType msg) {
        return getNativeSettings()->maximizable;
    }
    
    Any NativeWindowProxy::setFullScreenable(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        bool fullscreenable = args->getBool(0);
        getNativeSettings()->fullscreenable = fullscreenable;
        return Undefined();
    }
    
    Any NativeWindowProxy::isFullScreenable(IPCMessage::SmartType msg) {
        return getNativeSettings()->fullscreenable;
    }
    
    Any NativeWindowProxy::setClosable(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        bool closable = args->getBool(0);
        getNativeSettings()->closable = closable;
        return Undefined();
    }
    
    Any NativeWindowProxy::isClosable(IPCMessage::SmartType msg) {
        return  getNativeSettings()->closable;
    }
    
    Any NativeWindowProxy::setAlwaysOnTop(IPCMessage::SmartType msg) {
        return topmost(msg);
    }
    
    Any NativeWindowProxy::isAlwaysOnTop(IPCMessage::SmartType msg) {
        return isTopmost(msg);
    }
    
    Any NativeWindowProxy::center(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    
    Any NativeWindowProxy::setPosition(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        int x = args->getInt(0);
        int y = args->getInt(1);
        RECT rect = { 0 };
        ::GetWindowRect(getNativeHWND(args), &rect);
        ::MoveWindow(getNativeHWND(args),
                     x,
                     y,
                     rect.right - rect.left,
                     rect.bottom - rect.top,
                     TRUE);
        return Undefined();
    }
    
    Any NativeWindowProxy::getPosition(IPCMessage::SmartType msg) {
        return getWindowRect(msg);
    }
    
    Any NativeWindowProxy::setTitle(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        amo::u8string str(args->getString(0), true);
        
        getNativeSettings()->title = str.to_utf8();
        BOOL bOk = ::SetWindowTextW(getNativeHWND(args), str.to_wide().c_str());
        return bOk != FALSE;
    }
    
    Any NativeWindowProxy::getTitle(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        wchar_t str[256] = { 0 };
        ::GetWindowTextW(getNativeHWND(args), str, 256);
        return amo::u8string(str).to_utf8();
    }
    
    Any NativeWindowProxy::flashFrame(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        bool bFlahFrame = args->getBool(0);
        
        if (bFlahFrame) {
            FLASHWINFO fw;
            fw.cbSize = sizeof(FLASHWINFO);
            fw.hwnd = getNativeHWND(args);
            fw.dwFlags = FLASHW_ALL | FLASHW_TIMER;
            fw.uCount = 5;
            fw.dwTimeout = 500;
            FlashWindowEx(&fw);
        } else {
            FLASHWINFO fw;
            fw.cbSize = sizeof(FLASHWINFO);
            fw.hwnd = getNativeHWND(args);
            fw.dwFlags = FLASHW_STOP;
            FlashWindowEx(&fw);
        }
        
        return Undefined();
    }
    
    
    Any NativeWindowProxy::setHasShadow(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    
    Any NativeWindowProxy::hasShadow(IPCMessage::SmartType msg) {
        return getNativeSettings()->hasShadow;
    }
    
    Any NativeWindowProxy::topmost(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    
    Any NativeWindowProxy::isTopmost(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    
    
    Any NativeWindowProxy::toggleVisible(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    
    Any NativeWindowProxy::toggleFullScreen(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    
    
    
    Any NativeWindowProxy::showModal(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    
    Any NativeWindowProxy::setIcon(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        amo::u8string strPath(args->getString(0), true);
        HICON m_hIcon = (HICON)LoadImage(NULL,
                                         strPath.to_unicode().c_str(),
                                         IMAGE_ICON,
                                         0,
                                         0,
                                         LR_LOADFROMFILE);
        ::SendMessage(getNativeHWND(args),
                      WM_SETICON,
                      (WPARAM)false,
                      (LPARAM)m_hIcon);
        return Undefined();
    }
    
    
    Any NativeWindowProxy::setConfig(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        
        Any& val = args->getValue(0);
        auto appSettings = AppContext::getInstance()->getDefaultAppSettings();
        
        if (val.type() == AnyValueType<amo::u8json>::value) {
            // 更新AppSettings
            std::string strConfig = args->getString(0);
            getNativeSettings()->updateArgsSettings(strConfig);
        }
        
        return Undefined();
    }
    
    Any NativeWindowProxy::getConfig(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        
        Any& val = args->getValue(0);
        auto pSettings = getNativeSettings();
        
        if (val.type() == AnyValueType<Nil>::value) {
            // 返回所有设置
            return pSettings->toJson();
            
        } else  if (val.type() == AnyValueType<std::string>::value) {
            std::string strKey = args->getString(0);
            
            auto json = pSettings->toJson();
            
            if (json.is_bool(strKey)) {
                return json.getBool(strKey);
            } else if (json.is_int(strKey)) {
                return json.getInt(strKey);
            } else if (json.is_string(strKey)) {
                return json.getString(strKey);
            } else if (json.is_double(strKey)) {
                return json.get<double>(strKey);
            } else if (json.is_uint(strKey)) {
                return (int)json.getUint(strKey);
            } else if (json.is_object(strKey)) {
                return json.getJson(strKey);
            } else {
                return Undefined();
            }
            
            // 返回单项设置
        }
        
        return Undefined();
    }
    
    Any NativeWindowProxy::setOpacity(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    
    Any NativeWindowProxy::getOpacity(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return 255;
    }
    
    Any NativeWindowProxy::disableIME(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    
    
    Any NativeWindowProxy::enableIME(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    
    Any NativeWindowProxy::computeCursor(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    Any NativeWindowProxy::isComputeCursor(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    
    Any NativeWindowProxy::setTheme(IPCMessage::SmartType msg) {
        // 由LocalWindow类重载
        return Undefined();
    }
    
    HWND NativeWindowProxy::getNativeHWND(std::shared_ptr<AnyArgsList> args) {
        return getNativeHWND();
    }
    
    
    
    HWND NativeWindowProxy::getNativeHWND() {
        return NULL;
    }
    
    std::shared_ptr<NativeWindowSettings> NativeWindowProxy::getNativeSettings() {
        // 由LocalWindow类重载
        return  std::shared_ptr<NativeWindowSettings>();
    }
    
    
}

