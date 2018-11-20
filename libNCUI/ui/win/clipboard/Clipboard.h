// Created by amoylel on 08/03/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_CLIPBOARD_H__
#define AMO_CLIPBOARD_H__

#include <string>
#include <amo/stdint.hpp>
#include <amo/format.hpp>

namespace amo {
    inline bool BOOL2bool2(BOOL bVal) {
        return bVal != FALSE;
    }
    
    
    
    /*
    CloseClipboard 关闭剪贴板.
    CountClipboardFormats 统计在当前剪贴板里有多少剪贴板格式. 
    EmptyClipboard 清除剪贴板【必须在OpenClipboard之后调用】. 
    EnumClipboardFormats 枚举当前剪贴板里可用的剪贴板格式. 
    GetClipboardData 取当前剪贴板里德指定剪贴板格式的剪贴板内容. 
    GetClipboardDataAlloc 取当前剪贴板里德指定剪贴板格式的剪贴板内容【当前进程使用，LOCALFREE释放】. 
    GetClipboardFormatName 得到注册的剪贴板格式的名字. 
    GetClipboardOwner 得到拥有剪贴板的窗口. 
    GetOpenClipboardWindow 得到当前打开剪贴板的窗口. 
    GetPriorityClipboardFormat 得到剪贴板列表里的第一个可用的剪贴板格式. 
    IsClipboardFormatAvailable 判断剪贴板里是不是有指定格式的剪贴板数据. 
    OpenClipboard 打开检查剪贴板，这样其他程序就不可以修改剪贴板. 
    RegisterClipboardFormat 注册新的剪贴板格式，返回有效的剪贴板格式.
    SetClipboardData 放置剪贴板数据到指定剪贴板格式的剪贴板上. 
    */
    class UniqueClipboardLock {
    public:
        UniqueClipboardLock(HWND hWnd = NULL) {
            openClipboard();
            
        }
        
        ~UniqueClipboardLock() {
            closeClipboard();
        }
        
    public:
        inline bool emptyClipboard()  {
            return ::EmptyClipboard() != FALSE;
        }
        
        inline bool isEmpty() const {
            return EnumClipboardFormats(0) == 0;
        }
        
        inline bool setClipboardData(_In_ UINT uFormat, _In_opt_ HANDLE hMem) {
            return SetClipboardData(uFormat, hMem) != FALSE;
        }
        
        
        inline void openClipboard()  {
            //打开剪贴板
            if (m_opened) {
                return;
            }
            
            if (!::OpenClipboard(m_hWnd)) {
                std::runtime_error(amo::format("cannot open clipboard {}",
                                               ::GetLastError()));
            }
            
            m_opened = true;
        }
        
        inline void closeClipboard()  {
            ::CloseClipboard();
            m_opened = false;
        }
        
        // 判断格式是否有效
        inline bool isFormatAvailable(uint32_t uCF) {
            return IsClipboardFormatAvailable(uCF) != FALSE;
        }
        
        inline uint32_t enumClipboardFormats(uint32_t uCF = 0) {
            return EnumClipboardFormats(uCF);
        }
        
        
        int32_t dropEffect() {
        
            UINT CF_PREFERREDDROPEFFECT = RegisterClipboardFormat(
                                              CFSTR_PREFERREDDROPEFFECT);
                                              
            if (IsClipboardFormatAvailable(CF_PREFERREDDROPEFFECT)) {
            
                HANDLE hglb = GetClipboardData(CF_PREFERREDDROPEFFECT);
                DWORD dwEffects = *(DWORD*)GlobalLock(hglb);
                
                
                if (dwEffects & DROPEFFECT_MOVE) {
                    return DROPEFFECT_MOVE;
                } else if (dwEffects & DROPEFFECT_COPY) {
                    return DROPEFFECT_COPY;
                }
                
                GlobalUnlock(hglb);
                
            }
            
            return DROPEFFECT_NONE;
        }
        
    private:
        bool m_opened;
        HWND m_hWnd;
    };
    
    class Clipboard {
    public:
        Clipboard(HWND hWnd = NULL);
        ~Clipboard();
        
        // 判断格式是否有效
        bool isFormatAvailable(uint32_t uCF);
        
        uint32_t getFormat(uint32_t uCF = 0);
        
        std::vector<uint32_t> getFormats();
        
        // 当前剪贴板中是否有指定的数据格式
        bool hasFormat(uint32_t uCF);
        
        // 清空剪贴板
        bool empty();
        
        bool isEmpty();
        
        std::string getClipboardData();
        
        std::string getClipboardHTML(uint32_t HTMLformat);
        
        std::string getClipboardFiles();
        
        std::string getClipboardText();
        
        std::string getClipboardBitmap();
        
        HANDLE get_data(UINT uCF) const;
        
        bool getData(UINT uCF, std::string& str);
        
        bool getData(UINT uCF, std::wstring& str);
        
        bool writeText(const std::string& text);
        bool writeText(const std::wstring& text);
        
        bool writeBitmap(HBITMAP hBitmap);
        
        // 向剪贴板中写入文件
        bool writeFiles(const std::vector<std::wstring>& files);
        bool writeFiles(const std::vector<std::string>& u8files);
        
        bool writeHtml(const std::string& u8txt);
        bool writeHtml(const std::wstring& text);
        
        bool writeData(uint32_t uCF, const char* data, int len);
        bool writeData(uint32_t uCF, const wchar_t* data, int len);
        
        bool writeImageBase64(const std::string& str);
        bool writeImageFromFile(const std::wstring& str);
        
        
        std::string readText();
        
        std::wstring readUnicodeText();
        
        HBITMAP readBitmap();
        
        std::string readHtml();
        
        std::vector<std::wstring> readFiles();
        
        bool readImage(std::vector<uint8_t>& vec, const std::wstring fmt = L".bmp");
        
        bool saveImage(const std::wstring& filename);
        
        bool saveAsEMF(const std::wstring& csEMFname);
        
        bool saveAsBitmap(const std::wstring& csBitmapName);
        
        std::string readImageBase64(const std::wstring fmt = L".jpeg");
        
        int32_t dropEffect();
    private:
        Clipboard(const Clipboard& rhs);
        
    private:
    
        // This is based on BitmapToDIB() from DIBUTIL.C of the Microsoft WINCAP sample.
        bool BitmapToClipboard(HBITMAP hBM);
        HANDLE DDBToDIB(HBITMAP bitmap, DWORD dwCompression, HPALETTE pPal);
        
        bool WriteDIB(char * szFile, HANDLE hDIB);
        
        bool SetBackColorToWhite(HDC pDC);
        
        HBITMAP ConvertToBitmap(HENHMETAFILE hemf);
        
    private:
        HWND m_hWnd;
    };
}

#endif // AMO_CLIPBOARD_H__
