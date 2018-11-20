// Created by amoylel on 06/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_IMM32MANAGER_H__
#define AMO_IMM32MANAGER_H__

#include <windows.h>
#include <string>
#include <amo/string.hpp>

namespace amo {


    class  IMM32Manager {
    public:
        IMM32Manager(HWND hWnd);
        
        virtual ~IMM32Manager();
        
        void destroyImeWindow();
        void cleanupComposition();
        void resetComposition();
        amo::u8string getResult(LPARAM lparam);
        bool getComposition(LPARAM lparam);
        void enableIME();
        void disableIME();
        void cancelIME();
        void updateImeWindow(POINT pt);
        void completeComposition(HIMC imm_context);
        amo::u8string getString(HIMC imm_context, WPARAM lparam);
    private:
        HWND window_handle;
    };
}


#endif // AMO_IMM32MANAGER_H__