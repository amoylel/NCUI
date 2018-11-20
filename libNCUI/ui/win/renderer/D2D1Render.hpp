// Created by amoylel on 08/26/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_D2DRENDER_75ED5962_D439_4908_850B_7689FAD3C510_H__
#define AMO_D2DRENDER_75ED5962_D439_4908_850B_7689FAD3C510_H__
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <unordered_map>
#include <memory>

#include "ui/win/renderer/D2D1Utility.hpp"
#include "ui/win/renderer/D2D1Bitmap.hpp"



namespace amo {
    namespace d2d1 {
        class D2D1CompatibleRender {
        public:
            enum TEXT_ALIGNMENT_MODE {
                MODE_LEFT,
                MODE_CENTER,
                MODE_RIGHT,
            };
            
            std::wstring ATOW(const std::string& src) {
                std::wstring ret;
                
                if (!src.empty()) {
                    int nNum = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);
                    
                    if (nNum) {
                        WCHAR* wideChaUtilStr = new WCHAR[nNum + 1];
                        wideChaUtilStr[0] = 0;
                        nNum = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, wideChaUtilStr,
                                                   nNum + 1);
                        ret = wideChaUtilStr;
                        delete[] wideChaUtilStr;
                    } else {
                        // OutLog("Wrong convert to WideChar code:0x%x", GetLastError());
                    }
                }
                
                return ret;
            }
            
