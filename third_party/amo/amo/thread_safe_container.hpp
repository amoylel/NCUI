// Created by amoylel on 22/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_THREAD_SAFE_CONTAINER_259F94FD_044A_46C9_BB58_A6463233E662_HPP__
#define AMO_THREAD_SAFE_CONTAINER_259F94FD_044A_46C9_BB58_A6463233E662_HPP__





#include <map>
#include <unordered_map>
#include <set>
#include <mutex>

#include <amo/mutex.hpp>
namespace amo {

    template<typename T>
    class thread_safe_container {
    public:
        typedef typename  T::value_compare value_compare;
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
        
        
        void lock() {
            return m_mutex.lock();
        }
        
        void unlock() {
            return m_mutex.unlock();
        }
        
    public:
        T m_container;
        std::recursive_mutex m_mutex;
    };
    
    template<typename K, typename T, typename Container = std::map<K, T> >
    class thread_safe_map : public thread_safe_container<Container > {
    public:
        typedef K key_type;
        typedef T mapped_type;
    public:
        thread_safe_map() {
        
        }
        
        
        bool empty() const  {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            return m_container.empty();
        }
        
        size_type size() const  {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            return m_container.size();
        }
        
        size_type max_size() const  {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            return m_container.max_size();
        }
        
        // iterators
        // iter 遍历需要自己上锁
        iterator begin()   {
            return m_container.begin();
        }
        
        const_iterator begin() const  {
            return m_container.begin();
        }
        
        iterator end()  {
            return m_container.end();
        }
        
        const_iterator end() const  {
            return m_container.end();
        }
        
        const_iterator cbegin() const  {
            return m_container.cbegin();
        }
        
        const_iterator cend() const  {
            return m_container.cend();
        }
        
        std::pair<iterator, bool> insert(value_type const& x) {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            return m_container.insert(x);
        }
        
        
        iterator insert(const_iterator hint, value_type const& x) {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            return m_container.insert(hint, x);
        }
        
        
        iterator erase(const_iterator iter) {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            return m_container.erase(iter);
        }
        size_type erase(const key_type& key) {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            return m_container.erase(key);
        }
        
        //#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)
        //        void insert(std::initializer_list<value_type>);
        //#endif
        //
        
        iterator erase(const_iterator begin_iter, const_iterator end_iter) {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            return m_container.erase(begin_iter, end_iter);
        }
        
        
        void clear() {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            m_container.clear();
        }
        
        mapped_type& operator[](const key_type& key) {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            return m_container[key];
        }
        mapped_type& at(const key_type& key) {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            return m_container.at(key);
        }
        mapped_type const& at(const key_type& key) const {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            return m_container.at(key);
        }
        
        // lookup
        
        iterator find(const key_type& key) {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            return m_container.find(key);
        }
        
        const_iterator find(const key_type& key) const {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            return m_container.find(key);
        }
        
        
        size_type count(const key_type&  key) const {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            return m_container.count(key);
        }
        
    };
    
    
}


#endif //AMO_THREAD_SAFE_CONTAINER_259F94FD_044A_46C9_BB58_A6463233E662_HPP__