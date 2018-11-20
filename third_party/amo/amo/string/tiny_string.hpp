// Created by amoylel on 30/03/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_TINY_STRING_778A1115_C103_45C1_9FAA_F1854C658F89_HPP__
#define AMO_TINY_STRING_778A1115_C103_45C1_9FAA_F1854C658F89_HPP__

#include <string>

#include <amo/string/coding_type.hpp>

namespace amo {
    class tiny_string {
    public:
    
    public:
        typedef std::string::iterator iterator;
        typedef std::string::const_iterator const_iterator;
        typedef std::string::size_type size_type;
        typedef std::string::pointer pointer;
        typedef std::string::const_pointer const_pointer;
        typedef std::string::reference reference;
        typedef std::string::const_reference const_reference;
        typedef std::string::reverse_iterator reverse_iterator;
        typedef std::string::const_reverse_iterator const_reverse_iterator;
        typedef char _Elem;
        typedef std::string _Myt;
    public:
    
    public:
        operator std::string() {
            return  m_str;
        }
        
        operator std::string() const {
            return m_str;
        }
        
        
        std::string& str() {
            return m_str;
        }
        
        const std::string& str() const {
            return m_str;
        }
        
        iterator insert(iterator _Where, char _Ch) {
            return m_str.insert(_Where, _Ch);
        }
        
#ifdef _WIN32
        iterator insert(iterator _Where) {
            return m_str.insert(_Where);
        }
        
        
        iterator insert(iterator _Where, size_type _Count, _Elem _Ch) {
            return m_str.insert(_Where, _Count, _Ch);
        }
        
        iterator insert(iterator _Where, const_pointer _First, const_pointer _Last) {
            return m_str.insert(_Where, _First, _Last);
        }
        
        iterator insert(iterator _Where, iterator _First, iterator _Last) {
            return m_str.insert(_Where, _First, _Last);
        }
#endif
        
        
        
        void clear() {
            m_str.clear();
        }
        
        
        
        iterator begin() {
            return m_str.begin();
        }
        
        const_iterator begin() const {
            return m_str.begin();
        }
        
        iterator end() {
            return m_str.end();
        }
        
        const_iterator end() const {
            return m_str.end();
        }
        
        /*reverse_iterator rbegin()
        {
        return m_str.rbegin();
        }*/
        
        const_reverse_iterator rbegin() const {
            return m_str.rbegin();
        }
        
        reverse_iterator rend() {
            return m_str.rend();
        }
        
        const_reverse_iterator rend() const {
            return m_str.rend();
        }
        
        /*const_iterator cbegin() const
        {
        return m_str.cbegin();
        }*/
        
        /*const_iterator cend() const
        {
        return m_str.cend();
        }*/
        
        /*const_reverse_iterator crbegin() const
        {
        return m_str.crbegin();
        }*/
        
        /*const_reverse_iterator crend() const
        {
        return m_str.crend();
        }*/
        
        
        
        reference at(size_type _Off) {
            return m_str.at(_Off);
        }
        
        const_reference at(size_type _Off) const {
            return m_str.at(_Off);
        }
        
        reference operator[](size_type _Off) {
            return (m_str[_Off]);
        }
        
        const_reference operator[](size_type _Off) const {
            return (m_str[_Off]);
        }
        
        void push_back(_Elem _Ch) {
            m_str.push_back(_Ch);
        }
        
        
        
        /*void pop_back()
        {
        m_str.pop_back();
        }
        */
        /*reference front()
        {
        return m_str.front();
        }*/
        
        /*const_reference front() const
        {
        return m_str.front();
        }*/
        
        /*reference back()
        {
        return m_str.back();
        }*/
        
        /*const_reference back() const
        {
        return m_str.back();
        }*/
        
        const _Elem *c_str() const {
            return m_str.c_str();
        }
        
        _Elem *c_str() {
            return (char*)m_str.c_str();
        }
        
        const _Elem *data() const {
            return m_str.data();
        }
        
        _Elem *data() {
            return (char*)m_str.data();
        }
        size_type length() const {
            return m_str.length();
        }
        
        size_type size() const {
            return m_str.size();
        }
        
        size_type max_size() const {
            return m_str.max_size();
        }
        
        void resize(size_type _Newsize) {
            m_str.resize(_Newsize);
        }
        
        void resize(size_type _Newsize, _Elem _Ch) {
            m_str.resize(_Newsize, _Ch);
        }
        
        size_type capacity() const {
            return m_str.capacity();
        }
        
        void reserve(size_type _Newcap = 0) {
            m_str.reserve(_Newcap);
        }
        
