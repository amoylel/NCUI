#include "stdafx.h"

#include "ui/win/clipboard/Clipboard.h"

#include <vector>
#include <iostream>

#include <windows.h>
#include <shellapi.h>
#include "../Bitmap.hpp"

namespace amo {


    Clipboard::Clipboard(HWND hWnd /*= NULL*/)
        : m_hWnd(hWnd) {
        
    }
    
    Clipboard::~Clipboard() {
    
    }
    
    bool Clipboard::isFormatAvailable(uint32_t uCF) {
        return IsClipboardFormatAvailable(uCF) != FALSE;
    }
    
    std::string Clipboard::getClipboardData() {
        UniqueClipboardLock lock(m_hWnd);
        std::string clipboardData;
        
        UINT HTMLformat = RegisterClipboardFormatA("HTML Format");
        UINT formatList[3] = { HTMLformat, CF_HDROP, CF_TEXT };
        int format = GetPriorityClipboardFormat(formatList, 3);
        
        if (format == HTMLformat) {
            clipboardData = getClipboardHTML(HTMLformat);
        } else if (format == CF_HDROP) {
            clipboardData = getClipboardFiles();
        } else if (format == CF_TEXT) {
            clipboardData = getClipboardText();
        }
        
        return clipboardData;
    }
    
    std::string Clipboard::getClipboardHTML(uint32_t HTMLformat) {
        UniqueClipboardLock lock(m_hWnd);
        std::string clipboardHTML;
        HANDLE hData = GetClipboardData(HTMLformat);
        
        if (hData != nullptr) {
            char *hHTML = static_cast<char *>(GlobalLock(hData));
            
            if (hHTML != nullptr) {
                clipboardHTML.assign(hHTML);
            }
            
            GlobalUnlock(hData);
        }
        
        return clipboardHTML;
    }
    
    
    std::string Clipboard::getClipboardFiles() {
        UniqueClipboardLock lock(m_hWnd);
        std::string clipboardFiles;
        HANDLE hData = ::GetClipboardData(CF_HDROP);
        
        if (hData != nullptr) {
            HDROP hDropData = static_cast<HDROP>(GlobalLock(hData));
            int fileCount = ::DragQueryFileA(hDropData, -1, nullptr, 0);
            
            for (int i = 0; i < fileCount; ++i) {
                clipboardFiles.append(i == 0 ? "" : "\r\n");
                int filenameSize = ::DragQueryFileA(hDropData, i, NULL, 0);
                char *filename = new char[filenameSize + 1];
                ::DragQueryFileA(hDropData, i, filename, filenameSize + 1);
                clipboardFiles.append(filename);
                delete[] filename;
            }
            
            ::GlobalUnlock(hData);
        }
        
        return clipboardFiles;
    }
    
    std::string Clipboard::getClipboardText() {
        UniqueClipboardLock lock(m_hWnd);
        
        std::string clipboardText;
        HANDLE hData = ::GetClipboardData(CF_TEXT);
        
        if (hData != nullptr) {
            char *hText = static_cast<char *>(::GlobalLock(hData));
            
            if (hText != nullptr) {
                clipboardText.assign(hText);
            }
            
            ::GlobalUnlock(hData);
        }
        
        return clipboardText;
    }
    
    
    std::string Clipboard::getClipboardBitmap() {
        return "";
    }
    
    HANDLE Clipboard::get_data(UINT fmt) const {
        UniqueClipboardLock lock(m_hWnd);
        
        
        HANDLE hData = ::GetClipboardData(fmt);
        
        if (NULL == hData &&
                ERROR_SUCCESS != ::GetLastError()) {
            std::runtime_error(amo::format("cannot get clipboard data {}",
                                           ::GetLastError()));
        }
        
        
        return hData;
    }
    
    bool Clipboard::getData(UINT uCF, std::string& str) {
    
        UniqueClipboardLock lock(m_hWnd);
        
        HANDLE hClip = ::GetClipboardData(uCF);
        
        if (hClip != nullptr) {
            char *hText = static_cast<char *>(::GlobalLock(hClip));
            DWORD dwClipSize = GlobalSize(hClip);
            
            if (hText != nullptr) {
                str.assign(hText, dwClipSize);
            }
            
            ::GlobalUnlock(hClip);
            return true;
        }
        
        return false;
        
    }
    
