#include "stdafx.h"
#include "ClipWatcher.h"
#include <shellapi.h>
#include <strsafe.h>


ClipWatcher::ClipWatcher() {
    // Obtain the clipboard directory path.
    WCHAR clipdir[MAX_PATH];
    LPCWSTR clippath = DEFAULT_CLIPPATH;
    
    if (GetFileAttributes(clippath) == INVALID_FILE_ATTRIBUTES) {
        // Obtain the home path.
        WCHAR home[MAX_PATH];
        SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, SHGFP_TYPE_CURRENT, home);
        StringCchPrintf(clipdir, _countof(clipdir), L"%s\\%s",
                        home, clippath);
                        
        CreateDirectory(clippath, NULL);
        
        if (GetFileAttributes(clipdir) == INVALID_FILE_ATTRIBUTES) {
            WCHAR msg[MAX_PATH];
            StringCchPrintf(msg, _countof(msg), L"%s (%s)",
                            ERROR_NOTFOUND, clipdir);
            MessageBox(NULL, msg, ERROR_TITLE, MB_OK | MB_ICONERROR);
            return ;
        }
    } else {
        StringCchCopy(clipdir, _countof(clipdir), clippath);
    }
    
    // Obtain the computer name.
    WCHAR name[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD namelen = sizeof(name);
    GetComputerName(name, &namelen);
    CreateClipWatcher(clipdir, clipdir, name);
}

int ClipWatcher::getNumColors(BITMAPINFO* bmp) {
    int ncolors = bmp->bmiHeader.biClrUsed;
    
    if (ncolors == 0) {
        switch (bmp->bmiHeader.biBitCount) {
        case 1:
            ncolors = 2;
            
        case 8:
            ncolors = 256;
        }
    }
    
    return ncolors;
}

size_t ClipWatcher::getBMPHeaderSize(BITMAPINFO* bmp) {
    return (bmp->bmiHeader.biSize + getNumColors(bmp) * sizeof(RGBQUAD));
}

size_t ClipWatcher::getBMPSize(BITMAPINFO* bmp) {
    return (getBMPHeaderSize(bmp) + bmp->bmiHeader.biSizeImage);
}

LPWSTR ClipWatcher::getWCHARfromCHAR(LPCSTR bytes, int nbytes, int* pnchars) {
    int nchars = MultiByteToWideChar(CP_UTF8, 0, bytes, nbytes, NULL, 0);
    LPWSTR chars = (LPWSTR)malloc(sizeof(WCHAR) * (nchars + 1));
    
    if (chars != NULL) {
        MultiByteToWideChar(CP_UTF8, 0, bytes, nbytes, chars, nchars);
        chars[nchars] = L'\0';
        
        if (pnchars != NULL) {
            *pnchars = nchars;
        }
    }
    
    return chars;
}

LPSTR ClipWatcher::getCHARfromWCHAR(LPCWSTR chars, int nchars, int* pnbytes) {
    int nbytes = WideCharToMultiByte(CP_UTF8, 0, chars, nchars,
                                     NULL, 0, NULL, NULL);
    LPSTR bytes = (LPSTR)malloc(sizeof(CHAR) * (nbytes + 1));
    
    if (bytes != NULL) {
        WideCharToMultiByte(CP_UTF8, 0, chars, nchars,
                            (LPSTR)bytes, nbytes, NULL, NULL);
        bytes[nbytes] = '\0';
        
        if (pnbytes != NULL) {
            *pnbytes = nbytes;
        }
    }
    
    return bytes;
}

LPWSTR ClipWatcher::ristrip(LPCWSTR text1, LPCWSTR text2) {
    int len1 = wcslen(text1);
    int len2 = wcslen(text2);
    int dstlen = len1;
    
    if (len2 < len1 && wcsicmp(&text1[len1 - len2], text2) == 0) {
        dstlen -= len2;
    }
    
    LPWSTR dst = (LPWSTR)malloc(sizeof(WCHAR) * (dstlen + 1));
    
    if (dst != NULL) {
        StringCchCopy(dst, dstlen + 1, text1);
    }
    
    return dst;
}

void ClipWatcher::rmspace(LPWSTR src) {
    WCHAR* dst = src;
    WCHAR c;
    
    while ((c = *(src++)) != 0) {
        if (L' ' < c) {
            *(dst++) = c;
        }
    }
    
    *dst = L'\0';
}

