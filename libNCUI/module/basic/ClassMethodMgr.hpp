// Created by amoylel on 07/12/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_CLASSMETHODMGR_HPP__
#define AMO_CLASSMETHODMGR_HPP__

#include <unordered_map>
#include "transfer/FunctionWrapper.hpp"
#include <amo/singleton.hpp>


namespace amo {

    /*!
     * @class	ClassMethodMgr
     *
     * @brief	Manager for class methods.
     */
    
    class ClassMethodMgr: public amo::singleton<ClassMethodMgr> {
    public:
    
        /*!
         * @fn	void ClassMethodMgr::addClass(const std::string& strClass, FunctionWrapperMgr& mgr)
         *
         * @brief	Adds the class to 'mgr'.
         *
         * @param	strClass   	The class.
         * @param [in,out]	mgr	The manager.
         */
        
        void addClass(const std::string& strClass, FunctionWrapperMgr& mgr) {
            m_oClassMap[strClass] = mgr;
        }
        
        /*!
         * @fn	bool ClassMethodMgr::hasClass(const std::string& strClass) const
         *
         * @brief	Query if 'strClass' has class.
         *
         * @param	strClass	The class.
         *
         * @return	true if class, false if not.
         */
        
        bool hasClass(const std::string& strClass) const {
            return (m_oClassMap.find(strClass) != m_oClassMap.end());
        }
        
        /*!
         * @fn	FunctionWrapperMgr& ClassMethodMgr::getClass(const std::string& strClass)
         *
         * @brief	Gets the class.
         *
         * @param	strClass	The class.
         *
         * @return	The class.
         */
        
        FunctionWrapperMgr& getClass(const std::string& strClass) {
            return m_oClassMap[strClass];
        }
        
        /*!
         * @fn	void ClassMethodMgr::clear()
         *
         * @brief	Clears this object to its blank/initial state.
         */
        
        void clear() {
            m_oClassMap.clear();
        }
    private:
        /*! @brief	保存所给类的所有成员函数. */
        std::unordered_map<std::string, FunctionWrapperMgr > m_oClassMap;
        
    };
    
}

#endif // AMO_CLASSMETHODMGR_HPP__