    bool Clipboard::getData(UINT uCF, std::wstring& str) {
        UniqueClipboardLock lock(m_hWnd);
        
        HANDLE hClip = ::GetClipboardData(uCF);
        
        if (hClip != nullptr) {
            wchar_t *hText = static_cast<wchar_t *>(::GlobalLock(hClip));
            DWORD dwClipSize = GlobalSize(hClip);
            
            if (hText != nullptr) {
                str.assign(hText, dwClipSize / 2 - 1);
            }
            
            ::GlobalUnlock(hClip);
            return true;
        }
        
        return false;
        
    }
    
    bool Clipboard::writeText(const std::string& text) {
        return writeData(CF_TEXT, text.c_str(), text.size());
        
    }
    
    bool Clipboard::writeText(const std::wstring& text) {
        return writeData(CF_UNICODETEXT, text.c_str(), text.size());
    }
    
    bool Clipboard::writeBitmap(HBITMAP hBitmap) {
        UniqueClipboardLock lock(m_hWnd);
        lock.emptyClipboard();
        return lock.setClipboardData(CF_BITMAP, hBitmap);
    }
    
    bool Clipboard::writeHtml(const std::string& text) {
        //UTF-8
        BOOL bOk = FALSE;
        
        LPCSTR lpszHtml = text.c_str();
        UINT HtmlLen = text.size();
        CHAR* pszHtmlClipboard = new CHAR[HtmlLen + 512];
        memset(pszHtmlClipboard, 0, HtmlLen + 512);
        ::sprintf(pszHtmlClipboard,
                  "Version:0.9\r\n"
                  "StartHTML:%08u\r\n"
                  "EndHTML:%08u\r\n"
                  "StartFragment:%08u\r\n"
                  "EndFragment:%08u\r\n"
                  "<html><body>\r\n"
                  "<!--StartFragment -->\r\n"
                  "%s\r\n"
                  "<!--EndFragment-->\r\n"
                  "</body></html>",
                  97, 172 + HtmlLen, 111, 136 + HtmlLen, lpszHtml);
        bOk = writeData(RegisterClipboardFormatA("HTML Format"), pszHtmlClipboard,
                        strlen(pszHtmlClipboard));
        delete pszHtmlClipboard;
        return bOk != FALSE;
    }
    
    bool Clipboard::SetHtml(const std::wstring& text) {
        LPCWSTR lpszHtml = text.c_str();
        UINT HtmlLen = text.size();
        BOOL bOk = FALSE;
        int nUtf8Len = WideCharToMultiByte(CP_UTF8, 0, lpszHtml, HtmlLen, NULL, 0, NULL,
                                           NULL);
                                           
        if (nUtf8Len > 0) {
            CHAR* pszUtf8 = new CHAR[nUtf8Len + 1];
            memset(pszUtf8, 0, nUtf8Len + 1);
            WideCharToMultiByte(CP_UTF8, 0, lpszHtml, HtmlLen, pszUtf8, nUtf8Len, NULL,
                                NULL);
            bOk = writeHtml(std::string(pszUtf8, nUtf8Len));
            delete[]pszUtf8;
        }
        
        return bOk != FALSE;
    }
    
    bool Clipboard::writeData(uint32_t uCF, const char* data, int len) {
        UniqueClipboardLock lock(m_hWnd);
        bool bRet = false;
        
        // 清空剪贴板
        if (!lock.emptyClipboard()) {
            return bRet;
        }
        
        //hClip = GlobalAlloc(GMEM_DDESHARE, DataLen);
        HGLOBAL hMemory = ::GlobalAlloc(GMEM_MOVEABLE, len + 1);
        
        if (hMemory) {
            char *hClipboardData = static_cast<char*>(GlobalLock(hMemory));
            memset(hClipboardData, 0, len + 1);
            memcpy(hClipboardData, data, len);
            ::GlobalUnlock(hMemory);
            ::SetClipboardData(uCF, hMemory);
            bRet = true;
        }
        
        ::GlobalFree(hMemory);
        
        return bRet;
    }
    
