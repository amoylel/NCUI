// Created by amoylel on 7/02/2017.
// Copyright (c) 2017 amoylel. All rights reserved.
#ifndef AMO_RESOURCEBUNDLEHANDLER_H__
#define AMO_RESOURCEBUNDLEHANDLER_H__

#include "handler/CefHeader.hpp"

#include "handler/HandlerDelegate.hpp"
namespace amo {

    /*!
     * @class	ResourceBundleHandlerDelegate
     *
     * @brief	自定义资源处理类，多线程调用.
     */
    
    class ResourceBundleHandlerDelegate {
    public:
        ResourceBundleHandlerDelegate() {}
        
        virtual ~ResourceBundleHandlerDelegate() {}
        
        virtual bool GetLocalizedString(int message_id,
                                        CefString& string) {
            return false;
        }
        
        virtual bool GetDataResource(int resource_id,
                                     void*& data,
                                     size_t& data_size) {
            return false;
        }
#if CHROME_VERSION_BUILD >= 2454
        virtual bool GetDataResourceForScale(int resource_id,
                                             CefResourceBundleHandler::ScaleFactor scale_factor,
                                             void*& data,
                                             size_t& data_size) {
            return false;
        }
#endif
    };
    
    /*!
     * @class	ResourceBundleHandler
     *
     * @brief	自定义资源处理类，多线程调用.
     */
    
    class ResourceBundleHandler : public CefResourceBundleHandler
        , public HandlerDelegate < ResourceBundleHandlerDelegate > {
    public:
    
        /*!
         * @fn	virtual bool ResourceBundleHandler::GetLocalizedString(
         * 		int message_id, CefString& string);
         *
         * @brief	获取本地化字符串.
         *
         * @param	message_id	  	Identifier for the message.
         * @param 	string	The string.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool GetLocalizedString(int message_id,
                                        CefString& string) override;
                                        
        /*!
         * @fn	virtual bool ResourceBundleHandler::GetDataResource(
         * 		int resource_id,
         * 		void*& data,
         * 		size_t& data_size);
         *
         * @brief	Gets data resource.
         *
         * @param	resource_id		 	Identifier for the resource.
         * @param 	data	 	[in,out] If non-null, the data.
         * @param 	data_size	Size of the data.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool GetDataResource(int resource_id,
                                     void*& data,
                                     size_t& data_size) override;
                                     
                                     
#if CHROME_VERSION_BUILD >= 2454
                                     
        /*!
         * @fn	virtual bool ResourceBundleHandler::GetDataResourceForScale(
         * 		int resource_id,
         * 		ScaleFactor scale_factor,
         * 		void*& data, size_t& data_size);
         *
         * @brief	Gets data resource for scale.
         *
         * @param	resource_id		 	Identifier for the resource.
         * @param	scale_factor	 	The scale factor.
         * @param 	data	 	[in,out] If non-null, the data.
         * @param 	data_size	Size of the data.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool GetDataResourceForScale(int resource_id,
                                             ScaleFactor scale_factor,
                                             void*& data,
                                             size_t& data_size) override;
#else
#endif
                                             
                                             
        IMPLEMENT_REFCOUNTING(ResourceBundleHandler);
        
    };
}
#endif // AMO_RESOURCEBUNDLEHANDLER_H__
