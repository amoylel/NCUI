// Created by amoylel on 22/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_SAFE_MAP_7AB4D75D_B378_4B42_A1B7_C44928F81D89_HPP__
#define AMO_SAFE_MAP_7AB4D75D_B378_4B42_A1B7_C44928F81D89_HPP__

#include <map>
#include <unordered_map>
#include <set>
namespace amo {

    template<typename T>
    class thread_safe_container {
    public:
        typedef typename T::value_compare value_compare;
        typedef typename  T::allocator_type allocator_type;
        typedef typename  T::size_type size_type;
        typedef typename  T::difference_type difference_type;
        typedef typename  T::pointer pointer;
        typedef typename  T::const_pointer const_pointer;
        typedef typename  T::reference reference;
        typedef typename  T::const_reference const_reference;
        typedef typename  T::iterator iterator;
        typedef typename  T::const_iterator const_iterator;
        typedef typename  T::value_type value_type;
        
    public:
        thread_safe_container() {
        
        }
        
        
        thread_safe_container
        
    private:
        T map;
    };
}



#endif //AMO_SAFE_MAP_7AB4D75D_B378_4B42_A1B7_C44928F81D89_HPP__