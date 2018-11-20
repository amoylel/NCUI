#include "stdafx.h"
#include "handler/UtilityV8Handler.h"
#include "ipc/BlobManager.hpp"

namespace amo {

    bool UtilityV8Handler::Execute(const CefString& name,
                                   CefRefPtr<CefV8Value> object,
                                   const CefV8ValueList& arguments,
                                   CefRefPtr<CefV8Value>& retval,
                                   CefString& exception) {
        do {
            if (name == "sleep") {
            
                if (arguments.size() < 1) {
                    break;
                }
                
                CefRefPtr<CefV8Value> pValue = arguments.at(0);
                
                if (!pValue->IsInt()) {
                    break;
                }
                
                int val = pValue->GetIntValue();
                
                if (val <= 0) {
                    break;
                }
                
                std::this_thread::sleep_for(std::chrono::milliseconds(val));
                return true;
                
            } else if (name == "usleep") {
                if (arguments.size() < 1) {
                    break;
                }
                
                CefRefPtr<CefV8Value> pValue = arguments.at(0);
                
                if (!pValue->IsInt()) {
                    break;
                }
                
                int val = pValue->GetIntValue();
                
                if (val <= 0) {
                    break;
                }
                
                std::this_thread::sleep_for(std::chrono::microseconds(val));
                return true;
            } else if (name == "updateBlobData") {
                auto mgr = BlobManager<PID_RENDERER>::getInstance();
                
                if (arguments.size() != 3) {
                    exception = L"invalid arguments for updateBlobdata: need 3 arguments";
                    return false;
                }
                
                CefRefPtr<CefV8Value> pName = arguments.at(0);
                CefRefPtr<CefV8Value> pMime = arguments.at(1);
                CefRefPtr<CefV8Value> pData = arguments.at(2);
                
                if (!pName->IsString()) {
                    exception = L"invalid arguments for updateBlobdata: need string for blob name";
                    return false;
                    
                }
                
                if (!pMime->IsString()) {
                    exception =
                        L"invalid arguments for updateBlobdata: need string for blob mime type";
                    return false;
                    
                }
                
                if (!pData->IsString()) {
                    exception = L"invalid arguments for updateBlobdata: need string for blob data";
                    return false;
                    
                    
                }
                
                std::string name = pName->GetStringValue();
                std::string mime = pMime->GetStringValue();
                std::string data = pData->GetStringValue();
                
                if (name.empty()) {
                    exception =
                        L"invalid arguments for updateBlobdata: blob name is empty";
                    return false;
                }
                
                auto blob = mgr->find(name);
                
                if (!blob) {
                    exception =
                        L"invalid arguments for updateBlobdata: can not found bolb object";
                    return false;
                }
                
                blob->mime(mime);
                blob->write(data);
                blob->state(amo::blob::BLOB_READY);
                blob->write_header();
                
            }
        } while (false);
        
        
        exception == L"call function  failed";
        return false;
    }
    
}