int ClipWatcher::rindex(LPCWSTR text, WCHAR c) {
    int i = wcslen(text) - 1;
    
    while (0 <= i) {
        if (text[i] == c) {
            return i;
        }
        
        i--;
    }
    
    return -1;
}

int ClipWatcher::istartswith(LPCWSTR text1, LPCWSTR text2) {
    while (*text1 != 0 && *text2 != 0 && *text1 == *text2) {
        text1++;
        text2++;
    }
    
    return (*text2 == 0);
}

void ClipWatcher::setClipboardOrigin(LPCWSTR path) {
    int nbytes = sizeof(WCHAR) * (wcslen(path) + 1);
    HANDLE data = GlobalAlloc(GHND, nbytes);
    
    if (data != NULL) {
        BYTE* dst = (BYTE*)GlobalLock(data);
        
        if (dst != NULL) {
            CopyMemory(dst, path, nbytes);
            GlobalUnlock(data);
            SetClipboardData(CF_ORIGIN, data);
            data = NULL;
        }
        
        if (data != NULL) {
            GlobalFree(data);
        }
    }
}

void ClipWatcher::setClipboardText(LPCWSTR text, int nchars) {
    int nbytes = sizeof(WCHAR) * (nchars + 1);
    HANDLE data = GlobalAlloc(GHND, nbytes);
    
    if (data != NULL) {
        BYTE* dst = (BYTE*)GlobalLock(data);
        
        if (dst != NULL) {
            CopyMemory(dst, text, nbytes);
            GlobalUnlock(data);
            SetClipboardData(CF_UNICODETEXT, data);
            data = NULL;
        }
        
        if (data != NULL) {
            GlobalFree(data);
        }
    }
}

void ClipWatcher::setClipboardDIB(BITMAPINFO* src) {
    size_t nbytes = getBMPSize(src);
    HANDLE data = GlobalAlloc(GHND, nbytes);
    
    if (data != NULL) {
        LPVOID dst = GlobalLock(data);
        
        if (dst != NULL) {
            CopyMemory(dst, src, nbytes);
            GlobalUnlock(data);
            SetClipboardData(CF_DIB, data);
            data = NULL;
        }
        
        if (data != NULL) {
            GlobalFree(data);
        }
    }
}

int ClipWatcher::getClipboardText(LPWSTR buf, int buflen) {
    int filetype = -1;
    
    // CF_UNICODETEXT
    HANDLE data = GetClipboardData(CF_UNICODETEXT);
    
    if (data != NULL) {
        LPWSTR text = (LPWSTR)GlobalLock(data);
        
        if (text != NULL) {
            filetype = FILETYPE_TEXT;
            StringCchCopy(buf, buflen, text);
            GlobalUnlock(data);
        }
    }
    
    // CF_DIB
    data = GetClipboardData(CF_DIB);
    
    if (data != NULL) {
        LPVOID bytes = GlobalLock(data);
        
        if (bytes != NULL) {
            SIZE_T nbytes = GlobalSize(data);
            filetype = FILETYPE_BITMAP;
            StringCchCopy(buf, buflen, MESSAGE_BITMAP);
            GlobalUnlock(bytes);
        }
    }
    
    return filetype;
}

void ClipWatcher::writeBytes(LPCWSTR path, LPVOID bytes, int nbytes) {
    HANDLE fp = CreateFile(path, GENERIC_WRITE, 0,
                           NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
                           NULL);
                           
    if (fp != INVALID_HANDLE_VALUE) {
        if (logfp != NULL) {
            fwprintf(logfp, L"write: path=%s, nbytes=%u\n", path, nbytes);
        }
        
        DWORD writtenbytes;
        WriteFile(fp, bytes, nbytes, &writtenbytes, NULL);
        CloseHandle(fp);
    }
}

void ClipWatcher::writeTextFile(LPCWSTR path, LPCWSTR text, int nchars) {
    int nbytes;
    LPSTR bytes = getCHARfromWCHAR(text, nchars, &nbytes);
    
    if (bytes != NULL) {
        writeBytes(path, bytes, nbytes);
        free(bytes);
    }
}