    bool Clipboard::writeData(uint32_t uCF, const wchar_t* data, int len) {
        UniqueClipboardLock lock(m_hWnd);
        bool bRet = false;
        
        // 清空剪贴板
        if (!lock.emptyClipboard()) {
            return bRet;
        }
        
        HGLOBAL hMemory = ::GlobalAlloc(GMEM_MOVEABLE, len * 2 + 2);
        
        if (hMemory) {
            wchar_t* hClipboardData = static_cast<wchar_t*>(GlobalLock(hMemory));
            wmemset(hClipboardData, 0, len + 1);
            wmemcpy(hClipboardData, data, len);
            ::GlobalUnlock(hMemory);
            ::SetClipboardData(uCF, hMemory);
            bRet = true;
        }
        
        ::GlobalFree(hMemory);
        
        return bRet;
    }
    
    std::string Clipboard::readText() {
        std::string retval;
        getData(CF_TEXT, retval);
        return retval;
    }
    
    
    std::wstring Clipboard::readUnicodeText() {
        std::wstring retval;
        getData(CF_UNICODETEXT, retval);
        return retval;
    }
    
    
    HBITMAP Clipboard::readBitmap() {
        UniqueClipboardLock lock(m_hWnd);
        
        if (0 != EnumClipboardFormats(CF_ENHMETAFILE)) {
        
            HANDLE   hClip = ::GetClipboardData(CF_ENHMETAFILE);
            HENHMETAFILE hEnhMetaFile;
            hEnhMetaFile = (HENHMETAFILE)hClip;
            
            if (GetLastError() != 0) {
                ::GlobalUnlock(hClip);
                return NULL;
            }
            
            HBITMAP hBitmap = ConvertToBitmap(hEnhMetaFile);
            ::GlobalUnlock(hClip);
            return hBitmap;
            
        }
        
        
        if (0 != EnumClipboardFormats(CF_BITMAP)) {
            BOOL   ba = ::IsClipboardFormatAvailable(CF_BITMAP);
            HANDLE   hClip = ::GetClipboardData(CF_BITMAP);
            HBITMAP hBitmap;
            hBitmap = (HBITMAP)hClip;
            
            if (GetLastError() != 0) {
                ::GlobalUnlock(hClip);
                return NULL;
            }
            
            return hBitmap;
        }
        
        return NULL;
    }
    
    
    
    bool Clipboard::saveImage(const std::wstring& filename) {
        std::wstring wext = L".bmp";
        int  index = filename.find_last_of(L".");
        
        if (index != -1) {
            wext = filename.substr(index);
        }
        
        std::string ext = amo::string_utils::wide_to_ansi(wext);
        
        if (wext == L".emf") {
            return saveAsEMF(filename);
        }
        
        std::wstring imageFormat =  amo::GetFormatByExt(wext);
        HBITMAP hBitmap = readBitmap();
        
        if (hBitmap == NULL) {
            return false;
        }
        
        return amo::SaveHBitmapToFile(hBitmap, filename.c_str(),
                                      imageFormat.c_str()) != FALSE;
                                      
    }
    
