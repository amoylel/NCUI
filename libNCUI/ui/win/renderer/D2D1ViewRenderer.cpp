#include "stdAfx.h"

#ifndef NCUI_NO_D2D1

#include "ui/win/ViewRenderer.h"
#include "ui/win/renderer/GdiRenderer.h"
#include "ui/win/renderer/D2D1Utility.hpp"
#include "ui/win/renderer/GDIPlusBitmap.hpp"

#pragma comment(lib, "Msimg32.lib")




#define NUM_OBJECTS 100


static int num_objects = 400;
static bool cycle_color;
static bool cycle_alpha;
static int cycle_direction = 1;
static int current_alpha = 255;
static int current_color = 25;
ID2D1SolidColorBrush*	_pCommonBrush = NULL;

void
DrawPoints(ID2D1RenderTarget * renderer, const amo::rect& rt) {
    int i;
    int x, y;
    amo::rect viewport = rt;
    
    
    
    for (i = 0; i < num_objects * 4; ++i) {
        /* Cycle the color and alpha, if desired */
        if (cycle_color) {
            current_color += cycle_direction;
            
            if (current_color < 0) {
                current_color = 0;
                cycle_direction = -cycle_direction;
            }
            
            if (current_color > 255) {
                current_color = 255;
                cycle_direction = -cycle_direction;
            }
        }
        
        if (cycle_alpha) {
            current_alpha += cycle_direction;
            
            if (current_alpha < 0) {
                current_alpha = 0;
                cycle_direction = -cycle_direction;
            }
            
            if (current_alpha > 255) {
                current_alpha = 255;
                cycle_direction = -cycle_direction;
            }
        }
        
        //SDL_SetRenderDrawColor(renderer, 255, (Uint8)current_color,
        //                       (Uint8)current_color, (Uint8)current_alpha);
        //
        if (_pCommonBrush == NULL)
            renderer->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Orange),
                                            &_pCommonBrush);
                                            
        _pCommonBrush->SetColor(amo::d2d1::ImFloat4(1, current_color / 255.0,
                                current_color / 255.0, 1).ToD2DColorF());
        x = rand() % viewport.width();
        y = rand() % viewport.height();
        renderer->FillRectangle(amo::d2d1::ImFloat4(x, y, 1, 1).ToD2DRectF(),
                                _pCommonBrush);
        //SDL_RenderDrawPoint(renderer, x, y);
    }
}