LPWSTR ClipWatcher::readTextFile(LPCWSTR path, int* nchars) {
    const DWORD MAX_FILE_SIZE = 32767;
    
    LPWSTR text = NULL;
    HANDLE fp = CreateFile(path, GENERIC_READ, FILE_SHARE_READ,
                           NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
                           NULL);
                           
    if (fp != INVALID_HANDLE_VALUE) {
        DWORD nbytes = GetFileSize(fp, NULL);
        
        if (MAX_FILE_SIZE < nbytes) {
            nbytes = MAX_FILE_SIZE;
        }
        
        if (logfp != NULL) {
            fwprintf(logfp, L"read: path=%s, nbytes=%u\n", path, nbytes);
        }
        
        LPSTR bytes = (LPSTR)malloc(nbytes);
        
        if (bytes != NULL) {
            DWORD readbytes;
            ReadFile(fp, bytes, nbytes, &readbytes, NULL);
            text = getWCHARfromCHAR(bytes, (int)readbytes, nchars);
            free(bytes);
        }
        
        CloseHandle(fp);
    }
    
    return text;
}

void ClipWatcher::writeBMPFile(LPCWSTR path, LPVOID bytes, SIZE_T nbytes) {
    BITMAPFILEHEADER filehdr = { 0 };
    filehdr.bfType = BMP_SIGNATURE;
    filehdr.bfSize = sizeof(filehdr) + nbytes;
    filehdr.bfOffBits = sizeof(filehdr) + getBMPHeaderSize((BITMAPINFO*)bytes);
    HANDLE fp = CreateFile(path, GENERIC_WRITE, 0,
                           NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
                           NULL);
                           
    if (fp != INVALID_HANDLE_VALUE) {
        if (logfp != NULL) {
            fwprintf(logfp, L"write: path=%s, nbytes=%u\n", path, filehdr.bfSize);
        }
        
        DWORD writtenbytes;
        WriteFile(fp, &filehdr, sizeof(filehdr), &writtenbytes, NULL);
        WriteFile(fp, bytes, nbytes, &writtenbytes, NULL);
        CloseHandle(fp);
    }
}

BITMAPINFO* ClipWatcher::readBMPFile(LPCWSTR path) {
    BITMAPINFO* bmp = NULL;
    HANDLE fp = CreateFile(path, GENERIC_READ, FILE_SHARE_READ,
                           NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
                           NULL);
                           
    if (fp != INVALID_HANDLE_VALUE) {
        DWORD readbytes;
        BITMAPFILEHEADER filehdr;
        ReadFile(fp, &filehdr, sizeof(filehdr), &readbytes, NULL);
        
        if (filehdr.bfType == BMP_SIGNATURE) {
            DWORD bmpsize = filehdr.bfSize - sizeof(filehdr);
            bmp = (BITMAPINFO*)malloc(bmpsize);
            
            if (bmp != NULL) {
                ReadFile(fp, bmp, bmpsize, &readbytes, NULL);
            }
        }
        
        CloseHandle(fp);
    }
    
    return bmp;
}

BOOL ClipWatcher::openClipFile() {
    const LPCWSTR OP_OPEN = L"open";
    
    BOOL success = FALSE;
    
    // Try opening CF_UNICODETEXT.
    HANDLE data = GetClipboardData(CF_UNICODETEXT);
    
    if (data != NULL) {
        LPWSTR text = (LPWSTR)GlobalLock(data);
        
        if (text != NULL) {
            text = _wcsdup(text);
            
            if (text != NULL) {
                rmspace(text);
                
                if (istartswith(text, L"http://") ||
                        istartswith(text, L"https://")) {
                    if (logfp != NULL) {
                        fwprintf(logfp, L"open: url=%s\n", text);
                    }
                    
                    ShellExecute(NULL, OP_OPEN, text, NULL, NULL, SW_SHOWDEFAULT);
                    success = TRUE;
                }
                
                free(text);
            }
            
            GlobalUnlock(data);
        }
    }
    
    if (success) {
        return success;
    }
    
    // Try opening CF_ORIGIN.
    data = GetClipboardData(CF_ORIGIN);
    
    if (data != NULL) {
        LPWSTR path = (LPWSTR)GlobalLock(data);
        
        if (path != NULL) {
            if (logfp != NULL) {
                fwprintf(logfp, L"open: path=%s\n", path);
            }
            
            ShellExecute(NULL, OP_OPEN, path, NULL, NULL, SW_SHOWDEFAULT);
            success = TRUE;
            GlobalUnlock(data);
        }
    }
    
    if (success) {
        return success;
    }
    
    return FALSE;
}