    bool Clipboard::saveAsEMF(const std::wstring& csEMFname) {
        UniqueClipboardLock lock(m_hWnd);
        
        DWORD   dwError = 0;
        UINT nRet = EnumClipboardFormats(CF_ENHMETAFILE);
        
        std::string retval;
        
        if (nRet) {
            BOOL   ba = ::IsClipboardFormatAvailable(CF_ENHMETAFILE);
            HANDLE   hClip = ::GetClipboardData(CF_ENHMETAFILE);
            HENHMETAFILE hEnhMetaFile;
            hEnhMetaFile = (HENHMETAFILE)hClip;
            dwError = GetLastError();   //dwError=6，无效的句柄。
            
            if (dwError != 0) {
                return false;
            }
            
            HENHMETAFILE   hMetaFile = CopyEnhMetaFileW(hEnhMetaFile,
                                       csEMFname.c_str());//保存到文件
                                       
            ::DeleteEnhMetaFile(hMetaFile);
            ::GlobalUnlock(hClip);
            dwError = GetLastError();   //dwError=6，无效的句柄。
            return dwError == 0;
            
        }
        
        return false;
    }
    
    
    bool Clipboard::saveAsBitmap(const std::wstring& csBitmapName) {
        UniqueClipboardLock lock(m_hWnd);
        HBITMAP hBitmap = readBitmap();
        
        if (hBitmap == NULL) {
            return false;
        }
        
        return  amo::SaveHBitmapToFile(hBitmap, csBitmapName.c_str(),
                                       L"image/bmp") != FALSE;
                                       
    }
    
    
    
