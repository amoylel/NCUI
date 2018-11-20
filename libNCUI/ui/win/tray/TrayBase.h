// Created by amoylel on 09/08/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_TRAYBASE_H__
#define AMO_TRAYBASE_H__




#include <amo/timer.hpp>

namespace amo {
    /*!
     * @class	TrayBase
     *
     * @brief	托盘辅助，用于生成几个鼠标消息.
     *     * 来自 http://www.codeguru.com/cpp/com-tech/activex/tutorials/article.php/c8115/How-to-Implement-a-Mouse-HoverLeave-Message-on-the-System-Tray.htm
     */
    class TrayBase {
    public:
        /*! @brief	The point mouse. */
        POINT m_ptMouse;
        /*! @brief	Handle of the thread. */
        HANDLE m_hThread;
        /*! @brief	Handle of the exit event. */
        HANDLE m_hExitEvent;
        /*! @brief	true to track mouse. */
        bool m_bTrackMouse;
        /*! @brief	true to mouse hover. */
        bool m_bMouseHover;
        /*! @brief	The create struct. */
        CRITICAL_SECTION	m_cs;
        /*! @brief	The mouse enter timer. */
        amo::timer m_MouseEnterTimer;
        
    public:
        TrayBase();
        virtual ~TrayBase();
        
        static UINT CALLBACK TrackMousePt(PVOID pvClass);
        void onMouseMove();
        bool isMouseHover();
        
    protected:
        /*!
         * @fn	virtual void TrayBase::onMouseEnter() = 0;
         *
         * @brief	触发鼠标进入事件.
         */
        virtual void onMouseEnter() = 0;
        /*!
         * @fn	virtual void TrayBase::onMouseHover() = 0;
         *
         * @brief	触发鼠标悬停事件.
         */
        virtual void onMouseHover() = 0;
        /*!
         * @fn	virtual void TrayBase::onMouseLeave() = 0;
         *
         * @brief	触发鼠标离开事件.
         */
        virtual void onMouseLeave() = 0;
    };
}



#endif // AMO_TRAYBASE_H__