void
DrawLines(ID2D1RenderTarget * renderer, const amo::rect& rt) {
    int i;
    int x1, y1, x2, y2;
    amo::rect viewport = rt;
    
    
    
    for (i = 0; i < num_objects; ++i) {
        /* Cycle the color and alpha, if desired */
        if (cycle_color) {
            current_color += cycle_direction;
            
            if (current_color < 0) {
                current_color = 0;
                cycle_direction = -cycle_direction;
            }
            
            if (current_color > 255) {
                current_color = 255;
                cycle_direction = -cycle_direction;
            }
        }
        
        if (cycle_alpha) {
            current_alpha += cycle_direction;
            
            if (current_alpha < 0) {
                current_alpha = 0;
                cycle_direction = -cycle_direction;
            }
            
            if (current_alpha > 255) {
                current_alpha = 255;
                cycle_direction = -cycle_direction;
            }
        }
        
        
        if (_pCommonBrush == NULL)
            renderer->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Orange),
                                            &_pCommonBrush);
                                            
        _pCommonBrush->SetColor(amo::d2d1::ImFloat4(1, current_color / 255.0,
                                current_color / 255.0, 1).ToD2DColorF());
                                
        if (i == 0) {
            renderer->DrawLine(amo::d2d1::ImFloat2(0, 0).ToD2DPointF(),
                               amo::d2d1::ImFloat2(viewport.width() - 1, viewport.height() - 1).ToD2DPointF(),
                               _pCommonBrush);
            renderer->DrawLine(amo::d2d1::ImFloat2(0, viewport.height() - 1).ToD2DPointF(),
                               amo::d2d1::ImFloat2(viewport.width() - 1, 0).ToD2DPointF(), _pCommonBrush);
            renderer->DrawLine(amo::d2d1::ImFloat2(0, viewport.height() / 2).ToD2DPointF(),
                               amo::d2d1::ImFloat2(viewport.width() - 1, viewport.height() / 2).ToD2DPointF(),
                               _pCommonBrush);
            renderer->DrawLine(amo::d2d1::ImFloat2(viewport.width() / 2, 0).ToD2DPointF(),
                               amo::d2d1::ImFloat2(viewport.width() / 2, viewport.height() - 1).ToD2DPointF(),
                               _pCommonBrush);
            /*  SDL_RenderDrawLine(renderer, 0, 0, viewport.w - 1, viewport.h - 1);
              SDL_RenderDrawLine(renderer, 0, viewport.h - 1, viewport.w - 1, 0);
              SDL_RenderDrawLine(renderer, 0, viewport.h / 2, viewport.w - 1, viewport.h / 2);
              SDL_RenderDrawLine(renderer, viewport.w / 2, 0, viewport.w / 2, viewport.h - 1);*/
        } else {
            x1 = (rand() % (viewport.width() * 2)) - viewport.width();
            x2 = (rand() % (viewport.width() * 2)) - viewport.width();
            y1 = (rand() % (viewport.height() * 2)) - viewport.height();
            y2 = (rand() % (viewport.height() * 2)) - viewport.height();
            
            renderer->DrawLine(amo::d2d1::ImFloat2(x1, y1).ToD2DPointF(),
                               amo::d2d1::ImFloat2(x2, y2).ToD2DPointF(), _pCommonBrush);
                               
            //SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }
    }
}

