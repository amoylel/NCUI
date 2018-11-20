// Created by amoylel on 08/15/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_OVERLAPSETTINGS_4D3BB806_6E0C_41DC_A8F6_C33E343EACDE_H__
#define AMO_OVERLAPSETTINGS_4D3BB806_6E0C_41DC_A8F6_C33E343EACDE_H__
#include "settings/BasicSettings.h"
#include <amo/rect.hpp>

namespace amo {

    class OverlapRect {
    public:
        OverlapRect() {
            index = 5;
            renderMode = 0;
        }
        OverlapRect(const amo::rect& src_, const amo::rect& dst_, int index_,
                    int renderMode_) {
            index = index_;
            src = src_;
            dst = dst_;
            renderMode = renderMode_;
        }
        
        int index;
        amo::rect src;
        amo::rect dst;
        int renderMode;  // 0 平铺 1 居中 2 拉伸
    };
    
    class OverlapRegions {
    public:
        std::vector<OverlapRect> m_regions;
    };
    
    class OverlapSettings : public BasicSettings {
    
    
    public:
        OverlapSettings();
        ~OverlapSettings();
        
        
        /*!
        * @fn	void BrowserWindowSettings::InitDefaultBrowserSettings();
        *
        * @ignore
        *
        * @brief	初始化默认启动画面参数.
        */
        
        void initDefaultOverlapSettings();
        
        virtual void afterUpdateArgsSettings() override;
        
        virtual amo::u8json toJson() override;
        
        void setCanvasRect(const amo::rect& rt) {
            canvasRect = rt;
            
            if (!regions) {
                defaultRegions.reset(new OverlapRegions());
                OverlapRect rect;
                rect.index = index;
                rect.renderMode = renderMode;
                rect.dst = canvasRect;
                rect.src = amo::rect(0, 0, width, height);
                defaultRegions->m_regions.push_back(rect);
            }
            
        }
        amo::rect getCanvasRect() const {
            return canvasRect;
        }
        
        std::shared_ptr<OverlapRegions> getOverlapRegions() {
            if (regions) {
                return regions;
            }
            
            if (!defaultRegions) {
                defaultRegions.reset(new OverlapRegions());
                OverlapRect rect;
                rect.index = index;
                rect.renderMode = renderMode;
                rect.dst = amo::rect();
                rect.src = amo::rect(0, 0, width, height);
                defaultRegions->m_regions.push_back(rect);
            }
            
            return defaultRegions;
        }
    protected:
        void updateRectSettings(const std::string& name,
                                std::shared_ptr<OverlapRegions>& ptr);
    public:
        /*! @var #String name 图层名称. */
        std::string name;
        /*! @var #Int width 图层宽度. */
        int width;
        /*! @var #Int	imageHeight 图层高度. */
        int height;
        
        int step;	// ？？？
        
        /*! @var #Int=0 type 图层类型，0表示Bitmap. */
        int type;
        
        /*! @var #Int length 数据长度. */
        int length;
        
        /** @var #Int index 图层的渲染顺序，regions 不存在时有效. */
        int index;
        
        /** @var #Int renderMode=0 图层渲染模式，regions不存在时有效. */
        int renderMode;
        
        /*! @var #Rect	regions 图层区域. */
        std::shared_ptr<OverlapRegions> regions;
        
        /*! @var #RectArray	defaultRegions 默认图层区域，当前regions不存在时使用该值，由程序设置. */
        std::shared_ptr<OverlapRegions> defaultRegions;
        
        /*! @var #Rect	canvasRect 画布区域. */
        amo::rect canvasRect;
        
    };
}

#endif // AMO_OVERLAPSETTINGS_4D3BB806_6E0C_41DC_A8F6_C33E343EACDE_H__

