// Created by amoylel on 08/01/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_TRAY_H__
#define AMO_TRAY_H__


#include <amo/timer.hpp>
#include <shellapi.h>
#include "ui/win/tray/TrayBase.h"


namespace amo {

    class NotifyWindow;
    
    class Tray : public amo::singleton<Tray>
        , public TrayBase {
    public:
    
        Tray();
        ~Tray();
        
        virtual void onCreateSingleInstance() override;
        
        void createNotifyWindow();
        
        void create();
        
        void destory();
        
        void close();
        
        void blink(bool bBlink = true);
        
        void updateIcon(HICON icon);
        
        
        LRESULT OnImeMonitor(UINT uMsg, WPARAM wParam, LPARAM lParam);
        /*!
         * @fn	void Tray::show(bool bShow);
         *
         * @brief	显示或隐藏托盘.
         *
         * @param	bShow	true to show, false to hide.
         */
        void show(bool bShow);
        /*!
         * @fn	void Tray::setToolTip(const std::string& tipMsg);
         *
         * @brief	设置托盘提示文本.
         *
         * @param	tipMsg	Message describing the tip.
         */
        void setToolTip(const std::wstring& tipMsg);
        
        /*!
         * @fn	void Tray::setBlink(bool isBlink);
         *
         * @brief	设置托盘是否闪烁.
         *
         * @param	isBlink	true if this object is blink.
         */
        void setBlink(bool isBlink);
        /*!
         * @fn	bool Tray::isBlink() const;
         *
         * @brief	判断托盘是否正在闪烁.
         *
         * @return	true if blink, false if not.
         */
        bool isBlink() const;
        /*!
         * @fn	HICON Tray::getTrayIcon() const;
         *
         * @brief	获取托盘图标.
         *
         * @return	The tray icon.
         */
        HICON getTrayIcon() const;
        /*!
         * @fn	void Tray::setTrayIcon(HICON val);
         *
         * @brief	设置托盘图标.
         *
         * @param	val	The value.
         */
        void setTrayIcon(HICON val);
        /*!
         * @fn	void Tray::setTrayIcon(const amo::u8string& strPath);
         *
         * @brief	设置托盘图标.
         *
         * @param	strPath	Full pathname of the file.
         */
        void setTrayIcon(const amo::u8string& strPath);
        
        /*!
         * @fn	NOTIFYICONDATAW& Tray::getNotifyCondata();
         *
         * @brief	托盘数据结构.
         *
         * @return	The notify condata.
         */
        NOTIFYICONDATAW& getNotifyCondata();
        /*!
         * @fn	void Tray::setNotifyCondata(NOTIFYICONDATAW val);
         *
         * @brief	设置托盘数据.
         *
         * @param	val	The value.
         */
        void setNotifyCondata(NOTIFYICONDATAW val);
        
        /*!
         * @fn	std::function<void(const std::string&)> getEventCallback() const;
         *
         * @brief	获取托盘事件回调函数.
         *
         * @return	The event callback.
         */
        std::function<void(const std::string&)> getEventCallback() const;
        /*!
         * @fn	void Tray::setEventCallback(std::function<void(const std::string&)> fn);
         *
         * @brief	设置托盘事件回调函数.
         *
         * @param	fn	The function.
         */
        void setEventCallback(std::function<void(const std::string&)> fn);
        
        /*!
         * @fn	NotifyWindow* Tray::getNotifyWindow();
         *
         * @brief	获取通知窗口，现只作消息窗口使用.
         *
         * @return	null if it fails, else the notify window.
         */
        NotifyWindow* getNotifyWindow();
        
    private:
        /*!
         * @fn	void Tray::triggerEvent(const std::string& event);
         *
         * @brief	触发托盘事件.
         *
         * @param	event	The event.
         */
        void triggerEvent(const std::string& event);
        
        
        
    private:
    
        /*! @brief	托盘数据. */
        NOTIFYICONDATA m_notifyCondata;
        /*! @brief	消息循环句柄. */
        HWND m_hMessageWnd;
        /*托盘图标*/
        HICON m_hTrayIcon;
        /*! @brief	托盘消息框. */
        NotifyWindow* m_pNotifyWindow;
        /*! @brief	托盘闪烁定时器. */
        UINT_PTR m_flashTimer;
        /*! @brief	是否闪烁托盘，默认为false. */
        bool m_bBlink;
        /*! @brief	托盘消息. */
        UINT WM_TRAY_MONITOR;
        
        /*! @brief	双击计时器. */
        std::shared_ptr<amo::timer> m_timer;
        
        /*! @brief	托盘事件回调函数. */
        std::function<void(const std::string&)> m_fnEventCallback;
        
    public:
    
    
    
    protected:
        virtual void onMouseEnter() override;
        virtual void onMouseHover() override;
        virtual void onMouseLeave() override;
        
    };
    
}
#endif // AMO_TRAY_H__