    HANDLE Clipboard::DDBToDIB(HBITMAP bitmap, DWORD dwCompression, HPALETTE pPal) {
        BITMAP          bm;
        BITMAPINFOHEADER    bi;
        LPBITMAPINFOHEADER  lpbi;
        DWORD           dwLen;
        HANDLE          hDIB;
        HANDLE          handle;
        HDC             hDC;
        HPALETTE        hPal;
        
        if (dwCompression == BI_BITFIELDS) {
            return NULL;
        }
        
        //如果没有指定调色板，采用默认的
        hPal = (HPALETTE)pPal;
        
        if (hPal == NULL) {
            hPal = (HPALETTE) ::GetStockObject(DEFAULT_PALETTE);
        }
        
        // 获取bitmap信息
        ::GetObject(bitmap, sizeof(bm), (LPSTR)&bm);
        
        //初始化bitmap信息头
        bi.biSize = sizeof(BITMAPINFOHEADER);
        bi.biWidth = bm.bmWidth;
        bi.biHeight = bm.bmHeight;
        bi.biPlanes = 1;
        bi.biBitCount = bm.bmPlanes * bm.bmBitsPixel;
        bi.biCompression = dwCompression;
        bi.biSizeImage = 0;
        bi.biXPelsPerMeter = 0;
        bi.biYPelsPerMeter = 0;
        bi.biClrUsed = 0;
        bi.biClrImportant = 0;
        
        // 计算信息头和调色板大小
        int nColors = (1 << bi.biBitCount);
        
        if (nColors > 256) {
            nColors = 0;
        }
        
        dwLen = bi.biSize + nColors * sizeof(RGBQUAD);
        
        // We need a device context to get the DIB from
        hDC = ::GetDC(NULL);
        hPal = SelectPalette(hDC, hPal, FALSE);
        RealizePalette(hDC);
        
        // 申请内存
        hDIB = GlobalAlloc(GMEM_FIXED, dwLen);
        
        if (!hDIB) {
            SelectPalette(hDC, hPal, FALSE);
            ::ReleaseDC(NULL, hDC);
            return NULL;
        }
        
        lpbi = (LPBITMAPINFOHEADER)hDIB;
        *lpbi = bi;
        
        // Call GetDIBits with a NULL lpBits param, so the device driver
        // will calculate the biSizeImage field
        GetDIBits(hDC, (HBITMAP)bitmap, 0L, (DWORD)bi.biHeight,
                  (LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);
                  
                  
        bi = *lpbi;
        
        // If the driver did not fill in the biSizeImage field, then compute it
        // Each scan line of the image is aligned on a DWORD (32bit) boundary
        if (bi.biSizeImage == 0) {
            bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) *
                             bi.biHeight;
                             
            // If a compression scheme is used the result may infact be larger
            // Increase the size to account for this.
            if (dwCompression != BI_RGB) {
                bi.biSizeImage = (bi.biSizeImage * 3) / 2;
            }
        }
        
        
        // Realloc the buffer so that it can hold all the bits
        dwLen += bi.biSizeImage;
        
        if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE)) {
            hDIB = handle;
        } else {
            GlobalFree(hDIB);
            
            // Reselect the original palette
            SelectPalette(hDC, hPal, FALSE);
            ::ReleaseDC(NULL, hDC);
            return NULL;
        }
        
        // Get the bitmap bits
        lpbi = (LPBITMAPINFOHEADER)hDIB;
        
        // FINALLY get the DIB
        BOOL bGotBits = GetDIBits(hDC, bitmap,
                                  0L,             // Start scan line
                                  (DWORD)bi.biHeight,     // # of scan lines
                                  (LPBYTE)lpbi            // address for bitmap bits
                                  + (bi.biSize + nColors * sizeof(RGBQUAD)),
                                  (LPBITMAPINFO)lpbi,     // address of bitmapinfo
                                  (DWORD)DIB_RGB_COLORS);     // Use RGB for color table
                                  
        if (!bGotBits) {
            GlobalFree(hDIB);
            SelectPalette(hDC, hPal, FALSE);
            ::ReleaseDC(NULL, hDC);
            return NULL;
        }
        
        SelectPalette(hDC, hPal, FALSE);
        ::ReleaseDC(NULL, hDC);
        
        return hDIB;
    }
    
    bool Clipboard::WriteDIB(char * szFile, HANDLE hDIB) {
        BITMAPFILEHEADER    hdr;
        LPBITMAPINFOHEADER  lpbi;
        
        if (!hDIB) {
            return FALSE;
        }
        
        FILE* file;
        file = fopen(szFile, "wb");
        
        if (file == NULL) {
            return FALSE;
        }
        
        lpbi = (LPBITMAPINFOHEADER)hDIB;
        int nColors = 0;
        
        if (lpbi->biBitCount <= 8) {
            nColors = (1 << lpbi->biBitCount);
        }
        
        // Fill in the fields of the file header
        hdr.bfType = ((WORD)('M' << 8) | 'B');  // is always "BM"
        hdr.bfSize = GlobalSize(hDIB) + sizeof(hdr);
        hdr.bfReserved1 = 0;
        hdr.bfReserved2 = 0;
        hdr.bfOffBits = (DWORD)(sizeof(hdr) + lpbi->biSize + nColors * sizeof(RGBQUAD));
        
        // Write the file header
        fwrite(&hdr, sizeof(hdr), 1, file);
        
        
        // Write the DIB header and the bits
        fwrite(lpbi, GlobalSize(hDIB), 1, file);
        
        
        //Close the file and return
        fclose(file);
        
        return TRUE;
    }
    
    bool Clipboard::SetBackColorToWhite(HDC pDC) {
        // Set brush to desired background color
        HBRUSH backBrush = (HBRUSH)::GetStockObject(WHITE_BRUSH);
        // Save old brush
        HBRUSH pOldBrush = (HBRUSH)::SelectObject(pDC, backBrush);
        RECT rect;
        ::GetClipBox(pDC, &rect); // Erase the area needed
        
        //paint the given rectangle using the brush that is currently selected
        //into the specified device context
        ::PatBlt(pDC, rect.left, rect.top, abs(rect.left - rect.right),
                 abs(rect.top - rect.bottom), PATCOPY);
                 
                 
        //Select back the old brush
        ::SelectObject(pDC, pOldBrush);
        
        return TRUE;
    }
    
    bool Clipboard::ConvertToBMP(const char * strFileName, const char* strBMPFile,
                                 BOOL bScaleImage) {
        //Declartions
        HENHMETAFILE hemf;
        HBITMAP     bitmap;
        HDC         memDC;
        ENHMETAHEADER   emh;
        
        //Get the DC of the Window
        HDC dc = ::GetDC(NULL);
        
        //Get the Handle from the enhanced metafile
        hemf = GetEnhMetaFileA(strFileName);
        
        // Get the header from the enhanced metafile.
        ZeroMemory(&emh, sizeof(ENHMETAHEADER));
        emh.nSize = sizeof(ENHMETAHEADER);
        
        if (GetEnhMetaFileHeader(hemf, sizeof(ENHMETAHEADER), &emh) == 0) {
            DeleteEnhMetaFile(hemf);
            return FALSE;
        }
        
        //Declare variables for calculation of metafile rect
        RECT    rect;
        float   PixelsX, PixelsY, MMX, MMY;
        float fAspectRatio;
        long lWidth, lHeight;
        
        // Get the characteristics of the output device.
        PixelsX = (float)GetDeviceCaps(dc, HORZRES);
        PixelsY = (float)GetDeviceCaps(dc, VERTRES);
        MMX = (float)GetDeviceCaps(dc, HORZSIZE);
        MMY = (float)GetDeviceCaps(dc, VERTSIZE);
        
        
        // Calculate the rect in which to draw the metafile based on the
        // intended size and the current output device resolution.
        // Remember that the intended size is given in 0.01 mm units, so
        // convert those to device units on the target device.
        rect.top = (int)((float)(emh.rclFrame.top) * PixelsY / (MMY * 100.0f));
        rect.left = (int)((float)(emh.rclFrame.left) * PixelsX / (MMX * 100.0f));
        rect.right = (int)((float)(emh.rclFrame.right) * PixelsX / (MMX * 100.0f));
        rect.bottom = (int)((float)(emh.rclFrame.bottom) * PixelsY / (MMY * 100.0f));
        
        
        //Calculate the Width and Height of the metafile
        lWidth = (long)((float)(abs(rect.left - rect.right)));
        lHeight = (long)((float)(abs(rect.top - rect.bottom)));
        
        fAspectRatio = (float)lWidth / (float)lHeight;
        
        
        if (bScaleImage) { //If miniature option is ON, change the width and height accordingly
            if (fAspectRatio > 1) { //width is more than height
                //Make width as constant and calculate Height
                // 			lWidth = X_MINIATUREFRAME;
                // 			lHeight = (long)((float)Y_MINIATUREFRAME / fAspectRatio);
            } else { //width is less than height(or equal to height)
                //Make Height as constant and calculate Width
                // 			lHeight = Y_MINIATUREFRAME;
                // 			lWidth = (long)((float)X_MINIATUREFRAME * fAspectRatio);
            }
            
        }
        
        //Populate the rect structure
        rect.left = 0;
        rect.top = 0;
        rect.right = lWidth;
        rect.bottom = lHeight;
        
        //Create a Memory DC compatible to WindowDC
        memDC = ::CreateCompatibleDC(dc);
        
        
        //Create a bitmap compatible to Window DC
        bitmap = ::CreateCompatibleBitmap(dc, lWidth, lHeight);
        
        DWORD dwRetError = GetLastError();
        
        
        //Select the bitmap into the Mem DC
        ::SelectObject(memDC, bitmap);
        
        
        //Paint the background of the DC to White
        SetBackColorToWhite(memDC);
        
        
        //Now play the enhanced metafile into the memory DC; ignore its return value
        //it may be false even if successful
        PlayEnhMetaFile(memDC, hemf, &rect);
        DWORD dwRet = GetLastError();
        
        
        // Create logical palette if device support a palette
        HPALETTE pal;
        
        if (GetDeviceCaps(dc, RASTERCAPS) & RC_PALETTE) {
            UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
            LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
            pLP->palVersion = 0x300;
            pLP->palNumEntries = GetSystemPaletteEntries(dc, 0, 255, pLP->palPalEntry);
            
            // Create the palette
            pal = ::CreatePalette(pLP);
            delete[] pLP;
            
        }
        
        // Convert the bitmap to a DIB
        HANDLE hDIB = DDBToDIB(bitmap, BI_RGB, NULL);
        
        if (hDIB == NULL) {
            DeleteEnhMetaFile(hemf);
            return FALSE;
        }
        
        // Write it to file
        WriteDIB((char*)strBMPFile, hDIB);
        
        // Free the memory allocated by DDBToDIB for the DIB
        ::GlobalFree(hDIB);
        ::DeleteEnhMetaFile(hemf);
        ::ReleaseDC(NULL, dc);
        
        return TRUE;
    }
    
    HBITMAP Clipboard::ConvertToBitmap(HENHMETAFILE hemf) {
        HBITMAP     bitmap = NULL;
        HDC         memDC;
        ENHMETAHEADER   emh;
        
        //Get the DC of the Window
        HDC dc = ::GetDC(NULL);
        
        
        
        // Get the header from the enhanced metafile.
        ZeroMemory(&emh, sizeof(ENHMETAHEADER));
        emh.nSize = sizeof(ENHMETAHEADER);
        
        if (GetEnhMetaFileHeader(hemf, sizeof(ENHMETAHEADER), &emh) == 0) {
            DeleteEnhMetaFile(hemf);
            return FALSE;
        }
        
        //Declare variables for calculation of metafile rect
        RECT    rect;
        float   PixelsX, PixelsY, MMX, MMY;
        float fAspectRatio;
        long lWidth, lHeight;
        
        // Get the characteristics of the output device.
        PixelsX = (float)GetDeviceCaps(dc, HORZRES);
        PixelsY = (float)GetDeviceCaps(dc, VERTRES);
        MMX = (float)GetDeviceCaps(dc, HORZSIZE);
        MMY = (float)GetDeviceCaps(dc, VERTSIZE);
        
        
        // Calculate the rect in which to draw the metafile based on the
        // intended size and the current output device resolution.
        // Remember that the intended size is given in 0.01 mm units, so
        // convert those to device units on the target device.
        rect.top = (int)((float)(emh.rclFrame.top) * PixelsY / (MMY * 100.0f));
        rect.left = (int)((float)(emh.rclFrame.left) * PixelsX / (MMX * 100.0f));
        rect.right = (int)((float)(emh.rclFrame.right) * PixelsX / (MMX * 100.0f));
        rect.bottom = (int)((float)(emh.rclFrame.bottom) * PixelsY / (MMY * 100.0f));
        
        
        //Calculate the Width and Height of the metafile
        lWidth = (long)((float)(abs(rect.left - rect.right)));
        lHeight = (long)((float)(abs(rect.top - rect.bottom)));
        
        fAspectRatio = (float)lWidth / (float)lHeight;
        
        
        
        
        //Populate the rect structure
        rect.left = 0;
        rect.top = 0;
        rect.right = lWidth;
        rect.bottom = lHeight;
        
        //Create a Memory DC compatible to WindowDC
        memDC = ::CreateCompatibleDC(dc);
        
        
        //Create a bitmap compatible to Window DC
        bitmap = ::CreateCompatibleBitmap(dc, lWidth, lHeight);
        
        DWORD dwRetError = GetLastError();
        
        
        //Select the bitmap into the Mem DC
        ::SelectObject(memDC, bitmap);
        
        
        //Paint the background of the DC to White
        SetBackColorToWhite(memDC);
        
        
        //Now play the enhanced metafile into the memory DC; ignore its return value
        //it may be false even if successful
        PlayEnhMetaFile(memDC, hemf, &rect);
        DWORD dwRet = GetLastError();
        
        
        // Create logical palette if device support a palette
        HPALETTE pal;
        
        if (GetDeviceCaps(dc, RASTERCAPS) & RC_PALETTE) {
            UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
            LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
            pLP->palVersion = 0x300;
            pLP->palNumEntries = GetSystemPaletteEntries(dc, 0, 255, pLP->palPalEntry);
            
            // Create the palette
            pal = ::CreatePalette(pLP);
            delete[] pLP;
            
        }
        
        // Convert the bitmap to a DIB
        HANDLE hDIB = DDBToDIB(bitmap, BI_RGB, NULL);
        
        if (hDIB == NULL) {
            DeleteEnhMetaFile(hemf);
            return FALSE;
        }
        
        // Write it to file
        //WriteDIB((char*)strBMPFile, hDIB);
        
        // Free the memory allocated by DDBToDIB for the DIB
        ::GlobalFree(hDIB);
        ::DeleteEnhMetaFile(hemf);
        ::ReleaseDC(NULL, dc);
        return bitmap;
    }
    
}


