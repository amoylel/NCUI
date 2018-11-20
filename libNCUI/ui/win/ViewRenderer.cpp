#include "stdAfx.h"

#ifndef NCUI_NO_D2D1

#else

#include "ui/win/ViewRenderer.h"
#include <amo/timer.hpp>
#include "ui/win/WinUserMsg.hpp"
#include "ui/win/renderer/GDIPlusBitmap.hpp"


namespace amo {
    void ViewRenderer::insertBitmap(std::shared_ptr<PaintResource> image) {
        m_resource = image;
        
        if (m_resource) {
            m_resource->setPos(GetPos());
            ::PostMessage(GetManager()->GetPaintWindow(), WM_USER_PAINT, NULL, NULL);
        }
        
        //Invalidate();
        
        //GetManager()->NeedUpdate();
        //PaintStatusImage(GetManager()->GetPaintDC());
    }
    
    
    
    void ViewRenderer::SetToolTip(LPCTSTR pstrText) {
        return CControlUI::SetToolTip(pstrText);
    }
    
    void ViewRenderer::SetPos(RECT rc, bool bNeedInvalidate /*= true*/) {
    
    
    
    
        /*renderer->hwndRenderTarget->Resize(D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top));*/
        //renderer->dcRenderTarget->Resize(D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top));
        return CControlUI::SetPos(rc, bNeedInvalidate);
        
    }
    
    void ViewRenderer::DoInit() {
        if (isAccelerator()) {
            /* renderer.reset(new D2D1RendererOld());
            renderer->drawBackground(m_bDrawBackground);
            renderer->initialize();*/
        }
        
        return CControlUI::DoInit();
    }
    
    void ViewRenderer::drawBackground(bool bDraw /*= true*/) {
        m_bDrawBackground = bDraw;
    }
    
    void ViewRenderer::PaintBitmap(HWND hWnd) {
        static std::vector<int> vec;
        amo::timer t;
        
        
        if (!m_paintSettings) {
            return;
        }
        
        if (!m_paintSettings->offscreen) {
            return;
        }
        
        if (!m_resource) {
            return;
        }
        
        m_resource->setPos(GetPos());
        
        
        for (auto& p : m_resource->overlaps) {
            p->setCanvasRect(GetPos());
        }
        
        
        if (m_paintSettings->accelerator) {
            if (m_paintSettings->transparent) {
                PaintBitmapWithHDC(GetDC(hWnd));
            } else {
                PaintBitmapWithHWND(hWnd);
            }
        } else {
            PaintBitmapWithGDI(GetDC(hWnd));
        }
        
        updateFPS();
        vec.push_back(t.elapsed());
        
        if (vec.size() >= 30) {
            int ntotal = 0;
            
            for (auto& p : vec) {
                ntotal += p;
            }
            
            $cdevel("rendering time:{}", ntotal / vec.size());
            
            vec.clear();
        }
    }
    
    
    void ViewRenderer::PaintBitmapWithHWND(HWND hWnd) {
        if (!m_resource) {
            return;
        }
        
        return PaintBitmapWithHDC(GetDC(hWnd));
    }
    
    void ViewRenderer::PaintBitmapWithHDC(HDC hDC,
                                          bool releaseRenderer /*=false*/) {
        if (!m_resource) {
            return;
        }
        
        return PaintBitmapWithGDI(hDC);
    }
    
    void ViewRenderer::PaintBitmapWithGDI(HDC hDC) {
        if (!m_resource) {
            return;
        }
        
        
        amo::rect rect(GetPos());
        
        HDC memDC = ::CreateCompatibleDC(hDC);	//创建兼容内存DC
        HBITMAP hBitmap = NULL;
        HBITMAP hOldBitmap = NULL;
        
        
        hBitmap = ::CreateCompatibleBitmap(hDC, rect.width(), rect.height());
        hOldBitmap = (HBITMAP)::SelectObject(memDC, hBitmap); //载入该位图
        
        std::shared_ptr<Graphics> m_graphics(new Graphics(memDC));
        
        
        if (m_paintSettings->offscreen && !m_paintSettings->transparent) {
        
            Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255));
            m_graphics->FillRectangle(&brush,
                                      rect.left(),
                                      rect.top(),
                                      rect.width(),
                                      rect.height());
        }
        
        std::vector<std::shared_ptr<amo::d2d1::GdiplusBitmap> >  bitmaps;
        std::vector<std::shared_ptr<amo::d2d1::GdiplusBitmap> > regions;
        
        
        for (auto& p : m_resource->overlaps) {
        
            std::shared_ptr<amo::d2d1::GdiplusBitmap> bitmap(new amo::d2d1::GdiplusBitmap(
                        m_graphics, p));
            bitmaps.push_back(bitmap);
            auto vec = bitmap->regions();
            regions.insert(regions.end(), vec.begin(), vec.end());
        }
        
        std::sort(regions.begin(),
                  regions.end(), [&](std::shared_ptr<amo::d2d1::GdiplusBitmap>& a,
        std::shared_ptr<amo::d2d1::GdiplusBitmap>& b) {
            return a->getRenderIndex() < b->getRenderIndex();
        });
        
        
        for (auto& p : regions) {
            p->drawBitmap();
        }
        
        ::BitBlt(hDC, rect.left(), rect.top(), rect.width(), rect.height(),
                 memDC, rect.left(), rect.top(), SRCCOPY);
                 
        m_graphics->ReleaseHDC(memDC);
        ::SelectObject(memDC, hOldBitmap);
        //::DeleteObject(hOldBitmap);
        ::DeleteObject(hBitmap);
        //::ReleaseDC(NULL, memDC);
        ::DeleteDC(memDC);
    }
    
    ViewRenderer::ViewRenderer() {
        setAccelerator(false);
        
        m_bDrawBackground = true;
        m_nLastFPS = m_nCount = 0;
        m_nLastTimestamp = amo::timer::now<amo::chrono::seconds>();
    }
    
    ViewRenderer::~ViewRenderer() {
    
    }
    
    LPCTSTR ViewRenderer::GetClass() const {
        return _T("RenderViewRender");
    }
    
    
    void ViewRenderer::PaintStatusImage(HDC hDC) {
        static std::vector<int> vec;
        amo::timer t;
        
        if (!m_resource) {
            return;
        }
        
        m_resource->setPos(GetPos());
        
        for (auto& p : m_resource->overlaps) {
            p->setCanvasRect(GetPos());
        }
        
        if (!m_paintSettings->offscreen) {
            return;
        }
        
        if (m_paintSettings->accelerator) {
        
            PaintBitmapWithHDC(hDC, true);
        } else {
            PaintBitmapWithGDI(hDC);
        }
        
        updateFPS();
        vec.push_back(t.elapsed());
        
        if (vec.size() >= 30) {
            int ntotal = 0;
            
            for (auto& p : vec) {
                ntotal += p;
            }
            
            $cdevel("rendering time:{}", ntotal / vec.size());
            
            vec.clear();
        }
    }
    
    LayerViewRender::~LayerViewRender() {
    
    }
    
    void LayerViewRender::insertBitmap(std::shared_ptr<PaintResource> image) {
        m_resource = image;
        Invalidate();
    }
    
}
#endif