            std::string WTOA(const std::wstring& src) {
                std::string ret;
                
                if (!src.empty()) {
                    int nNum = WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, NULL, 0, NULL,
                                                   FALSE);
                                                   
                    if (nNum) {
                        char* utf8String = new char[nNum + 1];
                        utf8String[0] = 0;
                        nNum = WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, utf8String, nNum + 1,
                                                   NULL, FALSE);
                        ret = utf8String;
                        delete[] utf8String;
                    } else {
                        //OutLog("Wrong convert to Utf8 code:0x%x", GetLastError());
                    }
                }
                
                return ret;
            }
        public:
        
            D2D1CompatibleRender() {
                _penWidth = Styles.StrokeWidth;
                _pD2DFactory = NULL;
                _pDWriteFactory = NULL;
                _pWICFactory = NULL;
                _pMainRT = NULL;
                //dcRenderTarget = NULL;
                _pCommonBrush = NULL;
                _pTextFormat = NULL;
                m_bCompatibleMode = false;
            }
            
            
            ~D2D1CompatibleRender() {
                clearBitmaps();
                
                SafeRelease(&_pMainRT);
            }
            
            void clearBitmaps() {
            
                for (auto itor = _mapBitmaps.begin(); itor != _mapBitmaps.end(); ++itor) {
                    SafeRelease(&itor->second);
                }
                
                _mapBitmaps.clear();
            }
            virtual HRESULT CreateDeviceIndependentResources() {
                HRESULT hr;
                ID2D1GeometrySink *pSink = NULL;
                
                hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_pD2DFactory);
                
                if (SUCCEEDED(hr)) {
                    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(_pDWriteFactory),
                                             reinterpret_cast<IUnknown **>(&_pDWriteFactory));
                }
                
                CoInitialize(NULL);
                
                if (SUCCEEDED(hr)) {
                    hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
                                          IID_PPV_ARGS(&_pWICFactory));
                }
                
                return hr;
            }
            
            virtual HRESULT CreateDeviceResources() {
                HRESULT hr = S_OK;
                
                /*if (!_pMainRT) {
                RECT rc;
                GetClientRect(hWnd, &rc);
                D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
                hr = _pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(hWnd, size), &_pMainRT);
                }
                
                if (!dcRenderTarget) {
                
                D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
                D2D1_RENDER_TARGET_TYPE_DEFAULT,
                D2D1::PixelFormat(
                DXGI_FORMAT_B8G8R8A8_UNORM,
                D2D1_ALPHA_MODE_PREMULTIPLIED),
                0,
                0,
                D2D1_RENDER_TARGET_USAGE_NONE,
                D2D1_FEATURE_LEVEL_DEFAULT);
                auto props2 = D2D1::RenderTargetProperties();
                hr = _pD2DFactory->CreateDCRenderTarget(&props, &dcRenderTarget);
                dcRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &gBrush);
                }*/
                
                return hr;
            }
            
            
            
            
            
            
            virtual  void Resize(float w, float h) {
            
            }
            virtual  ID2D1Bitmap* GetBitmap() {
                if (!m_bCompatibleMode) {
                    return NULL;
                }
                
                ID2D1BitmapRenderTarget* target = reinterpret_cast<ID2D1BitmapRenderTarget*>
                                                  (_pMainRT);
                                                  
                if (target == NULL) {
                    return NULL;
                }
                
                ID2D1Bitmap* pBitmap = NULL;
                target->GetBitmap(&pBitmap);
                return pBitmap;
            }
            
            ID2D1RenderTarget* GetMainRT() {
                return _pMainRT;
            }
            
            void SetMainRT(ID2D1RenderTarget* ptr) {
                ID2D1BitmapRenderTarget* target = dynamic_cast<ID2D1BitmapRenderTarget*>
                                                  (ptr);
                                                  
                _pMainRT = ptr;
            }
            
            ID2D1RenderTarget* ChooseRT(ID2D1RenderTarget* pRT) {
                return pRT == NULL ? _pMainRT : pRT;
            }
            
            void BeginDraw() {
                ID2D1RenderTarget* pRT = ChooseRT(NULL);
                pRT->BeginDraw();
                pRT->Clear();
            }
            
            void Clear(D2D_COLOR_F color) {
                ID2D1RenderTarget* pRT = ChooseRT(NULL);
                pRT->Clear(color);
            }
            
            void EndDraw() {
                ID2D1RenderTarget* pRT = ChooseRT(NULL);
                pRT->EndDraw();
            }
            
            void PushClipRect(ImFloat4 rt) {
                ID2D1RenderTarget* pRenderTarget = ChooseRT(NULL);
                pRenderTarget->PushAxisAlignedClip(rt.ToD2DRectF(),
                                                   D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
            }
            
            void PopClipRect(ID2D1RenderTarget* pRT) {
                ID2D1RenderTarget* pRenderTarget = ChooseRT(NULL);
                pRenderTarget->PopAxisAlignedClip();
            }
            
            void SetPenWidth(float width) {
                _penWidth = width;
            }
            
            void DrawPoint(ImFloat4 color, const ImFloat2& pt) {
                ID2D1RenderTarget* pRenderTarget = ChooseRT(NULL);
                _pCommonBrush->SetColor(color.ToD2DColorF());
                pRenderTarget->DrawRectangle(D2D1::RectF(pt.x, pt.y, pt.x, pt.y), _pCommonBrush,
                                             _penWidth);
            }
            
            void DrawLine(ImFloat4 color, const ImFloat2& pt1,
                          const ImFloat2& pt2) {
                ID2D1RenderTarget* pRenderTarget = ChooseRT(NULL);
                _pCommonBrush->SetColor(color.ToD2DColorF());
                pRenderTarget->DrawLine(D2D1::Point2F(pt1.x, pt1.y), D2D1::Point2F(pt2.x,
                                        pt2.y), _pCommonBrush, _penWidth);
            }
            
            void DrawRect(ImFloat4 color, ImFloat4 rt,
                          bool isFilled = false) {
                ID2D1RenderTarget* pRenderTarget = ChooseRT(NULL);
                _pCommonBrush->SetColor(color.ToD2DColorF());
                
                if (isFilled) {
                    pRenderTarget->FillRectangle(rt.ToD2DRectF(), _pCommonBrush);
                } else {
                    pRenderTarget->DrawRectangle(rt.ToD2DRectF(), _pCommonBrush, _penWidth);
                }
            }
            
            void DrawRoundedRect(ImFloat4 color, ImFloat4 rt,
                                 float radiusX, float radiusY, bool isFilled = false) {
                ID2D1RenderTarget* pRenderTarget = ChooseRT(NULL);
                _pCommonBrush->SetColor(color.ToD2DColorF());
                
                D2D1_ROUNDED_RECT roundRect = D2D1::RoundedRect(rt.ToD2DRectF(), radiusX,
                                              radiusY);
                                              
                if (isFilled) {
                    pRenderTarget->FillRoundedRectangle(roundRect, _pCommonBrush);
                } else {
                    pRenderTarget->DrawRoundedRectangle(roundRect, _pCommonBrush, _penWidth);
                }
            }
            
            void DrawEllipse(ImFloat4 color, float x, float y,
                             float radiusX, float radiusY, bool isFilled = false) {
                ID2D1RenderTarget* pRenderTarget = ChooseRT(NULL);
                _pCommonBrush->SetColor(color.ToD2DColorF());
                
                D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radiusX, radiusY);
                
                if (isFilled) {
                    pRenderTarget->FillEllipse(ellipse, _pCommonBrush);
                } else {
                    pRenderTarget->DrawEllipse(ellipse, _pCommonBrush, _penWidth);
                }
            }
            
            void DrawTriangle(ImFloat4 color, const ImFloat2 &pt1,
                              const ImFloat2 &pt2, const ImFloat2 &pt3, bool isFilled = false) {
                ImFloat2 arrayPoint[3];
                arrayPoint[0] = pt1;
                arrayPoint[1] = pt2;
                arrayPoint[2] = pt3;
                DrawPolygon(color, arrayPoint, 3, isFilled);
            }
            
            void DrawPolygon(ImFloat4 color, ImFloat2* array,
                             ImUint count, bool isFilled = false) {
                ID2D1RenderTarget* pRenderTarget = ChooseRT(NULL);
                _pCommonBrush->SetColor(color.ToD2DColorF());
                
                D2D1_POINT_2F* D2DPoints = new D2D1_POINT_2F[count];
                
                if (D2DPoints != NULL) {
                    for (ImUint i = 0; i < count; i++) {
                        D2DPoints[i] = D2D1::Point2F(array[i].x, array[i].y);
                    }
                    
                    ID2D1PathGeometry* pGeometry = NULL;
                    HRESULT hr = _pD2DFactory->CreatePathGeometry(&pGeometry);
                    
                    if (SUCCEEDED(hr)) {
                        ID2D1GeometrySink *pSink = NULL;
                        hr = pGeometry->Open(&pSink);
                        
                        if (SUCCEEDED(hr)) {
                            pSink->BeginFigure(D2DPoints[0], D2D1_FIGURE_BEGIN_FILLED);
                            pSink->AddLines(D2DPoints, count);
                            pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
                        }
                        
                        pSink->Close();
                    }
                    
                    if (isFilled) {
                        pRenderTarget->FillGeometry(pGeometry, _pCommonBrush);
                    } else {
                        pRenderTarget->DrawGeometry(pGeometry, _pCommonBrush, _penWidth);
                    }
                    
                    SafeRelease(&pGeometry);
                    delete[] D2DPoints;
                }
            }
            
            void DrawPolygonalLine(ImFloat4 color, ImFloat2* array,
                                   ImUint count) {
                ID2D1RenderTarget* pRenderTarget = ChooseRT(NULL);
                _pCommonBrush->SetColor(color.ToD2DColorF());
                
                ID2D1PathGeometry* pGeometry = NULL;
                D2D1_POINT_2F* D2DPoints = new D2D1_POINT_2F[count];
                
                if (D2DPoints != NULL) {
                    for (ImUint i = 0; i < count; i++) {
                        D2DPoints[i] = D2D1::Point2F(array[i].x, array[i].y);
                    }
                    
                    HRESULT hr = _pD2DFactory->CreatePathGeometry(&pGeometry);
                    
                    if (SUCCEEDED(hr)) {
                        ID2D1GeometrySink *pSink = NULL;
                        hr = pGeometry->Open(&pSink);
                        
                        if (SUCCEEDED(hr)) {
                            pSink->BeginFigure(D2DPoints[0], D2D1_FIGURE_BEGIN_HOLLOW);
                            pSink->AddLines(D2DPoints, count);
                            pSink->EndFigure(D2D1_FIGURE_END_OPEN);
                        }
                        
                        pSink->Close();
                    }
                    
                    pRenderTarget->DrawGeometry(pGeometry, _pCommonBrush, _penWidth);
                    
                    SafeRelease(&pGeometry);
                    delete[] D2DPoints;
                }
            }
            
            void DrawText(ImFloat4 color, std::string txt,
                          const ImFloat4& rt, TEXT_ALIGNMENT_MODE mode =
                              D2D1CompatibleRender::MODE_CENTER) {
                ID2D1RenderTarget* pRenderTarget = ChooseRT(NULL);
                _pCommonBrush->SetColor(color.ToD2DColorF());
                
                switch (mode) {
                case D2D1CompatibleRender::MODE_LEFT:
                    _pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                    break;
                    
                case D2D1CompatibleRender::MODE_CENTER:
                    _pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                    break;
                    
                case D2D1CompatibleRender::MODE_RIGHT:
                    _pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
                    break;
                    
                default:
                    break;
                }
                
                pRenderTarget->DrawText(ATOW(txt).c_str(), ATOW(txt).length(), _pTextFormat,
                                        rt.ToD2DRectF(), _pCommonBrush);
            }
            
            ImFloat2 GetTextSize(std::string txt) {
                ImFloat2 size;
                IDWriteTextLayout* textLayout = NULL;
                
                HRESULT hr = _pDWriteFactory->CreateTextLayout(ATOW(txt).c_str(),
                             ATOW(txt).length(), _pTextFormat, 0, 0, &textLayout);
                             
                if (textLayout != NULL) {
                    DWRITE_TEXT_METRICS textMetrics;
                    textLayout->GetMetrics(&textMetrics);
                    size.x = ceil(textMetrics.widthIncludingTrailingWhitespace);
                    size.y = ceil(textMetrics.height);
                }
                
                SafeRelease(&textLayout);
                return size;
            }
            
            void DrawImage(std::string image, float x = 0,
                           float y = 0, float w = 0, float h = 0) {
                // find image object
                assert(!image.empty());
                ID2D1Bitmap* pBitmap = NULL;
                
                auto iter = _mapBitmaps.find(image);
                
                if (iter == _mapBitmaps.end()) {
                    _mapBitmaps[image] = LoadImage(image, w, h);
                    assert(_mapBitmaps[image] != NULL);
                }
                
                pBitmap = _mapBitmaps[image];
                
                // draw image object
                ID2D1RenderTarget* pRenderTarget = ChooseRT(NULL);
                
                if (w == 0 || h == 0) {
                    w = pBitmap->GetSize().width;
                    h = pBitmap->GetSize().height;
                }
                
                pRenderTarget->DrawBitmap(pBitmap, D2D1::RectF(x, y, x + w, y + h));
            }
            
            ID2D1Bitmap* LoadImage(std::string image, UINT width, UINT height) {
                ID2D1Bitmap* pBitmap = nullptr;
                IWICBitmapDecoder *pDecoder = nullptr;
                IWICBitmapFrameDecode *pSource = nullptr;
                IWICStream *pStream = nullptr;
                IWICFormatConverter *pConverter = nullptr;
                IWICBitmapScaler *pScaler = nullptr;
                
                HRESULT hr = _pWICFactory->CreateDecoderFromFilename(
                                 ATOW(image).c_str(),
                                 nullptr,
                                 GENERIC_READ,
                                 WICDecodeMetadataCacheOnLoad,
                                 &pDecoder);
                                 
                if (SUCCEEDED(hr)) {
                    hr = pDecoder->GetFrame(0, &pSource);
                }
                
                if (SUCCEEDED(hr)) {
                    hr = _pWICFactory->CreateFormatConverter(&pConverter);
                }
                
                if (SUCCEEDED(hr)) {
                    if (width != 0 || height != 0) {
                        UINT originalWidth, originalHeight;
                        hr = pSource->GetSize(&originalWidth, &originalHeight);
                        
                        if (SUCCEEDED(hr)) {
                            if (width == 0) {
                                FLOAT scalar = static_cast<FLOAT>(height) / static_cast<FLOAT>(originalHeight);
                                width = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
                            } else if (height == 0) {
                                FLOAT scalar = static_cast<FLOAT>(width) / static_cast<FLOAT>(originalWidth);
                                height = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
                            }
                            
                            hr = _pWICFactory->CreateBitmapScaler(&pScaler);
                            
                            if (SUCCEEDED(hr)) {
                                hr = pScaler->Initialize(pSource, width, height,
                                                         WICBitmapInterpolationModeCubic);
                            }
                            
                            if (SUCCEEDED(hr))
                                hr = pConverter->Initialize(pScaler, GUID_WICPixelFormat32bppPBGRA,
                                                            WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeMedianCut);
                        }
                    } else {
                        hr = pConverter->Initialize(
                                 pSource,
                                 GUID_WICPixelFormat32bppPBGRA,
                                 WICBitmapDitherTypeNone,
                                 nullptr,
                                 0.f,
                                 WICBitmapPaletteTypeMedianCut);
                    }
                }
                
                if (SUCCEEDED(hr)) {
                    hr = _pMainRT->CreateBitmapFromWicBitmap(pConverter, nullptr, &pBitmap);
                }
                
                SafeRelease(&pDecoder);
                SafeRelease(&pSource);
                SafeRelease(&pStream);
                SafeRelease(&pConverter);
                SafeRelease(&pScaler);
                
                return pBitmap;
            }
            
            bool switchCompatibleMode(int w, int h) {
                _mapBitmaps.clear();
                ID2D1BitmapRenderTarget* CRT = NULL;
                HRESULT hr = GetMainRT()->CreateCompatibleRenderTarget(
                                 D2D1::SizeF(w, h), &CRT);
                                 
                if (!SUCCEEDED(hr)) {
                    return false;
                }
                
                _pMainRT = CRT;
                m_bCompatibleMode = true;
                return true;
                /*ID2D1BitmapRenderTarget* target = dynamic_cast<ID2D1BitmapRenderTarget*>
                (CRT);*/
            }
            
        protected:
            GuiStyle				Styles;
            
            ImFloat4				_bgColor;
            float					_penWidth;
            
            ID2D1Factory*			_pD2DFactory;
            IDWriteFactory*			_pDWriteFactory;
            IWICImagingFactory*		_pWICFactory;
            ID2D1RenderTarget*	_pMainRT;
            
            //ID2D1DCRenderTarget *dcRenderTarget;
            ID2D1SolidColorBrush* gBrush = nullptr;
            
            IDWriteTextFormat*		_pTextFormat;
            ID2D1SolidColorBrush*	_pCommonBrush;
            
            std::unordered_map<std::string, ID2D1Bitmap*> _mapBitmaps;
            
            bool m_bCompatibleMode;
            
            
            
        };
        
        
        class D2D1Renderer : public D2D1CompatibleRender {
        public:
            D2D1Renderer() {
            
            }
            
            ~D2D1Renderer() {
            
                SafeRelease(&_pCommonBrush);
                SafeRelease(&_pTextFormat);
                DestroyResources();
            }
            void DestroyResources() {
                SafeRelease(&_pD2DFactory);
                SafeRelease(&_pDWriteFactory);
                SafeRelease(&_pWICFactory);
                
                
                CoUninitialize();
            }
            
            virtual bool Init() {
                CreateDeviceIndependentResources();
                CreateDeviceResources();
                
                _pMainRT->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
                
                HRESULT hr = S_OK;
                
                if (SUCCEEDED(hr)) {
                    hr = _pDWriteFactory->CreateTextFormat(
                             Styles.FontName,
                             NULL,
                             DWRITE_FONT_WEIGHT_REGULAR,
                             DWRITE_FONT_STYLE_NORMAL,
                             DWRITE_FONT_STRETCH_NORMAL,
                             Styles.FontSize,
                             L"en-us",
                             &_pTextFormat);
                             
                    _pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                    _pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                    _pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
                }
                
                if (SUCCEEDED(hr)) {
                    hr = _pMainRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Orange),
                                                         &_pCommonBrush);
                }
                
                return false;
            }
            
            std::shared_ptr<D2D1CompatibleRender> createCompatibleRenderer(int w, int h) {
                std::shared_ptr<D2D1CompatibleRender> renderer(new D2D1CompatibleRender(*this));
                renderer->switchCompatibleMode(w, h);
                return renderer;
                
                ID2D1BitmapRenderTarget* CRT = NULL;
                HRESULT hr = GetMainRT()->CreateCompatibleRenderTarget(
                                 D2D1::SizeF(w, h), &CRT);
                                 
                if (!SUCCEEDED(hr)) return{};
                
                ID2D1RenderTarget* ptr = CRT;
                
                ID2D1BitmapRenderTarget* target = dynamic_cast<ID2D1BitmapRenderTarget*>
                                                  (CRT);
                                                  
                renderer->SetMainRT(CRT);
                
                return renderer;
                
            }
            
        };
        
        
        
        
        class D2D1HWNDRenderer : public D2D1Renderer {
        public:
            D2D1HWNDRenderer() {
                m_hWnd = NULL;
                _pHWNDTarget = NULL;
            }
            
            D2D1HWNDRenderer(HWND hWnd) {
                m_hWnd = hWnd;
                _pHWNDTarget = NULL;
            }
            
            ~D2D1HWNDRenderer() {
            
            }
            
            void SetHWND(HWND hWnd) {
                m_hWnd = hWnd;
                
            }
            
            virtual HRESULT CreateDeviceResources() {
                HRESULT hr = S_OK;
                
                if (!_pHWNDTarget) {
                    RECT rc;
                    GetClientRect(m_hWnd, &rc);
                    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
                    hr = _pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
                            D2D1::HwndRenderTargetProperties(m_hWnd, size), &_pHWNDTarget);
                    _pMainRT = _pHWNDTarget;
                }
                
                return hr;
            }
            
            
            virtual bool Init() override {
                return D2D1Renderer::Init();
            }
            
            
            virtual void Resize(float w, float h) override {
                if (_pHWNDTarget != NULL) {
                    _pHWNDTarget->Resize(D2D1::SizeU(w, h));
                }
            }
            
        private:
            ID2D1HwndRenderTarget*	_pHWNDTarget;
            HWND m_hWnd;
        };
        
        
        
        class D2D1DCRenderer : public D2D1Renderer {
        public:
            D2D1DCRenderer() {
                m_HDC = NULL;
                _pDCRenderTarget = NULL;
            }
            
            D2D1DCRenderer(HDC hdc) {
                m_HDC = hdc;
                _pDCRenderTarget = NULL;
            }
            
            ~D2D1DCRenderer() {
            
            }
            
            void BindHDC(HDC hdc, const RECT& rt) {
                m_HDC = hdc;
                _pDCRenderTarget->BindDC(m_HDC, &rt);
                
            }
            
            virtual HRESULT CreateDeviceResources() {
                HRESULT hr = S_OK;
                
                if (!_pDCRenderTarget) {
                
                    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
                            D2D1_RENDER_TARGET_TYPE_DEFAULT,
                            D2D1::PixelFormat(
                                DXGI_FORMAT_B8G8R8A8_UNORM,
                                D2D1_ALPHA_MODE_PREMULTIPLIED),
                            0,
                            0,
                            D2D1_RENDER_TARGET_USAGE_NONE,
                            D2D1_FEATURE_LEVEL_DEFAULT);
                    auto props2 = D2D1::RenderTargetProperties();
                    hr = _pD2DFactory->CreateDCRenderTarget(&props, &_pDCRenderTarget);
                    /*   _pDCRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red),
                                                               &gBrush);*/
                    _pMainRT = _pDCRenderTarget;
                }
                
                
                return hr;
            }
            
            
            virtual bool Init() override {
                return D2D1Renderer::Init();
            }
            
            
            virtual void Resize(float w, float h) override {
                RECT rt = { 0, 0, (LONG)w, (LONG)h };
                _pDCRenderTarget->BindDC(m_HDC, &rt);
                return;
            }
            
        private:
            ID2D1DCRenderTarget*	_pDCRenderTarget;
            HDC m_HDC;
        };
    }
}



#endif // AMO_D2DRENDER_75ED5962_D439_4908_850B_7689FAD3C510_H__

