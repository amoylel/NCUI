// Created by amoylel on 28/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef LIBNCUI_D2D1BITMAP_5E308BE1_A01F_4623_B14F_646BDEA09AD5_HPP__
#define LIBNCUI_D2D1BITMAP_5E308BE1_A01F_4623_B14F_646BDEA09AD5_HPP__

#include <memory>
#include <vector>
#include <d2d1.h>
#include "ui/win/renderer/D2D1Utility.hpp"
#include "ui/win/overlap/Overlap.hpp"



namespace amo {
    namespace d2d1 {
    
    
        class D2D1Bitmap {
        public:
            static ID2D1Bitmap* CreateBitmpFromMemory(ID2D1RenderTarget* renderTarget,
                    std::shared_ptr<Overlap> resource) {
                ID2D1Bitmap* m_bitmap = NULL;
                
                m_bitmap = NULL;
                
                uint8_t* data = (uint8_t*)resource->data();
                HRESULT hr = S_OK;
                D2D1_SIZE_U size;
                size.width = resource->m_settings->width;
                size.height = resource->m_settings->height;
                uint32_t pitch = resource->m_settings->width * 4;
                
                // Create a pixel format and initial its format
                // and alphaMode fields.
                //指定RGB格式
                D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(
                                                    DXGI_FORMAT_B8G8R8A8_UNORM,
                                                    D2D1_ALPHA_MODE_PREMULTIPLIED
                                                );
                //设置属性
                D2D1_BITMAP_PROPERTIES props;
                
                props.pixelFormat = pixelFormat;
                props.dpiX = 0;
                props.dpiY = 0;
                
                D2D1_RECT_U rt = { 0, 0, size.width, size.height };
                resource->set_locked(true);
                
                if (m_bitmap) {
                    hr = m_bitmap->CopyFromMemory(&rt, data, pitch);
                } else {
                    hr = renderTarget->CreateBitmap(
                             size,
                             data,
                             pitch,
                             props,
                             &m_bitmap
                         );
                }
                
                resource->set_locked(false);
                
                if (SUCCEEDED(hr)) {
                
                    return m_bitmap;
                }
                
                SafeRelease(&m_bitmap);
                return NULL;
            }
        public:
            D2D1Bitmap(ID2D1RenderTarget* renderTarget_,
                       std::shared_ptr<Overlap> resource) {
                       
                renderTarget = renderTarget_;
                overlap = resource;
                m_bitmap = CreateBitmpFromMemory(renderTarget, overlap);
                m_releaseBitmap = true;
            }
            
            D2D1Bitmap(std::shared_ptr<D2D1Bitmap> rhs) {
                m_bitmap = rhs->m_bitmap;
                renderTarget = rhs->renderTarget;
                overlap = rhs->overlap;
                m_releaseBitmap = false;
            }
            
            D2D1Bitmap(const D2D1Bitmap& rhs) {
                m_bitmap = rhs.m_bitmap;
                renderTarget = rhs.renderTarget;
                overlap = rhs.overlap;
                m_releaseBitmap = false;
            }
            
            ~D2D1Bitmap() {
                if (m_releaseBitmap && m_bitmap != NULL) {
                    SafeRelease(&m_bitmap);
                }
            }
            std::vector<std::shared_ptr<D2D1Bitmap> > regions() {
                std::vector<std::shared_ptr<D2D1Bitmap> > vec;
                
                for (auto& p : overlap->m_settings->getOverlapRegions()->m_regions) {
                    auto rect = p;
                    
                    if (rect.dst.empty()) {
                        rect.dst = overlap->m_settings->canvasRect;
                    }
                    
                    std::shared_ptr<D2D1Bitmap> bitmap(new D2D1Bitmap(*this));
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
                    D2D1_RECT_F dstRect = { (FLOAT)dst_rect.left(), (FLOAT)dst_rect.top(), (FLOAT)dst_rect.right(), (FLOAT)dst_rect.bottom() };
                    D2D1_RECT_F srcRect = { (FLOAT)m_rect->src.left(), (FLOAT)m_rect->src.top(), (FLOAT)m_rect->src.right(), (FLOAT)m_rect->src.bottom() };
                    renderTarget->DrawBitmap(m_bitmap, dstRect, 1.0f,
                                             D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcRect);
                } else if (m_rect->renderMode == 2) {
                    // 拉伸
                    D2D1_RECT_F dstRect = { (FLOAT)m_rect->dst.left(), (FLOAT)m_rect->dst.top(), (FLOAT)m_rect->dst.right(), (FLOAT)m_rect->dst.bottom() };
                    D2D1_RECT_F srcRect = { (FLOAT)m_rect->src.left(), (FLOAT)m_rect->src.top(), (FLOAT)m_rect->src.right(), (FLOAT)m_rect->src.bottom() };
                    renderTarget->DrawBitmap(m_bitmap, dstRect, 1.0f,
                                             D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcRect);
                } else {
                
                    int srcWidth = (std::min)(m_rect->dst.width(), m_rect->src.width());
                    int srcHeight = (std::min)(m_rect->dst.height(), m_rect->src.height());
                    // 平铺
                    D2D1_RECT_F dstRect = { (FLOAT)m_rect->dst.left(), (FLOAT)m_rect->dst.top(), (FLOAT)m_rect->dst.width(), (FLOAT)m_rect->dst.height() };
                    D2D1_RECT_F srcRect = { (FLOAT)m_rect->src.left(), (FLOAT)m_rect->src.top(), (FLOAT)srcWidth, (FLOAT)srcHeight };
                    renderTarget->DrawBitmap(m_bitmap, dstRect, 1.0f,
                                             D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcRect);
                }
                
                return true;
                
            }
        private:
            bool m_releaseBitmap;
            ID2D1Bitmap* m_bitmap;
            ID2D1RenderTarget* renderTarget;
            std::shared_ptr<Overlap> overlap;
            std::shared_ptr< OverlapRect> m_rect;
        };
    }
}



#endif //LIBNCUI_D2D1BITMAP_5E308BE1_A01F_4623_B14F_646BDEA09AD5_HPP__