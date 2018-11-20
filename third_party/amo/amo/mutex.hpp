
// Created by amoylel on 02/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_MUTEX_C14FD432_E4D0_46B0_970D_8F737FDB784F_HPP__
#define AMO_MUTEX_C14FD432_E4D0_46B0_970D_8F737FDB784F_HPP__

#include <atomic>
#include <string>
#include <mutex>

#if defined(_WIN32) || defined(_WIN64) ||defined(WIN32) || defined(WIN64)
#include <windows.h>
#endif





namespace amo {
    struct null_mutex {
        void lock() {}
        void unlock() {}
        bool try_lock() {
            return true;
        }
    };
    
    struct null_atomic_int {
        int value;
        null_atomic_int() = default;
        
        null_atomic_int(int val) : value(val)
        {}
        
        int load(std::memory_order) const {
            return value;
        }
        
        void store(int val) {
            value = val;
        }
    };
    
    
#if defined(_WIN32) || defined(_WIN64) ||defined(WIN32) || defined(WIN64)
    class process_mutex  {
    public:
    
        process_mutex(const std::string& name) {
            m_bIsLocked = false;
            m_hMutex = ::CreateMutexA(NULL, FALSE, name.c_str());
        }
        
        process_mutex(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner,
                      LPCTSTR lpszName) {
            m_hMutex = CreateMutex(lpMutexAttributes, bInitialOwner, lpszName);
        }
        
        bool try_lock() {
            if (!m_hMutex) {
                return false;
            }
            
            DWORD ret = ::WaitForSingleObject(m_hMutex, 5);
            
            if (ret != WAIT_OBJECT_0) {
                return false;
            } else {
                return true;
            }
        }
        
        
        void set_locked(bool locked) {
        
        
            if (locked) {
                lock();
            } else {
                unlock();
            }
        }
        
        
        
        
        
        ~process_mutex() {
            if (m_hMutex) {
                CloseHandle(m_hMutex);
            }
            
            m_hMutex = NULL;
        }
        
        void lock() {
            if (m_hMutex) {
                ::WaitForSingleObject(m_hMutex, INFINITE);
            }
        }
        
        void unlock() {
            if (m_hMutex) {
                ::ReleaseMutex(m_hMutex);
            }
        }
    private:
        bool m_bIsLocked;
        HANDLE m_hMutex;									// 互斥对象
    };
#endif
}



#endif //AMO_MUTEX_C14FD432_E4D0_46B0_970D_8F737FDB784F_HPP__