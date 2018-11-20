#include "stdafx.h"
#include <commctrl.h>
#include <process.h>

#include "ui/win/tray/TrayBase.h"


namespace amo {

    TrayBase::TrayBase() {
        UINT	uThreadId;
        m_bMouseHover = false;
        m_bTrackMouse = FALSE;
        m_hExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        m_hThread = (HANDLE) _beginthreadex(NULL,
                                            0,
                                            TrayBase::TrackMousePt,
                                            this,
                                            0,
                                            &uThreadId);
        InitializeCriticalSection(&m_cs);
    }
    
    TrayBase::~TrayBase() {
        if (m_hThread != NULL) {
            SetEvent(m_hExitEvent);
            
            if (WaitForSingleObject(m_hThread, 500) == WAIT_TIMEOUT) {
                TerminateThread(m_hThread, 0);
            }
            
            CloseHandle(m_hThread);
            m_hThread = NULL;
        }
        
        if (m_hExitEvent != NULL) {
            CloseHandle(m_hExitEvent);
            m_hExitEvent = NULL;
        }
        
        DeleteCriticalSection(&m_cs);
    }
    
    UINT CALLBACK TrayBase::TrackMousePt(PVOID pvClass) {
        POINT		ptMouse;
        TrayBase	*pTrayPos = (TrayBase *) pvClass;
        
        while (WaitForSingleObject(pTrayPos->m_hExitEvent, 500)
                == WAIT_TIMEOUT) {
                
            if (pTrayPos->m_bTrackMouse == TRUE) {
                GetCursorPos(&ptMouse);
                
                if (!pTrayPos->m_bMouseHover
                        && ptMouse.x == pTrayPos->m_ptMouse.x
                        && ptMouse.y == pTrayPos->m_ptMouse.y
                        && pTrayPos->m_MouseEnterTimer.elapsed() > 300) {
                    pTrayPos->m_bMouseHover = true;
                    pTrayPos->onMouseHover();
                }
                
                if (ptMouse.x != pTrayPos->m_ptMouse.x
                        || ptMouse.y != pTrayPos->m_ptMouse.y) {
                    pTrayPos->m_bTrackMouse = FALSE;
                    pTrayPos->m_bMouseHover = false;
                    pTrayPos->onMouseLeave();
                }
            }
        }
        
        return 0;
    }
    
    void TrayBase::onMouseMove() {
        EnterCriticalSection(&m_cs);
        GetCursorPos(&m_ptMouse);
        
        if (m_bTrackMouse == FALSE) {
            onMouseEnter();
            m_bTrackMouse = TRUE;
            m_MouseEnterTimer.restart();
        }
        
        LeaveCriticalSection(&m_cs);
    }
    
    bool TrayBase::isMouseHover() {
        return m_bTrackMouse;
    }
    
    
}