#include "stdafx.h"

#include "handler/ResourceBundleHandler.h"

#include <amo/logger.hpp>


namespace amo {

    bool ResourceBundleHandler::GetDataResource(int resource_id,
            void*& data,
            size_t& data_size) {
        $clog(amo::cdevel << func_orient << "resource_id:" << resource_id << amo::endl;
             );
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->GetDataResource(resource_id, data, data_size);
            
            if (bHandled) {
                return true;
            }
        }
        
        return false;
    }
    
    bool ResourceBundleHandler::GetLocalizedString(int message_id,
            CefString& string) {
        $clog(amo::cdevel << func_orient << "message_id:" << message_id << amo::endl;);
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->GetLocalizedString(message_id, string);
            
            if (bHandled) {
                return true;
            }
        }
        
        return false;
    }
    
#if CHROME_VERSION_BUILD >= 2454
    bool ResourceBundleHandler::GetDataResourceForScale(int resource_id,
            ScaleFactor scale_factor,
            void*& data,
            size_t& data_size) {
        return false;
        //return CefResourceBundleHandler::GetDataResourceForScale(resource_id, scale_factor, data, data_size);
    }
#else
#endif
    
}