#include "stdafx.h"
#include "ui/win/renderer/GdiRenderer.h"


namespace amo {

    GdiRenderer::GdiRenderer() {
    
    }
    
    GdiRenderer::~GdiRenderer() {
    
    }
    
    bool GdiRenderer::initialize() {
        return true;
    }
    
    void GdiRenderer::Render(HDC hDC, std::shared_ptr<PaintResource> resource) {
        for (auto& p : resource->overlaps) {
            Render(hDC, resource, p);
        }
        
    }
    
    void GdiRenderer::Render(HDC hDC, std::shared_ptr<PaintResource> resource,
                             std::shared_ptr<Overlap> overlap) {
                             
        std::shared_ptr<Gdiplus::Bitmap> m_pBitmap = CreateBitmpFromMemory(overlap);
        
        if (m_pBitmap) {
        
            RECT srcRect = resource->getPos();
            
            int nSrcWidth = srcRect.right - srcRect.left;
            int nSrcHeight = srcRect.bottom - srcRect.top;
            int nWidth = m_pBitmap->GetWidth();
            int nHeight = m_pBitmap->GetHeight();
            
            amo::rect r1(0, 0, nSrcWidth, nSrcHeight);
            amo::rect r2(0, 0, nWidth, nHeight);
            amo::rect r3 = r1.intersect(r2);
            
            
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
            graph.DrawImage(m_pBitmap.get(),
                            RectF((float)srcRect.left,
                                  (float)srcRect.top,
                                  (float)nSrcWidth,
                                  (float)nSrcHeight),
                            0,
                            0,
                            (float)nWidth,
                            (float)nHeight,
                            UnitPixel);
            graph.ReleaseHDC(hDC);
            
        }
    }
    
    std::shared_ptr<Gdiplus::Bitmap> GdiRenderer::CreateBitmpFromMemory(
        std::shared_ptr<PaintResource> resource) {
        return{};
    }
    
    std::shared_ptr<Gdiplus::Bitmap> GdiRenderer::CreateBitmpFromMemory(
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
        
        /* pBitmap.reset(new Bitmap(imageWidth, imageHeight,
                                  PixelFormat32bppARGB));
        
         if (!pBitmap) {
             return pBitmap;
         }
        
         resource->set_locked(true);
         BitmapData bmpData;
         Gdiplus::Rect rect(0, 0, imageWidth, imageHeight);
         pBitmap->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppARGB, &bmpData);
         BYTE *pByte = (BYTE*)bmpData.Scan0;
        
         memcpy(bmpData.Scan0, resource->data(), imageSize);
        
         pBitmap->UnlockBits(&bmpData);
         resource->set_locked(false);
         return pBitmap;*/
    }
    
}

