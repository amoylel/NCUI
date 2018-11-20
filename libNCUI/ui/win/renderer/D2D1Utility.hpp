// Created by amoylel on 28/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef LIBNCUI_D2D1UTILITY_182DC6F5_AE57_4A5B_89E0_4BB2AF60C749_HPP__
#define LIBNCUI_D2D1UTILITY_182DC6F5_AE57_4A5B_89E0_4BB2AF60C749_HPP__
#include <d2d1.h>
#pragma comment(lib, "D2D1.lib")
#pragma comment(lib, "DWrite.lib")

namespace amo {
    namespace d2d1 {
    
    
    
        typedef unsigned int ImDuiWindowFlags;
        typedef unsigned int ImUint;
        
        
        struct ImFloat2 {
            float x, y;
            ImFloat2() {
                x = y = 0.0f;
            }
            ImFloat2(float _x, float _y) {
                x = _x;
                y = _y;
            }
            
            ImFloat2 operator*(const float rhs) {
                return ImFloat2(x * rhs, y * rhs);
            }
            ImFloat2 operator/(const float rhs) {
                return ImFloat2(x / rhs, y / rhs);
            }
            ImFloat2 operator+(const ImFloat2& rhs) const {
                return ImFloat2(x + rhs.x, y + rhs.y);
            }
            ImFloat2 operator-(const ImFloat2& rhs) const {
                return ImFloat2(x - rhs.x, y - rhs.y);
            }
            ImFloat2 operator*(const ImFloat2 rhs) const {
                return ImFloat2(x * rhs.x, y * rhs.y);
            }
            ImFloat2 operator/(const ImFloat2 rhs) const {
                return ImFloat2(x / rhs.x, y / rhs.y);
            }
            ImFloat2& operator+=(const ImFloat2& rhs) {
                x += rhs.x;
                y += rhs.y;
                return *this;
            }
            ImFloat2& operator-=(const ImFloat2& rhs) {
                x -= rhs.x;
                y -= rhs.y;
                return *this;
            }
            ImFloat2& operator*=(const float rhs) {
                x *= rhs;
                y *= rhs;
                return *this;
            }
            ImFloat2& operator/=(const float rhs) {
                x /= rhs;
                y /= rhs;
                return *this;
            }
            
            D2D1_POINT_2F ToD2DPointF() const {
                return D2D1::Point2F(x, y);
            }
            D2D1_SIZE_F ToD2DSizeF() const {
                return D2D1::SizeF(x, y);
            }
        };
        
        struct ImFloat4 {
            float x, y, z, w;
            ImFloat4() {
                x = y = z = w = 0.0f;
            }
            ImFloat4(float _x, float _y, float _z, float _w) {
                x = _x;
                y = _y;
                z = _z;
                w = _w;
            }
            ImFloat4(const ImFloat2& _pos, const ImFloat2& _size) {
                x = _pos.x;
                y = _pos.y;
                z = _size.x;
                w = _size.y;
            }
            
            D2D1_RECT_F ToD2DRectF() const {
                return D2D1::RectF(x, y, x + z, y + w);
            }
            D2D_COLOR_F ToD2DColorF() const {
                return D2D1::ColorF(x, y, z, w);
            }
        };
        
