// Created by amoylel on 16/11/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef LIBNCUI_CLIPBOARDMONITOR_015610D8_823A_4A1B_8E6E_3578E609D109_H__
#define LIBNCUI_CLIPBOARDMONITOR_015610D8_823A_4A1B_8E6E_3578E609D109_H__

#include <amo/singleton.hpp>
#include "ui/win/tray/Tray.h"
#include "ui/win/tray/NotifyWindow.h"

namespace amo {
    class ClipboardMonitor : public amo::singleton<ClipboardMonitor> {
    public:
        ClipboardMonitor();
        
        
        bool startWatch();
        
        bool stopWatch();
        
        bool isWatching();
        
        virtual LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam,
                                      LPARAM lParam);
                                      
        virtual LRESULT OnChangeClipboardChain(HWND hWnd, UINT uMsg, WPARAM wParam,
                                               LPARAM lParam);
        virtual LRESULT OnDrawClipboard(HWND hWnd, UINT uMsg, WPARAM wParam,
                                        LPARAM lParam);
                                        
    private:
        HWND m_hNextClipboard;
        bool m_bWatching;
    };
}



#endif //LIBNCUI_CLIPBOARDMONITOR_015610D8_823A_4A1B_8E6E_3578E609D109_H__