void ClipWatcher::exportClipFile(LPCWSTR basepath) {
    // CF_UNICODETEXT
    HANDLE data = GetClipboardData(CF_UNICODETEXT);
    
    if (data != NULL) {
        LPWSTR text = (LPWSTR)GlobalLock(data);
        
        if (text != NULL) {
            WCHAR path[MAX_PATH];
            StringCchPrintf(path, _countof(path), L"%s.txt", basepath);
            setClipboardOrigin(path);
            writeTextFile(path, text, wcslen(text));
            GlobalUnlock(data);
        }
    }
    
    // CF_DIB
    data = GetClipboardData(CF_DIB);
    
    if (data != NULL) {
        LPVOID bytes = GlobalLock(data);
        
        if (bytes != NULL) {
            SIZE_T nbytes = GlobalSize(data);
            WCHAR path[MAX_PATH];
            StringCchPrintf(path, _countof(path), L"%s.bmp", basepath);
            setClipboardOrigin(path);
            writeBMPFile(path, bytes, nbytes);
            GlobalUnlock(bytes);
        }
    }
}

DWORD ClipWatcher::getFileHash(HANDLE fp, DWORD n) {
    DWORD hash = 0;
    DWORD bufsize = sizeof(DWORD) * n;
    DWORD* buf = (DWORD*)malloc(bufsize);
    
    if (buf != NULL) {
        DWORD readbytes;
        ZeroMemory(buf, bufsize);
        ReadFile(fp, buf, bufsize, &readbytes, NULL);
        
        for (int i = 0; i < n; i++) {
            hash ^= buf[i];
        }
        
        free(buf);
    }
    
    return hash;
}

void ClipWatcher::CreateClipWatcher(LPCWSTR dstdir, LPCWSTR srcdir,
                                    LPCWSTR name) {
                                    
    HINSTANCE hInstance = GetModuleHandle(NULL);
    /*  HICON_EMPTY =
          LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIPEMPTY));
      HICON_FILETYPE[FILETYPE_TEXT] =
          LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIPTEXT));
      HICON_FILETYPE[FILETYPE_BITMAP] =
          LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIPBITMAP));*/
    
    // Register the clipboard format.
    CF_ORIGIN = RegisterClipboardFormat(CLIPWATCHER_ORIGIN);
    // Register the window message.
    WM_TASKBAR_CREATED = RegisterWindowMessage(TASKBAR_CREATED);
    
    
    this->m_strDstDir = wcsdup(dstdir);
    this->m_strSrcDir = wcsdup(srcdir);
    this->m_notifierHandler = INVALID_HANDLE_VALUE;
    this->m_strName = wcsdup(name);
    this->m_entryFiles = NULL;
    this->m_nSeqno = 0;
    
    this->m_nIconID = 1;
    this->m_nBlinkTimerID = 1;
    this->m_nCheckTimerID = 2;
    this->m_IconBlinking = NULL;
    this->m_nIconBlinkCount = 0;
    this->m_nShowBalloon = 0;
}

void ClipWatcher::StartClipWatcher() {
    if (this->m_notifierHandler == INVALID_HANDLE_VALUE) {
        this->m_notifierHandler = FindFirstChangeNotification(
                                      this->m_strSrcDir, FALSE,
                                      (FILE_NOTIFY_CHANGE_FILE_NAME |
                                       FILE_NOTIFY_CHANGE_SIZE |
                                       FILE_NOTIFY_CHANGE_ATTRIBUTES |
                                       FILE_NOTIFY_CHANGE_LAST_WRITE));
                                       
        if (logfp != NULL) {
            fwprintf(logfp, L"register: srcdir=%s, notifier=%p\n",
                     this->m_strSrcDir, this->m_notifierHandler);
        }
    }
}

void ClipWatcher::StopClipWatcher() {
    if (this->m_notifierHandler != INVALID_HANDLE_VALUE) {
        FindCloseChangeNotification(this->m_notifierHandler);
        this->m_notifierHandler = INVALID_HANDLE_VALUE;
    }
}

void ClipWatcher::DestroyClipWatcher() {
    if (this->m_strSrcDir != NULL) {
        free(this->m_strSrcDir);
    }
    
    if (this->m_strDstDir != NULL) {
        free(this->m_strDstDir);
    }
    
    if (this->m_strName != NULL) {
        free(this->m_strName);
    }
    
    freeFileEntries(this->m_entryFiles);
    
    free(this);
}

