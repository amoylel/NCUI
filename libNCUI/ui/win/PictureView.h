// Created by amoylel on 10/06/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_PICTUREVIEW_H__
#define AMO_PICTUREVIEW_H__

#include <memory>

namespace amo {


    //页面渲染控件
    class PictureView : public CControlUI {
    public:
        PictureView();
        virtual ~PictureView();
        LPCTSTR GetClass() const override;
        virtual void PaintStatusImage(HDC hDC) override;
        
        void setPicture(const std::wstring& file);
        
    protected:
    
        CDuiRect getDisplayRect(const CDuiRect& windowRect, const CDuiRect& textureRect);
    protected:
        std::shared_ptr<Gdiplus::Bitmap> m_pBitmap;
        Gdiplus::RectF m_dstRect;
        std::shared_ptr<Gdiplus::Bitmap> m_pBkImage;
        
        ULONG_PTR				m_gdiplusToken;
        GdiplusStartupInput		m_gdiplusStartupInput;
        
    };
    
    
}
#endif // AMO_PICTUREVIEW_H__