        static ImFloat4 ImHexToRGBA(ImUint rgb) {
            static const ImUint sc_redShift = 16;
            static const ImUint sc_greenShift = 8;
            static const ImUint sc_blueShift = 0;
            
            static const ImUint sc_redMask = 0xff << sc_redShift;
            static const ImUint sc_greenMask = 0xff << sc_greenShift;
            static const ImUint sc_blueMask = 0xff << sc_blueShift;
            
            ImFloat4 c;
            c.x = static_cast<float>((rgb & sc_redMask) >> sc_redShift) / 255.f;
            c.y = static_cast<float>((rgb & sc_greenMask) >> sc_greenShift) / 255.f;
            c.z = static_cast<float>((rgb & sc_blueMask) >> sc_blueShift) / 255.f;
            c.w = 1.f;
            
            return c;
        }
        
        
        enum GuiStyleColor_ {
            Color_Text,
            Color_Border,
            Color_WindowBg,
            Color_WidgetBg,
            Color_WidgetActive,
            Color_TitleBar,
            Color_TitleBarCollapsed,
            Color_Slider,
            Color_SliderActive,
            Color_Button,
            Color_ButtonHovered,
            Color_ButtonActive,
            Color_Collapse,
            Color_CollapseHovered,
            Color_CollapseActive,
            Color_ResizeGrip,
            Color_ResizeGripHovered,
            Color_ResizeGripActive,
            Color_TooltipBg,
            Color_COUNT,
        };
        
        
        struct GuiStyle {
            wchar_t*		FontName;
            float			FontSize;
            float			StrokeWidth;
            float			DefaultWindowAlpha;
            ImFloat2		WindowPadding;
            ImFloat2		WindowMinSize;
            ImFloat2		FramePadding;
            ImFloat2		ItemSpacing;
            ImFloat2		ItemInnerSpacing;
            float			WindowRounding;
            float			TitleBarHeight;
            ImFloat2		ResizeGripSize;
            ImFloat4		Colors[Color_COUNT];
            
            GuiStyle() {
                FontName = L"Arial";
                FontSize = 12.f;
                StrokeWidth = 1.f;
                
                DefaultWindowAlpha = 1.0f;
                WindowPadding = ImFloat2(8, 8);
                WindowMinSize = ImFloat2(48, 48);
                FramePadding = ImFloat2(5, 4);
                ItemSpacing = ImFloat2(10, 5);
                ItemInnerSpacing = ImFloat2(5, 5);
                TitleBarHeight = 20.f;
                ResizeGripSize = ImFloat2(30, 30);
                
                Colors[Color_Text] = ImHexToRGBA(0x000000);
                Colors[Color_Border] = ImHexToRGBA(0xFF00FF);
                Colors[Color_WindowBg] = ImHexToRGBA(0xDCDCDC);
                Colors[Color_WidgetBg] = ImHexToRGBA(0xFFFFFF);
                Colors[Color_WidgetActive] = ImHexToRGBA(0x4296FA);
                Colors[Color_TitleBar] = ImHexToRGBA(0x7BB4F7);
                Colors[Color_TitleBarCollapsed] = ImHexToRGBA(0xA0C8FF);
                Colors[Color_Slider] = ImHexToRGBA(0x64B4FF);
                Colors[Color_SliderActive] = ImHexToRGBA(0x4296FA);
                Colors[Color_Button] = ImHexToRGBA(0x9FA8DA);
                Colors[Color_ButtonHovered] = ImHexToRGBA(0xB2B4D2);
                Colors[Color_ButtonActive] = ImHexToRGBA(0x8B94C8);
                Colors[Color_Collapse] = ImHexToRGBA(0x9FA8DA);
                Colors[Color_CollapseHovered] = ImHexToRGBA(0xB2B4D2);
                Colors[Color_CollapseActive] = ImHexToRGBA(0x8B94C8);
                Colors[Color_ResizeGrip] = ImHexToRGBA(0x95A5A6);
                Colors[Color_ResizeGripHovered] = ImHexToRGBA(0xBDC3C7);
                Colors[Color_ResizeGripActive] = ImHexToRGBA(0xABB7B7);
                Colors[Color_TooltipBg] = ImHexToRGBA(0xD8FFC4);
            }
        };
        
        
        template<class Interface>
        inline void SafeRelease(Interface **ppInterfaceToRelease) {
            if (*ppInterfaceToRelease != NULL) {
                (*ppInterfaceToRelease)->Release();
                
                (*ppInterfaceToRelease) = NULL;
            }
        }
        
    }
}



#endif //LIBNCUI_D2D1UTILITY_182DC6F5_AE57_4A5B_89E0_4BB2AF60C749_HPP__