FileEntry* ClipWatcher::findFileEntry(FileEntry* entry, LPCWSTR path) {
    while (entry != NULL) {
        if (wcsicmp(entry->path, path) == 0) {
            return entry;
        }
        
        entry = entry->next;
    }
    
    return entry;
}

void ClipWatcher::freeFileEntries(FileEntry* entry) {
    while (entry != NULL) {
        void* p = entry;
        entry = entry->next;
        free(p);
    }
}

LRESULT CALLBACK ClipWatcher::clipWatcherWndProc(HWND hWnd, UINT uMsg,
        WPARAM wParam, LPARAM lParam) {
    //fwprintf(stderr, L"msg: %x, hWnd=%p, wParam=%p\n", uMsg, hWnd, wParam);
    
    switch (uMsg) {
    case WM_CREATE: {
        // Initialization.
        CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
        ClipWatcher* watcher = (ClipWatcher*)(cs->lpCreateParams);
        
        if (watcher != NULL) {
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)watcher);
            
            if (logfp != NULL) {
                fwprintf(logfp, L"watcher: %s\n", watcher->m_strName);
            }
            
            // Start watching the clipboard content.
            AddClipboardFormatListener(hWnd);
            SetTimer(hWnd, watcher->m_nBlinkTimerID, ICON_BLINK_INTERVAL, NULL);
            SetTimer(hWnd, watcher->m_nCheckTimerID, FILESYSTEM_INTERVAL, NULL);
            SendMessage(hWnd, WM_TASKBAR_CREATED, 0, 0);
        }
        
        return FALSE;
    }
    
    case WM_DESTROY: {
        // Clean up.
        LONG_PTR lp = GetWindowLongPtr(hWnd, GWLP_USERDATA);
        ClipWatcher* watcher = (ClipWatcher*)lp;
        
        if (watcher != NULL) {
            KillTimer(hWnd, watcher->m_nBlinkTimerID);
            KillTimer(hWnd, watcher->m_nCheckTimerID);
            // Stop watching the clipboard content.
            RemoveClipboardFormatListener(hWnd);
            // Unregister the icon.
            NOTIFYICONDATA nidata = { 0 };
            nidata.cbSize = sizeof(nidata);
            nidata.hWnd = hWnd;
            nidata.uID = watcher->m_nIconID;
            Shell_NotifyIcon(NIM_DELETE, &nidata);
        }
        
        PostQuitMessage(0);
        return FALSE;
    }
    
    case WM_CLIPBOARDUPDATE: {
        // Clipboard change detected.
        LONG_PTR lp = GetWindowLongPtr(hWnd, GWLP_USERDATA);
        ClipWatcher* watcher = (ClipWatcher*)lp;
        
        if (watcher != NULL) {
            DWORD seqno = GetClipboardSequenceNumber();
            
            if (watcher->m_nSeqno < seqno) {
                watcher->m_nSeqno = seqno;
                
                if (logfp != NULL) {
                    fwprintf(logfp, L"updated clipboard: seqno=%d\n", seqno);
                }
                
                for (int i = 0; i < CLIPBOARD_RETRY; i++) {
                    Sleep(CLIPBOARD_DELAY);
                    
                    if (OpenClipboard(hWnd)) {
                        if (GetClipboardData(CF_ORIGIN) == NULL) {
                            WCHAR path[MAX_PATH];
                            StringCchPrintf(path, _countof(path), L"%s\\%s",
                                            watcher->m_strDstDir, watcher->m_strName);
                            watcher->exportClipFile(path);
                        }
                        
                        WCHAR text[256];
                        int filetype = watcher->getClipboardText(text, _countof(text));
                        
                        if (0 <= filetype) {
                            if (watcher->m_nShowBalloon) {
                                NOTIFYICONDATA nidata = { 0 };
                                nidata.cbSize = sizeof(nidata);
                                nidata.hWnd = hWnd;
                                nidata.uID = watcher->m_nIconID;
                                nidata.uFlags = NIF_INFO;
                                nidata.dwInfoFlags = NIIF_INFO;
                                nidata.uTimeout = 1000;
                                StringCchCopy(nidata.szInfoTitle,
                                              _countof(nidata.szInfoTitle),
                                              MESSAGE_UPDATED);
                                StringCchCopy(nidata.szInfo,
                                              _countof(nidata.szInfo),
                                              text);
                                Shell_NotifyIcon(NIM_MODIFY, &nidata);
                            }
                            
                            watcher->m_IconBlinking = HICON_FILETYPE[filetype];
                            watcher->m_nIconBlinkCount = ICON_BLINK_COUNT;
                        }
                        
                        CloseClipboard();
                        break;
                    }
                }
            }
        }
        
        return FALSE;
    }
    
    case WM_NOTIFY_FILE: {
        // File change detected.
        LONG_PTR lp = GetWindowLongPtr(hWnd, GWLP_USERDATA);
        ClipWatcher* watcher = (ClipWatcher*)lp;
        
        if (watcher != NULL) {
            FileEntry* entry = watcher->checkFileChanges();
            
            if (entry != NULL) {
                if (logfp != NULL) {
                    fwprintf(logfp, L"updated file: path=%s\n", entry->path);
                }
                
                int index = watcher->rindex(entry->path, L'.');
                
                if (0 <= index) {
                    WCHAR* ext = &(entry->path[index]);
                    
                    if (_wcsicmp(ext, FILE_EXT_TEXT) == 0) {
                        // CF_UNICODETEXT
                        int nchars;
                        LPWSTR text = watcher->readTextFile(entry->path, &nchars);
                        
                        if (text != NULL) {
                            if (OpenClipboard(hWnd)) {
                                EmptyClipboard();
                                watcher->setClipboardOrigin(entry->path);
                                watcher->setClipboardText(text, nchars);
                                CloseClipboard();
                            }
                            
                            free(text);
                        }
                    } else if (_wcsicmp(ext, FILE_EXT_BITMAP) == 0) {
                        // CF_DIB
                        BITMAPINFO* bmp = watcher->readBMPFile(entry->path);
                        
                        if (bmp != NULL) {
                            if (OpenClipboard(hWnd)) {
                                EmptyClipboard();
                                watcher->setClipboardOrigin(entry->path);
                                watcher->setClipboardDIB(bmp);
                                CloseClipboard();
                            }
                            
                            free(bmp);
                        }
                    }
                }
            }
        }
        
        return FALSE;
    }
    
    case WM_COMMAND: {
        // Command specified.
        LONG_PTR lp = GetWindowLongPtr(hWnd, GWLP_USERDATA);
        ClipWatcher* watcher = (ClipWatcher*)lp;
        
        switch (LOWORD(wParam)) {
        case 102:
            if (watcher != NULL) {
                if (OpenClipboard(hWnd)) {
                    watcher->openClipFile();
                    CloseClipboard();
                }
            }
            
            break;
            
        case 105:
            SendMessage(hWnd, WM_CLOSE, 0, 0);
            break;
        }
        
        return FALSE;
    }
    
    case WM_TIMECHANGE: {
        // Filesytem/Network share change detected.
        // NOTICE: We wanted to check if wParam is DBT_DEVICEARRIVAL.
        //   But it doesn't work when the system is suspended.
        LONG_PTR lp = GetWindowLongPtr(hWnd, GWLP_USERDATA);
        ClipWatcher* watcher = (ClipWatcher*)lp;
        
        if (watcher != NULL) {
            // Re-initialize the watcher object.
            watcher->StopClipWatcher();
        }
        
        return TRUE;
    }
    
    case WM_NOTIFY_ICON: {
        // UI event handling.
        POINT pt;
        HMENU menu = GetMenu(hWnd);
        
        if (menu != NULL) {
            menu = GetSubMenu(menu, 0);
        }
        
        switch (lParam) {
        case WM_LBUTTONDBLCLK:
            if (menu != NULL) {
                UINT item = GetMenuDefaultItem(menu, FALSE, 0);
                SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(item, 1), NULL);
            }
            
            break;
            
        case WM_LBUTTONUP:
            break;
            
        case WM_RBUTTONUP:
            if (GetCursorPos(&pt)) {
                SetForegroundWindow(hWnd);
                
                if (menu != NULL) {
                    TrackPopupMenu(menu, TPM_LEFTALIGN,
                                   pt.x, pt.y, 0, hWnd, NULL);
                }
                
                PostMessage(hWnd, WM_NULL, 0, 0);
            }
            
            break;
        }
        
        return FALSE;
    }
    
    case WM_TIMER: {
        LONG_PTR lp = GetWindowLongPtr(hWnd, GWLP_USERDATA);
        ClipWatcher* watcher = (ClipWatcher*)lp;
        
        if (watcher != NULL) {
            UINT_PTR timer_id = wParam;
            
            if (timer_id == watcher->m_nBlinkTimerID) {
                // Blink the icon.
                if (watcher->m_nIconBlinkCount) {
                    watcher->m_nIconBlinkCount--;
                    BOOL on = (watcher->m_nIconBlinkCount % 2);
                    NOTIFYICONDATA nidata = { 0 };
                    nidata.cbSize = sizeof(nidata);
                    nidata.hWnd = hWnd;
                    nidata.uID = watcher->m_nIconID;
                    nidata.uFlags = NIF_ICON;
                    nidata.hIcon = (on ? watcher->m_IconBlinking : HICON_EMPTY);
                    Shell_NotifyIcon(NIM_MODIFY, &nidata);
                }
            } else if (timer_id == watcher->m_nCheckTimerID) {
                // Check the filesystem.
                watcher->StartClipWatcher();
            }
        }
        
        return FALSE;
    }
    
    case WM_CLOSE:
        DestroyWindow(hWnd);
        return FALSE;
        
    default:
        if (uMsg == WM_TASKBAR_CREATED) {
            LONG_PTR lp = GetWindowLongPtr(hWnd, GWLP_USERDATA);
            ClipWatcher* watcher = (ClipWatcher*)lp;
            
            if (watcher != NULL) {
                // Register the icon.
                NOTIFYICONDATA nidata = { 0 };
                nidata.cbSize = sizeof(nidata);
                nidata.hWnd = hWnd;
                nidata.uID = watcher->m_nIconID;
                nidata.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
                nidata.uCallbackMessage = WM_NOTIFY_ICON;
                nidata.hIcon = HICON_EMPTY;
                StringCchPrintf(nidata.szTip, _countof(nidata.szTip),
                                MESSAGE_WATCHING, watcher->m_strSrcDir);
                Shell_NotifyIcon(NIM_ADD, &nidata);
            }
        }
        
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