void
DrawRects(ID2D1RenderTarget * renderer, const amo::rect& rt) {
    int i;
    amo::rect rect;
    amo::rect viewport = rt;
    
    
    
    for (i = 0; i < num_objects / 4; ++i) {
        /* Cycle the color and alpha, if desired */
        if (cycle_color) {
            current_color += cycle_direction;
            
            if (current_color < 0) {
                current_color = 0;
                cycle_direction = -cycle_direction;
            }
            
            if (current_color > 255) {
                current_color = 255;
                cycle_direction = -cycle_direction;
            }
        }
        
        if (cycle_alpha) {
            current_alpha += cycle_direction;
            
            if (current_alpha < 0) {
                current_alpha = 0;
                cycle_direction = -cycle_direction;
            }
            
            if (current_alpha > 255) {
                current_alpha = 255;
                cycle_direction = -cycle_direction;
            }
        }
        
        if (_pCommonBrush == NULL)
            renderer->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Orange),
                                            &_pCommonBrush);
                                            
        _pCommonBrush->SetColor(amo::d2d1::ImFloat4(1, current_color / 255.0,
                                current_color / 255.0, 1).ToD2DColorF());
                                
        int width = rand() % (viewport.height() / 2);
        int height = rand() % (viewport.height() / 2);
        
        rect.left((rand() % (viewport.width() * 2) - viewport.width()) - (width / 2));
        rect.top((rand() % (viewport.height() * 2) - viewport.height()) - (height / 2));
        
        rect.width(width);
        rect.height(height);
        renderer->FillRectangle(amo::d2d1::ImFloat4(rect.left(), rect.top(),
                                rect.width(), rect.height()).ToD2DRectF(), _pCommonBrush);
        //SDL_RenderFillRect(renderer, &rect);
    }
}


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
    
    
        if (m_hwndRenderer) {
            m_hwndRenderer->Resize(rc.right - rc.left, rc.bottom - rc.top);
        }
        
        if (m_dcRenderer) {
            m_dcRenderer->Resize(rc.right - rc.left, rc.bottom - rc.top);
        }
        
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
        
        if (!m_hwndRenderer) {
            m_hwndRenderer.reset(new amo::d2d1::D2D1HWNDRenderer());
            m_hwndRenderer->SetHWND(hWnd);
            m_hwndRenderer->Init();
        }
        
        m_hwndRenderer->BeginDraw();
        m_hwndRenderer->Clear(amo::d2d1::ImFloat4(1, 1, 1, 1).ToD2DColorF());
        std::vector<std::shared_ptr<amo::d2d1::D2D1Bitmap> >  bitmaps;
        std::vector<std::shared_ptr<amo::d2d1::D2D1Bitmap> > regions;
        
        
        for (auto& p : m_resource->overlaps) {
        
            std::shared_ptr<amo::d2d1::D2D1Bitmap> bitmap(new amo::d2d1::D2D1Bitmap(
                        m_hwndRenderer->GetMainRT(), p));
            bitmaps.push_back(bitmap);
            
            auto vec =  bitmap->regions();
            regions.insert(regions.end(), vec.begin(), vec.end());
        }
        
        std::sort(regions.begin(),
                  regions.end(), [&](std::shared_ptr<amo::d2d1::D2D1Bitmap>& a,
        std::shared_ptr<amo::d2d1::D2D1Bitmap>& b) {
            return a->getRenderIndex() < b->getRenderIndex();
        });
        
        
        for (auto& p : regions) {
            p->drawBitmap();
        }
        
        /* DrawPoints(m_hwndRenderer->GetMainRT(), GetPos());
         DrawLines(m_hwndRenderer->GetMainRT(), GetPos());
         DrawRects(m_hwndRenderer->GetMainRT(), GetPos());*/
        //m_hwndRenderer->DrawImage("background.png", 0, 0, 0, 0);
        m_hwndRenderer->EndDraw();
    }
    
    void ViewRenderer::PaintBitmapWithHDC(HDC hDC,
                                          bool releaseRenderer /*=false*/) {
        if (!m_resource) {
            return;
        }
        
        if (!m_dcRenderer) {
            m_dcRenderer.reset(new amo::d2d1::D2D1DCRenderer());
            m_dcRenderer->Init();
            m_dcRenderer->BindHDC(hDC, this->GetPos());
        }
        
        m_dcRenderer->BeginDraw();
        
        if (m_paintSettings->offscreen && m_paintSettings->transparent) {
        
            if (m_paintSettings->transparent) {
                m_dcRenderer->Clear(amo::d2d1::ImFloat4(1, 1, 1, 0).ToD2DColorF());
            } else {
                m_dcRenderer->Clear(amo::d2d1::ImFloat4(1, 1, 1, 1).ToD2DColorF());
            }
            
        }
        
        std::vector<std::shared_ptr<amo::d2d1::D2D1Bitmap> >  bitmaps;
        std::vector<std::shared_ptr<amo::d2d1::D2D1Bitmap> > regions;
        
        
        for (auto& p : m_resource->overlaps) {
        
            std::shared_ptr<amo::d2d1::D2D1Bitmap> bitmap(new amo::d2d1::D2D1Bitmap(
                        m_dcRenderer->GetMainRT(), p));
            bitmaps.push_back(bitmap);
            
            auto vec = bitmap->regions();
            regions.insert(regions.end(), vec.begin(), vec.end());
        }
        
        std::sort(regions.begin(),
                  regions.end(), [&](std::shared_ptr<amo::d2d1::D2D1Bitmap>& a,
        std::shared_ptr<amo::d2d1::D2D1Bitmap>& b) {
            return a->getRenderIndex() < b->getRenderIndex();
        });
        
        
        for (auto& p : regions) {
            p->drawBitmap();
        }
        
        m_dcRenderer->EndDraw();
        
        if (releaseRenderer) {
            m_dcRenderer.reset();
        }
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