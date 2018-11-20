// Created by amoylel on 11/18/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_BIGSTR_CE96EF2D_D54E_45CF_B362_60F407B221D5_HPP__
#define AMO_BIGSTR_CE96EF2D_D54E_45CF_B362_60F407B221D5_HPP__

#include <amo/blob.hpp>

namespace amo {
    class bigstr : public amo::file_mapping_data {
    public:
        bigstr(const std::string& name, int32_t size,
               int64_t delay_destroy = 10000) : file_mapping_data(name, size,
                           delay_destroy) {
                           
                           
        }
        
        bigstr(const std::string& name): file_mapping_data(name) {
        
        }
        
    };
}

#endif // AMO_BIGSTR_CE96EF2D_D54E_45CF_B362_60F407B221D5_HPP__

