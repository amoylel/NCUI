// Created by amoylel on 26/02/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_RECT_DFF827D9_B233_4647_B49E_B1C8D4B4AF45_HPP__
#define AMO_RECT_DFF827D9_B233_4647_B49E_B1C8D4B4AF45_HPP__

#include <stdint.h>
#include <amo/config.hpp>
#ifdef OS_WIN
#include <WinSock2.h>
#include <windows.h>
#endif // OS_WIN



namespace amo {

    class ltrm {
    public:
        ltrm() {
            left = top = right = bottom = 0;
        }
        ltrm(int32_t left_, int32_t top_, int32_t right_, int32_t bottom_)
            : left(left_)
            , top(top_)
            , right(right_)
            , bottom(bottom_) {
            
        }
    public:
        int32_t    left;
        int32_t    top;
        int32_t    right;
        int32_t    bottom;
    };
    
    class rect {
    
    
    public:
        rect() {
            m_left = m_top = m_right = m_bottom = 0;
        }
        
        rect(const rect& rhs) {
            memcpy(this, &rhs, sizeof rect);
        }
        
        /* rect(int32_t left_, int32_t top_, int32_t right_, int32_t bottom_)
             : m_left(left_)
             , m_top(top_)
             , m_right(right_)
             , m_bottom(bottom_) {
        
         }*/
        
        rect(int32_t left_, int32_t top_, int32_t width_, int32_t height_)
            : m_left(left_)
            , m_top(top_)
            , m_right(left_ + width_)
            , m_bottom(top_ + height_) {
            
        }
        
#ifdef OS_WIN
        rect(const RECT& rt)
            : m_left(rt.left)
            , m_top(rt.top)
            , m_right(rt.right)
            , m_bottom(rt.bottom) {
            
        }
        
        operator RECT() const {
            RECT rt = { m_left, m_top, m_right, m_bottom };
            return rt;
        }
        
#endif // OS_WIN
        rect(const ltrm& rt)
            : m_left(rt.left)
            , m_top(rt.top)
            , m_right(rt.right)
            , m_bottom(rt.bottom) {
            
        }
        
        operator ltrm() const {
            ltrm rt = { m_left, m_top, m_right, m_bottom };
            return rt;
        }
        
        bool operator ==(const rect& rhs) const {
            return m_left == rhs.m_left
                   && m_top == rhs.m_top
                   && m_bottom == rhs.m_bottom
                   && m_right == rhs.m_right;
        }
        
        bool operator !=(const rect& rhs) const {
            return !this->operator ==(rhs);
        }
        
        int32_t width() const {
            return m_right - m_left;
        }
        
        void width(int32_t val) {
            m_right = m_left + val;
        }
        
        int32_t height() const {
            return m_bottom - m_top;
        }
        
        void height(int32_t val) {
            m_bottom = m_top + val;
        }
        
        int32_t left() const {
            return m_left;
        }
        
        void left(int32_t val) {
            m_left = val;
        }
        
        int32_t top() const {
            return m_top;
        }
        
        void top(int32_t val) {
            m_top = val;
        }
        
        int32_t right() const {
            return m_right;
        }
        
        void right(int32_t val) {
            m_right = val;
        }
        
        int32_t bottom() const {
            return m_bottom;
        }
        
        void bottom(int32_t val) {
            m_bottom = val;
        }
        
        bool empty() const {
            return width() == 0 && height() == 0;
        }
        
        int32_t size() const {
            return width() * height();
        }
        
        amo::rect intersect(const amo::rect& rhs) const {
        
            if (this->empty() || rhs.empty()) {
                return amo::rect();
            }
            
            //  x 在另一个矩形的r点内&& y 在另一个矩形内，说明有交集
            // 先判断X轴是否有交集
            if (!(m_left < rhs.m_right && m_right > rhs.m_left)) {
                return amo::rect();
            }
            
            // 判断Y轴是否有交集
            if (!(m_top < rhs.m_bottom && m_bottom > rhs.m_top)) {
                return amo::rect();
            }
            
            // 交集就好办了，取两个left, top的最大值 ，两个right,bottom的最小值
            amo::rect rc(rhs);
            
            if (m_left > rhs.m_left) {
                rc.left(m_left);
            }
            
            if (m_top > rhs.m_top) {
                rc.top(m_top);
            }
            
            if (m_right < rhs.m_right) {
                rc.right(m_right);
            }
            
            if (m_bottom < rhs.m_bottom) {
                rc.bottom(m_bottom);
            }
            
            return rc;
        }
        
        /**
         * @fn	std::vector<amo::rect> rect::complement(const amo::rect& subrect) const
         *
         * @brief 求被集，反回所有不包含所给rect的其他rect,
         * 		  所给定的rect必须是当前rect的真子集
         *
         * @param	subrect	The subrect.
         *
         * @return	A std::vector&lt;amo::rect&gt;
         */
        
