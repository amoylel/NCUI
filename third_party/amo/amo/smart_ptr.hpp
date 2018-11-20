// Created by 苏元海 on 02/15/2016.
// Copyright (c) 2016 amoylel. All rights reserved.

#ifndef IME_SMART_PTR_HPP__
#define IME_SMART_PTR_HPP__

#include <amo/config.hpp>
#include <amo/memory.hpp>


#ifdef BOOST_NO_CXX11_SMART_PTR
#include <amo/config.hpp>
#else
#include <memory>
#endif
namespace amo {
#ifdef BOOST_NO_CXX11_SMART_PTR
    using boost::shared_ptr;
    using boost::scoped_ptr;
    using boost::intrusive_ptr;
    using boost::weak_ptr;
#else
    using std::shared_ptr;
    /*   using std::scoped_ptr;
       using std::intrusive_ptr;*/
    using std::weak_ptr;
#endif
}



#endif // IME_SMART_PTR_HPP__
