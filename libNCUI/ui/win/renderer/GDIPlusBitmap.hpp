// Created by amoylel on 08/30/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_GDIPLUSBITMAP_1C6EB951_C13F_424A_ADFB_C51C1684FDA9_HPP__
#define AMO_GDIPLUSBITMAP_1C6EB951_C13F_424A_ADFB_C51C1684FDA9_HPP__


#include <memory>
#include <vector>
#include <d2d1.h>
#include "ui/win/renderer/D2D1Utility.hpp"
#include "ui/win/overlap/Overlap.hpp"



namespace amo {
    namespace d2d1 {
    
    
        class GdiplusBitmap {
        public:
            static std::shared_ptr<Gdiplus::Bitmap> CreateBitmpFromMemory(
                std::shared_ptr<Overlap> resource) {
                
                
                
                int imageWidth = resource->m_settings->width;
                int imageHeight = resource->m_settings->height;
                int imageSize = resource->size();
                
                std::shared_ptr<Bitmap>  pBitmap;
                
                if (resource->data() == NULL) {
                    return pBitmap;
                }
                
                resource->set_locked(true);
                std::shared_ptr<Gdiplus::Bitmap> image;
                image.reset(new Gdiplus::Bitmap(imageWidth,
                                                imageHeight,
                                                imageWidth * 4,
                                                PixelFormat32bppARGB,
                                                (BYTE*)resource->data()));
                resource->set_locked(false);
                
                return image;
            }
            
        public:
            GdiplusBitmap(std::shared_ptr<Graphics> graphics_,
                          std::shared_ptr<Overlap> resource) {
                          
                graphics = graphics_;
                overlap = resource;
                m_bitmap = CreateBitmpFromMemory(overlap);
                m_releaseBitmap = true;
            }
            
            GdiplusBitmap(std::shared_ptr<GdiplusBitmap> rhs) {
                m_bitmap = rhs->m_bitmap;
                graphics = rhs->graphics;
                overlap = rhs->overlap;
                m_releaseBitmap = false;
            }
            
            GdiplusBitmap(const GdiplusBitmap& rhs) {
                m_bitmap = rhs.m_bitmap;
                graphics = rhs.graphics;
                overlap = rhs.overlap;
                m_releaseBitmap = false;
            }
            
            ~GdiplusBitmap() {
            
            }
            std::vector<std::shared_ptr<GdiplusBitmap> > regions() {
                std::vector<std::shared_ptr<GdiplusBitmap> > vec;
                
                for (auto& p : overlap->m_settings->getOverlapRegions()->m_regions) {
                    auto rect = p;
                    
                    if (rect.dst.empty()) {
                        rect.dst = overlap->m_settings->canvasRect;
                    }
                    
                    std::shared_ptr<GdiplusBitmap> bitmap(new GdiplusBitmap(*this));
                    bitmap->setOverlapRect(rect);
                    vec.push_back(bitmap);
                }
                
                return vec;
            }
            
            void setOverlapRect(const OverlapRect& rect) {
                m_rect.reset(new OverlapRect(rect));
            }
            
            int getRenderIndex() const {
                return m_rect->index;
            }
            
            bool drawBitmap() {
                if (m_bitmap == NULL) {
                    return false;
                }
                
                
                //m_rect->renderMode = 1;
                
                if (m_rect->renderMode == 1) {
                    // 居中
                    amo::rect dst_rect = m_rect->dst.get_full_rect(m_rect->src);
                    amo::rect dstRect = { dst_rect.left(), dst_rect.top(), dst_rect.right(), dst_rect.bottom() };
                    amo::rect srcRect = { m_rect->src.left(), m_rect->src.top(), m_rect->src.right(), m_rect->src.bottom() };
                    
                    graphics->DrawImage(m_bitmap.get(),
                                        RectF((float)dst_rect.left(),
                                              (float)dst_rect.top(),
                                              (float)dst_rect.width(),
                                              (float)dst_rect.height()),
                                        srcRect.left(),
                                        srcRect.top(),
                                        (float)srcRect.width(),
                                        (float)srcRect.height(),
                                        UnitPixel);
                                        
                                        
                } else if (m_rect->renderMode == 2) {
                    // 拉伸
                    amo::rect dstRect = { m_rect->dst.left(), m_rect->dst.top(), m_rect->dst.right(), m_rect->dst.bottom() };
                    amo::rect dst_rect = dstRect;
                    amo::rect srcRect = { m_rect->src.left(), m_rect->src.top(), m_rect->src.right(), m_rect->src.bottom() };
                    graphics->DrawImage(m_bitmap.get(),
                                        RectF((float)dst_rect.left(),
                                              (float)dst_rect.top(),
                                              (float)dst_rect.width(),
                                              (float)dst_rect.height()),
                                        srcRect.left(),
                                        srcRect.top(),
                                        (float)srcRect.width(),
                                        (float)srcRect.height(),
                                        UnitPixel);
                } else {
                
                    int srcWidth = (std::min)(m_rect->dst.width(), m_rect->src.width());
                    int srcHeight = (std::min)(m_rect->dst.height(), m_rect->src.height());
                    // 平铺
                    amo::rect dstRect = { m_rect->dst.left(), m_rect->dst.top(), m_rect->dst.width(), m_rect->dst.height() };
                    amo::rect dst_rect = dstRect;
                    amo::rect srcRect = { m_rect->src.left(), m_rect->src.top(),  srcWidth,  srcHeight };
                    graphics->DrawImage(m_bitmap.get(),
                                        RectF((float)dst_rect.left(),
                                              (float)dst_rect.top(),
                                              (float)dst_rect.width(),
                                              (float)dst_rect.height()),
                                        srcRect.left(),
                                        srcRect.top(),
                                        (float)srcRect.width(),
                                        (float)srcRect.height(),
                                        UnitPixel);
                }
                
                return true;
                
            }
        private:
            bool m_releaseBitmap;
            std::shared_ptr<Gdiplus::Bitmap> m_bitmap;
            std::shared_ptr<Graphics> graphics;
            std::shared_ptr<Overlap> overlap;
            std::shared_ptr< OverlapRect> m_rect;
        };
    }
}


#endif // AMO_GDIPLUSBITMAP_1C6EB951_C13F_424A_ADFB_C51C1684FDA9_HPP__