FileEntry* ClipWatcher::checkFileChanges() {
    WCHAR dirpath[MAX_PATH];
    StringCchPrintf(dirpath, _countof(dirpath), L"%s\\*.*", m_strSrcDir);
    
    WIN32_FIND_DATA data;
    FileEntry* found = NULL;
    
    HANDLE fft = FindFirstFile(dirpath, &data);
    
    if (fft == NULL) {
        goto fail;
    }
    
    for (;;) {
        LPWSTR name = data.cFileName;
        int index = rindex(name, L'.');
        
        if (0 <= index && wcsnicmp(name, name, index) != 0) {
            WCHAR path[MAX_PATH];
            StringCchPrintf(path, _countof(path), L"%s\\%s",
                            m_strSrcDir, name);
            HANDLE fp = CreateFile(path, GENERIC_READ, FILE_SHARE_READ,
                                   NULL, OPEN_EXISTING,
                                   (FILE_ATTRIBUTE_NORMAL |
                                    FILE_FLAG_NO_BUFFERING),
                                   NULL);
                                   
            if (fp != INVALID_HANDLE_VALUE) {
                DWORD hash = getFileHash(fp, 256);
                FILETIME mtime;
                GetFileTime(fp, NULL, NULL, &mtime);
                
                if (logfp != NULL) {
                    fwprintf(logfp, L"check: name=%s (%08x, %08x)\n",
                             name, hash, mtime.dwLowDateTime);
                }
                
                FileEntry* entry = findFileEntry(m_entryFiles, path);
                
                if (entry == NULL) {
                    if (logfp != NULL) {
                        fwprintf(logfp, L"added: name=%s\n", name);
                    }
                    
                    entry = (FileEntry*)malloc(sizeof(FileEntry));
                    StringCchCopy(entry->path, _countof(entry->path), path);
                    entry->hash = hash;
                    entry->mtime = mtime;
                    entry->next = m_entryFiles;
                    m_entryFiles = entry;
                    found = entry;
                } else if (hash != entry->hash ||
                           CompareFileTime(&mtime, &(entry->mtime)) != 0) {
                    if (logfp != NULL) {
                        fwprintf(logfp, L"updated: name=%s\n", name);
                    }
                    
                    entry->hash = hash;
                    entry->mtime = mtime;
                    found = entry;
                }
                
                CloseHandle(fp);
            }
        }
        
        if (!FindNextFile(fft, &data)) {
            break;
        }
    }
    
    FindClose(fft);
    
fail:
    return found;
}
