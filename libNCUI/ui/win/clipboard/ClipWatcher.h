// Created by amoylel on 09/02/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_CLIPWATCHER_H__
#define AMO_CLIPWATCHER_H__



#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include <shlobj.h>
#include <dbt.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")

// Constants (you shouldn't change)
const LPCWSTR CLIPWATCHER_NAME = L"ClipWatcher";
const LPCWSTR CLIPWATCHER_WNDCLASS = L"ClipWatcherClass";
const LPCWSTR CLIPWATCHER_ORIGIN = L"ClipWatcherOrigin";
const LPCWSTR TASKBAR_CREATED = L"TaskbarCreated";
const WORD BMP_SIGNATURE = 0x4d42; // 'BM' in little endian.
static UINT CF_ORIGIN;
static UINT WM_TASKBAR_CREATED;
enum {
    WM_NOTIFY_ICON = WM_USER + 1,
    WM_NOTIFY_FILE,
};
const LPCWSTR FILE_EXT_TEXT = L".txt";
const LPCWSTR FILE_EXT_BITMAP = L".bmp";
enum {
    FILETYPE_TEXT = 0,
    FILETYPE_BITMAP = 1,
};

// Constants (you may change)
const int CLIPBOARD_RETRY = 3;
const UINT CLIPBOARD_DELAY = 100;
const UINT ICON_BLINK_INTERVAL = 400;
const UINT ICON_BLINK_COUNT = 10;
const UINT FILESYSTEM_INTERVAL = 1000;
const LPCWSTR ERROR_TITLE = L"ClipWatcher Error";
const LPCWSTR ERROR_NOTFOUND = L"Directory does not exist";

// Resources (loaded at runtime)
static HICON HICON_EMPTY;
static HICON HICON_FILETYPE[2];
static WCHAR DEFAULT_CLIPPATH[100] = L"Clipboard";
static WCHAR MESSAGE_WATCHING[100] = L"Watching: %s";
static WCHAR MESSAGE_UPDATED[100] = L"Clipboard Updated";
static WCHAR MESSAGE_BITMAP[100] = L"Bitmap";

// logging
static FILE* logfp = NULL;


//  FileEntry
//
typedef struct _FileEntry {
    WCHAR path[MAX_PATH];
    DWORD hash;
    FILETIME mtime;
    struct _FileEntry* next;
} FileEntry;


class ClipWatcher {
public:
    ClipWatcher();
    int getNumColors(BITMAPINFO* bmp);
    
    size_t getBMPHeaderSize(BITMAPINFO* bmp);
    
    size_t getBMPSize(BITMAPINFO* bmp);
    
    LPWSTR getWCHARfromCHAR(LPCSTR bytes, int nbytes, int* pnchars);
    
    LPSTR getCHARfromWCHAR(LPCWSTR chars, int nchars, int* pnbytes);
    
    // stripspace(text1, text2)
    LPWSTR ristrip(LPCWSTR text1, LPCWSTR text2);
    
    // rmspace(text)
    void rmspace(LPWSTR src);
    
    // rindex(filename)
    int rindex(LPCWSTR text, WCHAR c);
    
    // istartswith(text1, text2)
    int istartswith(LPCWSTR text1, LPCWSTR text2);
    
    // setClipboardOrigin(path)
    
    void setClipboardOrigin(LPCWSTR path);
    
    // setClipboardText(text, nchars)
    void setClipboardText(LPCWSTR text, int nchars);
    
    // setClipboardDIB(bmp)
    void setClipboardDIB(BITMAPINFO* src);
    
    // getClipboardText(buf, buflen)
    int getClipboardText(LPWSTR buf, int buflen);
    
    // writeBytes(path, bytes, nbytes)
    void writeBytes(LPCWSTR path, LPVOID bytes, int nbytes);
    
    // writeTextFile(path, text, nchars)
    void writeTextFile(LPCWSTR path, LPCWSTR text, int nchars);
    
    // readTextFile(path, &nchars)
    LPWSTR readTextFile(LPCWSTR path, int* nchars);
    
    // writeBMPFile(path, bytes, nbytes)
    void writeBMPFile(LPCWSTR path, LPVOID bytes, SIZE_T nbytes);
    
    // readBMPFile(path, &nchars)
    BITMAPINFO* readBMPFile(LPCWSTR path);
    
    // openClipFile()
    BOOL openClipFile();
    
    // exportClipFile(basepath)
    void exportClipFile(LPCWSTR basepath);
    
    // getFileHash(fp)
    DWORD getFileHash(HANDLE fp, DWORD n);
    
public:
    void CreateClipWatcher(
        LPCWSTR dstdir, LPCWSTR srcdir, LPCWSTR name);
        
    //  StartClipWatcher
    //
    void StartClipWatcher();
    
    //  StopClipWatcher
    //
    void StopClipWatcher();
    
    //  DestroyClipWatcher
    //
    void DestroyClipWatcher();
    
public:

    // findFileEntry(files, path)
    FileEntry* findFileEntry(FileEntry* entry, LPCWSTR path);
    
    // freeFileEntries(files)
    void freeFileEntries(FileEntry* entry);
    
    
    
    //  clipWatcherWndProc
    //
    static LRESULT CALLBACK clipWatcherWndProc(
        HWND hWnd,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam);
        
        
        
    // checkFileChanges(watcher)
    FileEntry* checkFileChanges();
    
public:

    LPWSTR m_strDstDir;
    LPWSTR m_strSrcDir;
    HANDLE m_notifierHandler;
    LPWSTR m_strName;
    FileEntry* m_entryFiles;
    DWORD m_nSeqno;
    
    UINT m_nIconID;
    UINT_PTR m_nBlinkTimerID;
    UINT_PTR m_nCheckTimerID;
    HICON m_IconBlinking;
    int m_nIconBlinkCount;
    int m_nShowBalloon;
};



#endif // AMO_CLIPWATCHER_H__

