// Created by amoylel on 05/17/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NODEPROCESSEXCHANGERMANAGER_HPP__
#define AMO_NODEPROCESSEXCHANGERMANAGER_HPP__

#include <amo/singleton.hpp>
#include "ipc/ProcessExchanger.hpp"

namespace amo {
    class NodeProcessExchangerManager : public ProcessExchangerManager < ProcessExchanger >
        , public amo::singleton < NodeProcessExchangerManager > {
    public:
        NodeProcessExchangerManager() {
            m_nN = 3;
        }
        
    };
    
}

#endif // AMO_NODEPROCESSEXCHANGERMANAGER_HPP__




