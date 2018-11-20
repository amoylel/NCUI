// Created by amoylel on 08/19/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_GDIRENDERER_33BFE9BE_15A8_42A1_8847_667EC0BA0F11_H__
#define AMO_GDIRENDERER_33BFE9BE_15A8_42A1_8847_667EC0BA0F11_H__

namespace amo {
    class PaintResource;
    class GdiRenderer {
    public:
        GdiRenderer();
        
        ~GdiRenderer();
        
        bool initialize();
        
        
        void Render(HDC hDC, std::shared_ptr<PaintResource> resource);
        
        void Render(HDC hDC, std::shared_ptr<PaintResource> resource, std::shared_ptr<Overlap> overlap);
        
        std::shared_ptr<Gdiplus::Bitmap> CreateBitmpFromMemory(std::shared_ptr<PaintResource> resource);
        
        std::shared_ptr<Gdiplus::Bitmap> CreateBitmpFromMemory(std::shared_ptr<Overlap> resource);
        
        
        void setRect(amo::rect& rc) {
            m_rect = rc;
        }
    public:
    
        amo::rect m_rect;
        
        
        
        
        
    };
}

#endif // AMO_GDIRENDERER_33BFE9BE_15A8_42A1_8847_667EC0BA0F11_H__

