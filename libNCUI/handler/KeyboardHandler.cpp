#include "stdAfx.h"
#include "handler/KeyboardHandler.h"
#include <amo/logger.hpp>

namespace amo {

    bool KeyboardHandler::OnKeyEvent(CefRefPtr<CefBrowser> browser,
                                     const CefKeyEvent& event,
                                     CefEventHandle os_event) {
        $clog(amo::cdevel << func_orient << amo::endl;);

        bool bRet = OnHotkeyPro(browser, event);

        if (bRet) {
            return bRet;
        }

        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();

        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->OnKeyEvent(browser, event, os_event);

            if (bHandled) return true;
        }

        return CefKeyboardHandler::OnKeyEvent(browser, event, os_event);
    }

    bool KeyboardHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
                                        const CefKeyEvent& event,
                                        CefEventHandle os_event,
                                        bool* is_keyboard_shortcut) {
        $clog(amo::cdevel << func_orient << event.windows_key_code << ":" << event.type << amo::endl;);

        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();

        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->OnPreKeyEvent(browser, event, os_event, is_keyboard_shortcut);

            if (bHandled) return true;
        }

        return CefKeyboardHandler::OnPreKeyEvent(browser, event, os_event, is_keyboard_shortcut);
    }

    bool KeyboardHandler::OnHotkeyPro(CefRefPtr<CefBrowser> browser,
                                      const CefKeyEvent& event) {
        if (KEYEVENT_RAWKEYDOWN != event.type) {
            return false;
        }

        char szBuf[100 + 1] = { 0 };

        bool bRet = false;
        int  iVirtualKey = 0;
        int  iModifiers = 0;
        std::string strTotalHotkey = "";

        BOOL  bCtrl = ::GetKeyState(VK_CONTROL) & 0x8000;

        if (bCtrl) {
            iModifiers |= MOD_CONTROL;
            BuildHotkey(strTotalHotkey, "Ctrl", iVirtualKey);
        }

        BOOL  bShift = ::GetKeyState(VK_SHIFT) & 0x8000;

        if (bShift) {
            iModifiers |= MOD_SHIFT;
            BuildHotkey(strTotalHotkey, "Shift", iVirtualKey);
        }

        BOOL  bAlt = ::GetKeyState(VK_MENU) & 0x8000;

        if (bAlt) {
            iModifiers |= MOD_ALT;
            BuildHotkey(strTotalHotkey, "Alt", iVirtualKey);
        }

        if ((event.windows_key_code >= 0x41 && event.windows_key_code <= 0x5A)
                || (event.windows_key_code >= 0x30 && event.windows_key_code <= 0x39)) {
            _snprintf_s(szBuf, sizeof(szBuf), _TRUNCATE, "%c", event.windows_key_code);
            BuildHotkey(strTotalHotkey, szBuf, iVirtualKey, event.windows_key_code);
        } else {
            switch (event.windows_key_code) {
                case VK_INSERT:
                    BuildHotkey(strTotalHotkey, "Insert", iVirtualKey, event.windows_key_code);
                    break;

                case VK_DELETE:
                    BuildHotkey(strTotalHotkey, "Delete", iVirtualKey, event.windows_key_code);
                    break;

                case VK_HOME:
                    BuildHotkey(strTotalHotkey, "Home", iVirtualKey, event.windows_key_code);
                    break;

                case VK_END:
                    BuildHotkey(strTotalHotkey, "End", iVirtualKey, event.windows_key_code);
                    break;

                case VK_PRIOR:
                    BuildHotkey(strTotalHotkey, "PageUp", iVirtualKey, event.windows_key_code);
                    break;

                case VK_NEXT:
                    BuildHotkey(strTotalHotkey, "PageDown", iVirtualKey, event.windows_key_code);
                    break;

                case VK_UP:
                    BuildHotkey(strTotalHotkey, "Up", iVirtualKey, event.windows_key_code);
                    break;

                case VK_DOWN:
                    BuildHotkey(strTotalHotkey, "Down", iVirtualKey, event.windows_key_code);
                    break;

                case VK_LEFT:
                    BuildHotkey(strTotalHotkey, "Left", iVirtualKey, event.windows_key_code);
                    break;

                case VK_RIGHT:
                    BuildHotkey(strTotalHotkey, "Right", iVirtualKey, event.windows_key_code);
                    break;

                case VK_NUMPAD0:
                case VK_NUMPAD1:
                case VK_NUMPAD2:
                case VK_NUMPAD3:
                case VK_NUMPAD4:
                case VK_NUMPAD5:
                case VK_NUMPAD6:
                case VK_NUMPAD7:
                case VK_NUMPAD8:
                case VK_NUMPAD9:
                    _snprintf_s(szBuf, sizeof(szBuf), _TRUNCATE, "%c", event.windows_key_code - 0x30);
                    BuildHotkey(strTotalHotkey, szBuf, iVirtualKey, event.windows_key_code);
                    break;

                case VK_F1:
                case VK_F2:
                case VK_F3:
                case VK_F4:
                case VK_F5:
                case VK_F6:
                case VK_F7:
                case VK_F8:
                case VK_F9:
                case VK_F10:
                case VK_F11:
                case VK_F12:
                    _snprintf_s(szBuf, sizeof(szBuf), _TRUNCATE, "F%d", event.windows_key_code - 0x70 + 1);
                    BuildHotkey(strTotalHotkey, szBuf, iVirtualKey, event.windows_key_code);
                    break;

                case VK_OEM_MINUS:
                    BuildHotkey(strTotalHotkey, "-", iVirtualKey, event.windows_key_code);
                    break;

                case VK_OEM_PLUS:
                    BuildHotkey(strTotalHotkey, "=", iVirtualKey, event.windows_key_code);
                    break;

                case VK_BACK:
                    BuildHotkey(strTotalHotkey, "Backspace", iVirtualKey, event.windows_key_code);
                    break;

                case VK_OEM_4:
                    BuildHotkey(strTotalHotkey, "[", iVirtualKey, event.windows_key_code);
                    break;

                case VK_OEM_6:
                    BuildHotkey(strTotalHotkey, "]", iVirtualKey, event.windows_key_code);
                    break;

                case VK_OEM_5:
                    BuildHotkey(strTotalHotkey, "\\", iVirtualKey, event.windows_key_code);
                    break;

                case VK_OEM_1:
                    BuildHotkey(strTotalHotkey, ";", iVirtualKey, event.windows_key_code);
                    break;

                case VK_OEM_7:
                    BuildHotkey(strTotalHotkey, "'", iVirtualKey, event.windows_key_code);
                    break;

                case VK_OEM_COMMA:
                    BuildHotkey(strTotalHotkey, ",", iVirtualKey, event.windows_key_code);
                    break;

                case VK_OEM_PERIOD:
                    BuildHotkey(strTotalHotkey, ".", iVirtualKey, event.windows_key_code);
                    break;

                case VK_OEM_2:
                    BuildHotkey(strTotalHotkey, "/", iVirtualKey, event.windows_key_code);
                    break;

                case VK_CONTROL:
                case VK_SHIFT:
                case VK_MENU:

                    break;

                default:

                    return bRet;
            }
        }


        return bRet;
    }

    void KeyboardHandler::BuildHotkey(std::string& strTotalHotkey,
                                      const std::string& strCurHotkey,
                                      int& iVirtualKey,
                                      int iKey /*= 0*/) {
        iVirtualKey = iKey;

        if (strTotalHotkey.empty()) {
            strTotalHotkey.append(strCurHotkey);
            return;
        }

        strTotalHotkey.append(" + ");
        strTotalHotkey.append(strCurHotkey);
    }
}