        bool empty() const {
            return m_str.empty();
        }
        
        
        size_type copy(_Elem *_Ptr, size_type _Count, size_type _Off = 0) const {
            return m_str.copy(_Ptr, _Count, _Off);
        }
        
        void swap(_Myt& _Right) {
            m_str.swap(_Right);
        }
        
        bool contains(const _Myt& _Right) const {
            return find(_Right) != -1;
        }
        size_type find(const _Myt& _Right, size_type _Off = 0) const {
            return m_str.find(_Right, _Off);
        }
        
        size_type find(const _Elem *_Ptr, size_type _Off, size_type _Count) const {
            return m_str.find(_Ptr, _Off, _Count);
        }
        
        size_type find(const _Elem *_Ptr, size_type _Off = 0) const {
            return m_str.find(_Ptr, _Off);
        }
        
        size_type find(_Elem _Ch, size_type _Off = 0) const {
            return m_str.find(_Ch, _Off);
        }
        
        size_type rfind(const _Myt& _Right, size_type _Off = std::string::npos) const {
            return m_str.rfind(_Right, _Off);
        }
        
        size_type rfind(const _Elem *_Ptr, size_type _Off, size_type _Count) const {
            return m_str.rfind(_Ptr, _Off, _Count);
        }
        
        size_type rfind(const _Elem *_Ptr, size_type _Off = std::string::npos) const {
            return m_str.rfind(_Ptr, _Off);
        }
        
        size_type rfind(_Elem _Ch, size_type _Off = std::string::npos) const {
            return m_str.rfind(_Ch, _Off);
        }
        
        size_type find_first_of(const _Myt& _Right, size_type _Off = 0) const {
            return m_str.find_first_of(_Right, _Off);
        }
        
        size_type find_first_of(const _Elem *_Ptr, size_type _Off,
                                size_type _Count) const {
            return m_str.find_first_of(_Ptr, _Off, _Count);
        }
        
        size_type find_first_of(const _Elem *_Ptr, size_type _Off = 0) const {
            return m_str.find_first_of(_Ptr, _Off);
        }
        
        size_type find_first_of(_Elem _Ch, size_type _Off = 0) const {
            return m_str.find_first_of(_Ch, _Off);
        }
        
        size_type find_last_of(const _Myt& _Right,
                               size_type _Off = std::string::npos) const {
            return m_str.find_last_of(_Right, _Off);
        }
        
        size_type find_last_of(const _Elem *_Ptr, size_type _Off,
                               size_type _Count) const {
            return m_str.find_last_of(_Ptr, _Off, _Count);
        }
        
        size_type find_last_of(const _Elem *_Ptr,
                               size_type _Off = std::string::npos) const {
            return m_str.find_last_of(_Ptr, _Off);
        }
        
        size_type find_last_of(_Elem _Ch, size_type _Off = std::string::npos) const {
            return m_str.find_last_of(_Ch, _Off);
        }
        
        size_type find_first_not_of(const _Myt& _Right, size_type _Off = 0) const {
            return m_str.find_first_not_of(_Right, _Off);
        }
        
        size_type find_first_not_of(const _Elem *_Ptr, size_type _Off,
                                    size_type _Count) const {
            return m_str.find_first_not_of(_Ptr, _Off, _Count);
        }
        
        size_type find_first_not_of(const _Elem *_Ptr, size_type _Off = 0) const {
            return m_str.find_first_not_of(_Ptr, _Off);
        }
        
        size_type find_first_not_of(_Elem _Ch, size_type _Off = 0) const {
            return m_str.find_first_not_of(_Ch, _Off);
        }
        
        size_type find_last_not_of(const _Myt& _Right,
                                   size_type _Off = std::string::npos) const {
            return m_str.find_last_not_of(_Right, _Off);
        }
        
        size_type find_last_not_of(const _Elem *_Ptr, size_type _Off,
                                   size_type _Count) const {
            return m_str.find_last_not_of(_Ptr, _Off, _Count);
        }
        
        size_type find_last_not_of(const _Elem *_Ptr,
                                   size_type _Off = std::string::npos) const {
            return m_str.find_last_not_of(_Ptr, _Off);
        }
        
        size_type find_last_not_of(_Elem _Ch,
                                   size_type _Off = std::string::npos) const {
            return m_str.find_last_not_of(_Ch, _Off);
        }
        
        
        
        
        
    public:
        std::string m_str;
        
    };
    
}



#endif //AMO_TINY_STRING_778A1115_C103_45C1_9FAA_F1854C658F89_HPP__