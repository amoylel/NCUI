// Created by amoylel on 08/19/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_PAINTRESOURCE_11000264_4503_4123_BCBC_935E7B3630C6_HPP__
#define AMO_PAINTRESOURCE_11000264_4503_4123_BCBC_935E7B3630C6_HPP__

#include <vector>
#include <memory>
#include "ui/win/overlap/Overlap.hpp"
#include <amo/rect.hpp>



namespace amo {

    class PaintResource {
    public:
        PaintResource() {
        
        }
        
        bool addOverlap(std::shared_ptr<Overlap> ptr) {
            overlaps.push_back(ptr);
            return true;
        }
        
        std::vector<std::shared_ptr<Overlap> > overlaps;
        
        void setPos(const amo::rect& rect) {
            m_rect = rect;
        }
        
        amo::rect getPos() {
            return m_rect;
        }
        
        amo::rect m_rect;
        
    };
}

#endif // AMO_PAINTRESOURCE_11000264_4503_4123_BCBC_935E7B3630C6_HPP__