        std::vector<amo::rect> complement(const amo::rect& subrect, bool contains_self = false) const {
            std::vector<amo::rect> vec;
            
            if (this->empty() || subrect.empty()) {
                return vec;
            }
            
            // 所给的rect必须是当前rect的子集
            if (intersect(subrect) != subrect) {
                return vec;
            }
            
            //
            if (this->operator==(subrect)) {
                return vec;
            }
            
            // 最好的情况应该有八个rect
            // 1x 1y 2x 2y
            // 2x 1y 2r 2y
            // 2r 1y 1r 2y
            
            // 1x 2y 2x 2b
            // 2x 2y 2r 2b // 这是子集坐标，不要
            // 2r 2y 1r 2b
            
            // 1x 2b 2x 1b
            // 2x 2b 2r 1b
            // 2r 2b 1r 1b
            
            vec.push_back(amo::ltrm{ m_left, m_top, subrect.m_left, subrect.m_top });
            vec.push_back(amo::ltrm{ subrect.m_left, m_top, subrect.m_right, subrect.m_top });
            vec.push_back(amo::ltrm{ subrect.m_right, m_top, m_right, subrect.m_top });
            
            vec.push_back(amo::ltrm{ m_left, subrect.m_top, subrect.m_left, subrect.m_bottom });
            
            if (contains_self) {
                vec.push_back(amo::ltrm{ subrect.m_left, subrect.m_top, subrect.m_right, subrect.m_bottom });// 这是子集坐标，不要
            }
            
            vec.push_back(amo::ltrm{ subrect.m_right, subrect.m_top, m_right, subrect.m_bottom });
            
            vec.push_back(amo::ltrm{ m_left, subrect.m_bottom, subrect.m_left, m_bottom });
            vec.push_back(amo::ltrm{ subrect.m_left, subrect.m_bottom, subrect.m_right, m_bottom, });
            vec.push_back(amo::ltrm{ subrect.m_right, subrect.m_bottom, m_right, m_bottom });
            
            return vec;
        }
        
        /**
         * @fn	amo::rect rect::get_full_rect(const amo::rect& _rect)
         *
         * @brief	获取所给区域在当前区域的最大原比例显示区域 .
         *
         * @param	_rect	The rectangle.
         *
         * @return	The full rectangle.
         */
        
        amo::rect get_full_rect(const amo::rect& rhs) {
        
            amo::rect window_rect = *this;
            amo::rect texture_rect = rhs;
            
            if (texture_rect.width() == 0
                    || texture_rect.height() == 0
                    || window_rect.width() == 0
                    || window_rect.height() == 0) {
                return window_rect;
            }
            
            
            // 保证window rect 比texture_rect 大
            const int64_t enlarge_number = 10000;
            int64_t window_width = window_rect.width() * enlarge_number;
            int64_t window_height = window_rect.height() * enlarge_number;
            int64_t texture_width = texture_rect.width();
            int64_t texture_height = texture_rect.height();
            
            double width_rate = window_width / (double)texture_width;
            double height_rate = window_height / (double)texture_height;
            bool width_first = true;
            
            if (width_rate > height_rate) {
                width_first = false;
            }
            
            //X轴占满屏幕
            double rate = (double)window_width / texture_width;
            int64_t width = window_width;
            int64_t height = texture_height * rate;
            int64_t left = 0;
            int64_t top = window_height / 2 - height / 2;
            top /= enlarge_number;
            
            if (!width_first) {	//X轴占满屏幕
                double rate = window_height / texture_height;
                height = window_height;
                width = texture_width * rate;
                top = 0;
                left = window_width / 2 - width / 2;
                left /= enlarge_number;
            }
            
            width += (enlarge_number / 2);
            height += (enlarge_number / 2);
            width /= enlarge_number;
            height /= enlarge_number;
            
            //居中
            return amo::rect(left + window_rect.left(),
                             top + window_rect.top(),
                             width,
                             height);
        }
        
        // 将区域分割为九个部分
        std::vector < amo::rect> split(const amo::rect& rc) const {
            amo::rect rect =  this->intersect(rc);
            return complement(rect, true);
        }
        
        // 平均分为九个部分
        std::vector < amo::rect> split() const {
            int32_t x1 = width() / 3;
            int32_t y1 = height() / 3;
            
            return split(amo::rect(x1, y1, x1, y1));
        }
    private:
        int32_t    m_left;
        int32_t    m_top;
        int32_t    m_right;
        int32_t    m_bottom;
    };
}



#endif //AMO_RECT_DFF827D9_B233_4647_B49E_B1C8D4B4AF45_HPP__