// Created by amoylel on 21/09/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef LIBNCUI_D2D1VIEWRENDERER_094266D4_B6A1_4B24_AC35_413B3992F271_H__
#define LIBNCUI_D2D1VIEWRENDERER_094266D4_B6A1_4B24_AC35_413B3992F271_H__

 

#ifndef NCUI_NO_D2D1
 
#include <memory>
#include <vector>
#include "ui/win/WinUserMsg.hpp"
#include "ui/win/overlap/PaintResource.hpp"
#include "ui/win/renderer/D2D1Render.hpp"

 
namespace amo {
     
    
    
    //页面渲染控件
    class ViewRenderer : public CControlUI {
    
    public:
        ViewRenderer();
        virtual ~ViewRenderer();
        LPCTSTR GetClass() const override;
        virtual void PaintStatusImage(HDC hDC) override;
        
        virtual void insertBitmap(std::shared_ptr<PaintResource> image);
        
        virtual void SetToolTip(LPCTSTR pstrText) override;
        
        virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;
        
        
        virtual void DoInit() override;
        
        
        
        bool isAccelerator() const {
            return m_accelerator;
        }
        void setAccelerator(bool val) {
            m_accelerator = val;
        }
        
        void drawBackground(bool bDraw = true);
        
        
        void PaintBitmap(HWND hWnd);
        
        void PaintBitmapWithHWND(HWND hWnd);
        
        void PaintBitmapWithHDC(HDC hDC, bool releaseRenderer = true);
        
        void PaintBitmapWithGDI(HDC hDC);
        
        std::shared_ptr<amo::NativeWindowSettings> getPaintSettings() const {
            return m_paintSettings;
        }
        void setPaintSettings(std::shared_ptr<amo::NativeWindowSettings> val) {
            m_paintSettings = val;
        }
        
    protected:
        void updateFPS() {
            int64_t timestamp = amo::timer::now<amo::chrono::seconds>();
            
            if (timestamp > m_nLastTimestamp) {
                m_nLastTimestamp = timestamp;
                $cdevel("fps : {}", m_nCount);
                m_nLastFPS = m_nCount;
                m_nCount = 0;
            }
            
            ++m_nCount;
        }
        int getFPS() {
            return  m_nLastFPS;
        }
        
    protected:
    
        std::shared_ptr<PaintResource> m_resource;
      
        
        std::shared_ptr<amo::d2d1::D2D1HWNDRenderer> m_hwndRenderer;
        
        std::shared_ptr<amo::d2d1::D2D1DCRenderer> m_dcRenderer;
        
        
        /*! @brief	是否使用硬件加速. */
        bool m_accelerator;
        
        int m_nCount;	// 最近一秒内渲染的帧数
        int m_nLastFPS;	// 最近一次有记录的FPS
        
        int64_t m_nLastTimestamp;  // 最近一次渲染时间 秒
        
        bool m_bDrawBackground;
        
        std::shared_ptr<NativeWindowSettings> m_paintSettings;
    };
    
    class LayerViewRender : public ViewRenderer {
    public:
        virtual ~LayerViewRender();
        virtual void insertBitmap(std::shared_ptr<PaintResource> image) override;
        
    };
    
}
#endif // NCUI_NO_D2D1

#endif //LIBNCUI_D2D1VIEWRENDERER_094266D4_B6A1_4B24_AC35_413B3992F271_H__