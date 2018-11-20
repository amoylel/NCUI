// Created by amoylel on 06/11/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_LAYEREDWINDOW_H__
#define AMO_LAYEREDWINDOW_H__

#include <windef.h>
#include <winuser.h>
#include <commdlg.h>
#include <functional>
#include <unordered_map>

#include <amo/stdint.hpp>
#include "ShadowWindow.h"


using namespace Gdiplus;


namespace amo {
    class IMM32Manager;
    class NativeWindowSettings;
    class GlobalShortcutSettings;
    class LayeredWindow
        : public WindowImplBase
        
    {
    public:
    
        const static uint32_t CUSTOM_MSG_BEGIN = WM_USER + 10000;
        const static uint32_t CUSTOM_MSG_END = WM_USER + 40000;
        
    public:
        typedef std::function<void(LayeredWindow*)> ClosedCbType;
        
    public:
        LayeredWindow(std::shared_ptr<NativeWindowSettings> pBrowserSettings);
        ~LayeredWindow();
        
        virtual void InitWindow() override;
        virtual void OnFinalMessage(HWND hWnd) override;
        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
        virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL& bHandled) override;
        virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                BOOL& bHandled);
        virtual LRESULT OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                 BOOL& bHandled);
        virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                     BOOL& bHandled) override;
        virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                    BOOL& bHandled) override;
        virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                        BOOL& bHandled) override;
        virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam) override;
        
        void needUpdate();
        
        void setLayered(bool bLayered);
        bool isLayered() const;
        
        uint8_t getTransparent() const;
        void setTransparent(uint8_t val);
        
        void runOnUIThread(amo::function<void()> fn);
        void setFullscreen(bool bFull);
        void setTopmost(bool bTopmost);
        void setIcon(const std::string& strPath);
        
        // 允许拖拽标题栏最大化/还原窗口
        void enableResizeInCaption();
        // 禁止拖拽标题栏最大化/还原窗口
        void disableResizeInCaption();
        
        void resizableInCaption(bool resizable = true);
        
        
        // 注册快捷键
        std::shared_ptr<GlobalShortcutSettings> createSettingByString(
            const std::string& strKey) const;
            
        bool registerHotKey(const int32_t& nID, const uint32_t& nMod,
                            const uint32_t& nKey);
        int32_t registerHotKey(const uint32_t& nMod, const uint32_t& nKey);
        int32_t registerHotKey(const std::string& strKey);
        int32_t registerHotKey(std::shared_ptr<GlobalShortcutSettings> pSettings);
        void unregisterHotKey(const int32_t& iHotkeyID);
        void unregisterHotKey(const std::string& strKey);
        void unregisterAllHotKey();
        bool isRegistered(int32_t nHotKey) const;
        bool isRegistered(const std::string& strKey) const;
        int32_t createHotKey();
        
        std::function<bool(int32_t, amo::u8json)> getHotKeyEventCallback() const;
        void setHotKeyEventCallback(std::function<bool(int32_t, amo::u8json)> val);
        
        void setClosedCallback(ClosedCbType fn);
        
        
        bool ptInWindow();
        bool ptInWindow(POINT& pt);
    protected:
    
        virtual void drawWindow();
        virtual void onUIThreadEvent(uint32_t umsg);
        uint32_t getMessageID();
        bool isDragable(CControlUI* pControl);
    protected:
    
        /*! @brief	BLENDFUNCTION. */
        BLENDFUNCTION m_Blend;
        /*! @brief	分层 透明窗口. */
        bool m_bLayered;
        /*! @brief	The message map. */
        std::unordered_map<uint32_t, amo::function<void()> > m_oMsgMap;
        /*! @brief	The mutex. */
        amo::recursive_mutex  m_mutex;
        /*! @brief	自定义消息索引. */
        uint32_t m_nMsgIndex;
        
        /*! @brief	是否置顶. */
        bool m_isTopmost;
        
        // 全屏相关
        
        /*! @brief	是否在全屏模式. */
        bool m_isFullScreen;
        /*! @brief	正常显示. */
        WINDOWPLACEMENT m_wpNormalScreen;
        /*! @brief	记录窗口显示状态信息. */
        WINDOWPLACEMENT m_wpFullScreen;
        /*! @brief	窗口坐标. */
        RECT m_rectWindow;
        /*! @brief	客户区坐标. */
        RECT m_rectClient;
        /*! @brief	屏幕全屏时窗口位置. */
        RECT m_rectFullScreen;
        /*! @brief	左上角坐标. */
        POINT m_ptUpLeft;
        /*! @brief	右下角坐标. */
        POINT m_ptDownRight;
        /*! @brief	显示器分辨率. */
        int m_nScreenWidth;
        /*! @brief	显示器分辨率. */
        int m_nScreenHeight;
        /*! @brief	记录窗口最小最大信息. */
        MINMAXINFO* pmmi;
        
        /*! @brief	The hot key container. */
        std::set<int32_t> m_oHotKeyContainer;
        
        /*! @brief	The hot key map. */
        std::unordered_map<int32_t, std::shared_ptr<GlobalShortcutSettings> >
        m_oHotKeyMap;
        /*! @brief	全局热键回调. */
        std::function<bool(int32_t, amo::u8json)> m_fnHotKeyEventCallback;
        
        /*! @brief	窗口阴影控件. */
        ShadowWindow m_Shadow;
        
        /*! @brief	浏览器参数. */
        std::shared_ptr<NativeWindowSettings> m_pNativeSettings;
        
        /*! @brief	窗口关闭时回调函数. */
        ClosedCbType m_fnClosedCallback;
        
        std::shared_ptr<IMM32Manager> imm32_manager_;
        
        
    };
}
#endif // AMO_LAYEREDWINDOW_H__

