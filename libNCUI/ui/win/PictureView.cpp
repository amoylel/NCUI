#include "stdAfx.h"

#include "ui/win/PictureView.h"

namespace amo {

    void PictureView::setPicture(const std::wstring& file) {
    
    
    
        m_pBitmap.reset(new Bitmap(file.c_str()));
    }
    
    
    
    DuiLib::CDuiRect PictureView::getDisplayRect(const CDuiRect& windowRect, const CDuiRect& textureRect) {
        if (!m_pBitmap) {
            return GetPos();
        }
        
        int window_width = windowRect.GetWidth() * 1000;
        int window_height = windowRect.GetHeight() * 1000;
        int texture_width = textureRect.GetWidth();
        int texture_height = textureRect.GetHeight();
        
        double width_rate = window_width / (double)texture_width;
        double height_rate = window_height / (double)texture_height;
        bool width_first = true;
        
        if (width_rate > height_rate) {
            width_first = false;
        }
        
        //X轴占满屏幕
        double rate = (double)window_width / texture_width;
        int width = window_width;
        int height = static_cast<int>(texture_height * rate);
        int left = 0;
        int top = window_height / 2 - height / 2;
        top /= 1000;
        
        if (!width_first) {	//X轴占满屏幕
            double rate = window_height / texture_height;
            height = window_height;
            width = static_cast<int>(texture_width * rate);
            top = 0;
            left = window_width / 2 - width / 2;
            left /= 1000;
        }
        
        width += 500;
        height += 500;
        width /= 1000;
        height /= 1000;
        
        //居中
        int x = left + windowRect.left;
        int y = top + windowRect.top;
        int x2 = x + width;
        int y2 = y + height;
        return CDuiRect(x, y, x2, y2);
    }
    
    PictureView::PictureView() : m_gdiplusToken(0) {
        ::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
        m_pBkImage.reset(new Bitmap(100, 100, PixelFormat32bppARGB));
        Graphics g(&*m_pBkImage);
        Gdiplus::Color   myColor(100, 0, 0, 0);
        g.Clear(myColor);
    }
    
    PictureView::~PictureView() {
    
        m_pBitmap.reset();
        m_pBkImage.reset();
        ::GdiplusShutdown(m_gdiplusToken);
    }
    
    LPCTSTR PictureView::GetClass() const {
        return _T("PictureView");
    }
    
    
    void PictureView::PaintStatusImage(HDC hDC) {
    
        if (!m_pBitmap) {
            return;
        }
        
        if (m_pBitmap) {
            RECT srcRect = GetPos();
            CDuiRect windowRect(0, 0, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top);
            CDuiRect imageRect(0, 0, m_pBitmap->GetWidth(), m_pBitmap->GetHeight());
            CDuiRect drawRect = getDisplayRect(windowRect, imageRect);
            
            int nSrcWidth = srcRect.right - srcRect.left;
            int nSrcHeight = srcRect.bottom - srcRect.top;
            int nWidth = m_pBitmap->GetWidth();
            int nHeight = m_pBitmap->GetHeight();
            
            if (nWidth < nSrcWidth) {
                nSrcWidth = nWidth;
            }
            
            if (nHeight < nSrcHeight) {
                nSrcHeight = nHeight;
            }
            
            if (nWidth > nSrcWidth) {
                nWidth = nSrcWidth;
            }
            
            if (nHeight > nSrcHeight) {
                nHeight = nSrcHeight;
            }
            
            Graphics graph(hDC);
            /*   graph.DrawImage(m_pBkImage.get(),
            				   RectF((float)0,
            						 (float)0,
            						 (float)windowRect.GetWidth(),
            						 (float)windowRect.GetHeight()),
            				   0,
            				   0,
            				   m_pBkImage->GetWidth(),
            				   m_pBkImage->GetHeight(),
            				   UnitPixel);*/
            
            graph.DrawImage(m_pBitmap.get(),
                            RectF((float)drawRect.left,
                                  (float)drawRect.top,
                                  (float)drawRect.GetWidth(),
                                  (float)drawRect.GetHeight()),
                            0,
                            0,
                            imageRect.GetWidth(),
                            imageRect.GetHeight(),
                            UnitPixel);
            graph.ReleaseHDC(hDC);
        }
    }
    
}