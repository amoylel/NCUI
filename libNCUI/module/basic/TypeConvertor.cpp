#include "stdafx.h"
#include "TypeConvertor.h"
#include <amo/json.hpp>
#include "module/JsClassObjectV8Handler.h"
#include "transfer/FunctionWrapper.hpp"

#include <utility>
#include "module/basic/ClassMethodMgr.hpp"
#include "utility/utility.hpp"
#include "../../ipc/BlobManager.hpp"



namespace amo {

    std::unordered_map < std::pair<int64_t, int64_t>,
        CefRefPtr<CefV8Value>,
        pair_hash2<int64_t, int64_t> > TypeConvertor::m_oObjectMap;
        
    bool TypeConvertor::getRendererObject() const {
        return m_bRendererObject;
    }
    
    void TypeConvertor::setRendererObject(bool val) {
        m_bRendererObject = val;
    }
    
    TypeConvertor::TypeConvertor(CefRefPtr<CefFrame> frame /*= NULL*/) {
        pFrame = frame;
        m_bRendererObject = false;
        
        if (frame) {
            pContext = frame->GetV8Context();
        } else {
            pContext = CefV8Context::GetCurrentContext();
            pFrame = pContext->GetFrame();
        }
        
        // 如果是C++在调用JS,那么需要Context环境
        pContext->Enter();
    }
    
    TypeConvertor::~TypeConvertor() {
        if (pContext) {
            pContext->Exit();
        }
    }
    
    
    
    
    
    CefRefPtr<CefV8Value> TypeConvertor::ParseJsonToObject(amo::u8json& json) {
    
        CefRefPtr<CefV8Value> retval = CefV8Value::CreateUndefined();
        
        // 非法的JSON
        if (!json.is_valid()) {
            return retval;
        }
        
        bool bTransfer = false;
        
        if (json.is_object() && json.contains_key("transferObject")
                && json.get<bool>("transferObject")) {
            bTransfer = true;
        }
        
        if (!bTransfer
                && json.is_object()
                && json.contains_key("transferClass")
                && json.get<bool>("transferClass")) {
            bTransfer = true;
        }
        
        std::string sssss = json.to_string();
        
        if (bTransfer || (!json.is_object() && !json.is_array())) {
            return ParseSingleJsonToObject(json);
        } else {
        
#if CHROME_VERSION_BUILD >= 2840
            CefRefPtr<CefV8Value> pObject = CefV8Value::CreateObject(NULL, NULL);
#else
            CefRefPtr<CefV8Value> pObject = CefV8Value::CreateObject(NULL);
#endif
            
            if (json.is_array()) {
            
                std::vector<amo::u8json> vec = json.to_array();
                pObject = CefV8Value::CreateArray(vec.size());
                
                for (size_t i = 0; i < vec.size(); ++i) {
                    pObject->SetValue(i, ParseJsonToObject(vec[i]));
                }
                
            } else {
            
                // 遍历JSON
                for (auto iter = json.begin(); iter != json.end(); ++iter) {
                    amo::u8json p(iter->value);
                    std::string sb = p.to_string();
                    std::string name = iter->name.GetString();
                    pObject->SetValue(name,
                                      ParseSingleJsonToObject(p),
                                      V8_PROPERTY_ATTRIBUTE_NONE);
                                      
                }
            }
            
            
            return pObject;
        }
        
        
        
    }
    
    CefRefPtr<CefV8Value> TypeConvertor::ParseSingleJsonToObject(
        amo::u8json& json) {
        std::string sb = json.to_string();
        
        if (!json.is_valid()) {
            return  CefV8Value::CreateUndefined();
        } else if (json.is_bool()) {
            return CefV8Value::CreateBool(json.get<bool>());
            
        } else if (json.is_int()) {
            return CefV8Value::CreateInt(json.get<int>());
            
        } else if (json.is_uint()) {
            return Int64ToObject(json.get<uint32_t>());
        } else if (json.is_uint64()) {
            return Int64ToObject(json.get<uint64_t>());
        } else if (json.is_int64()) {
            return Int64ToObject(json.get<int64_t>());
            //return CefV8Value::CreateInt(json.get<int64_t>());
        } else if (json.is_double()) {
            return
                CefV8Value::CreateDouble(json.get<double>());
        } else if (json.is_string()) {
            return
                CefV8Value::CreateString(json.get<std::string>());
        } else if (json.is_object()) {
            // 先判断JSON是否为Transfer
            bool bTransfer = false;
            
            if (json.contains_key("transferObject")
                    && json.get<bool>("transferObject")) {
                bTransfer = true;
            }
            
            if (!bTransfer
                    && json.contains_key("transferClass")
                    && json.get<bool>("transferClass")) {
                bTransfer = true;
            }
            
            if (bTransfer) {
                int64_t nId = json.get<int64_t>("id");
                std::string strClass = json.get<std::string>("name");
                std::string strObjectName = json.get<std::string>("objectName");
                // 通过ID查找缓存
                CefRefPtr<CefV8Value> pValue = getClassObject(nId);
                
                if (pValue && pValue->IsObject()) {
                    return  pValue;
                }
                
                
                
                // 查看当前Class是否是可以实例外的类
                auto classMethodMgr = ClassMethodMgr::getInstance();
                
                if (!classMethodMgr->hasClass(strClass)) {
                
                    // 不是，返回Undefined
                    return
                        CefV8Value::CreateUndefined();
                } else {
                    // 获取所给类的成员信息
                    FunctionWrapperMgr mgr =
                        classMethodMgr->getClass(strClass);
                        
                    // 设置ObjectID
                    mgr.setObjectID(nId);
                    mgr.setObjectName(strObjectName);
                    
                    {
                        //// 创建新的Object
                        //CefRefPtr<JsClassObjectV8Handler> pObjectHandler;
                        //pObjectHandler = new JsClassObjectV8Handler();
                        //pObjectHandler->setRendererClass(mgr.isRendererClass());
                        //pObjectHandler->setHandlerName(strClass);
                        ////pObjectHandler->setFunctions(mgr.toVector());
                        //pObjectHandler->setFuncMgr(mgr);
                        //pObjectHandler->setID(nId);
                        //
                        //// 设置返回值为新创建的对象
                        //CefRefPtr<CefV8Value> pObject =
                        //    pObjectHandler->getV8Object();
                        ////// 添加对象进缓存
                        ////addClassObject(nId, pObject);
                        ////
                        ////CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
                        ////CefRefPtr<CefFrame> pFrame = context->GetFrame();
                        ////int64_t nFrameID = pFrame->GetIdentifier();
                        ////
                        ////removeClassObject(nFrameID, nId);
                        ////pObject->Release();
                        //
                        //int i = 3;
                        //++i;
                        //
                        //if (i == 4) {
                        //
                        //}
                    }
                    // 创建新的Object
                    CefRefPtr<JsClassObjectV8Handler> pObjectHandler;
                    pObjectHandler = new JsClassObjectV8Handler();
                    pObjectHandler->setRendererClass(mgr.isRendererClass());
                    pObjectHandler->setHandlerName(strClass);
                    //pObjectHandler->setFunctions(mgr.toVector());
                    pObjectHandler->setFuncMgr(mgr);
                    pObjectHandler->setID(nId);
                    
                    // 设置返回值为新创建的对象
                    CefRefPtr<CefV8Value> pObject =
                        pObjectHandler->getV8Object();
                    // 添加对象进缓存
                    addClassObject(nId, pObject);
                    
                    
                    /* int64_t nFrameID =
                    	 CefV8Context::GetCurrentContext()->GetFrame()->GetIdentifier();
                     removeClassObject(nFrameID, nId);*/
                    return pObject;
                }
                
            } else {
                if (json.contains_key("a02c2b36-3d70-44f6-9456-85dab0e0ddb5")) {
                    int nType = json.getInt("a02c2b36-3d70-44f6-9456-85dab0e0ddb5");
                    
                    switch (nType) {
                    case 0: // Undefined
                        return CefV8Value::CreateUndefined();
                        
                    case 1: // NULL
                        return CefV8Value::CreateNull();
                        
                    default:
                        break;
                    }
                } else {
                    return ParseJsonToObject(json);
                }
                
            }
            
            
        } else if (json.is_array()) {
            std::vector<amo::u8json> vec = json.to_array();
            CefRefPtr<CefV8Value> pArray = CefV8Value::CreateArray(vec.size());
            
            for (size_t i = 0; i < vec.size(); ++i) {
                pArray->SetValue(i, ParseJsonToObject(vec[i]));
            }
            
            return pArray;
            
        }
        
        return CefV8Value::CreateUndefined();
    }
    
    
    
    Any TypeConvertor::ParseObjectToJson(CefRefPtr<CefV8Value> pObject) {
        amo::u8json json;
        
        if (!pObject || !pObject->IsObject()) {
            return  json;
        }
        
        {
            std::string transferJson = transferObject(pObject);
            
            if (!transferJson.empty()) {
                amo::u8json transfer(transferJson);
                amo::u8string id = amo::u8string::from_number<int64_t>(transfer.getInt64("id"));
                auto bigstr = BigStrManager<PID_RENDERER>::getInstance()->find(id.str());
                
                if (bigstr) {
                    return *bigstr;
                }
                
                return id.to_utf8();
            }
        }
        
        
        
        std::vector<CefString> vec;
        pObject->GetKeys(vec);
        
        for (size_t i = 0; i < vec.size(); ++i) {
            CefRefPtr<CefV8Value> pValue = pObject->GetValue(vec[i]);
            
            if (!pValue || pValue->IsUndefined() || pValue->IsNull()) {
                continue;
            }
            
            
            std::string transferJson = transferObject(pValue);
            
            if (transferJson.empty()) {
                Any ret = ParseSingleObjectToAny(pValue);
                AddAnyToJson(json, vec[i].ToString(), ret);
                /* amo::cdevel << json.to_string() << amo::endl;
                 amo::cdevel << amo::u8string(json.to_string(), true).str() << amo::endl;*/
            } else {
                amo::u8json transfer(transferJson);
                amo::u8string id = amo::u8string::from_number<int64_t>(transfer.getInt64("id"));
                json.put(vec[i].ToString(), id.to_utf8());
            }
        }
        
        /*       std::string u8String = json.to_string();
               amo::cdevel << func_orient << u8String << amo::endl;
               amo::cdevel << func_orient << amo::u8string(json.to_string(), true).str() << amo::endl;*/
        return json;
    }
    
    Any TypeConvertor::ParseSingleObjectToAny(CefRefPtr<CefV8Value> pValue) {
        if (!pValue || pValue->IsUndefined()) {
            return Undefined();
        } else if (pValue->IsNull()) {
            return amo::Nil();
        } else if (pValue->IsBool()) {
            return pValue->GetBoolValue();
        } else if (pValue->IsInt()) {
            return pValue->GetIntValue();
        } else if (pValue->IsDouble()) {
            return pValue->GetDoubleValue();
        } else if (pValue->IsString()) {
            //amo::cdevel << pValue->GetStringValue().ToString() << amo::endl;
            return pValue->GetStringValue().ToString();
        } else if (pValue->IsArray()) {
            std::vector<Any> vec;
            
            for (int i = 0; i < pValue->GetArrayLength(); ++i) {
                vec.push_back(toAny(pValue->GetValue(i)));
            }
            
            return vec;
        } else if (pValue->IsObject()) {
            return ParseObjectToJson(pValue);
        }
        
        return Nothing();
    }
    
    void TypeConvertor::AddAnyToJson(amo::u8json& json, const std::string& key,
                                     Any& val) {
        return amo::util().addAnyToJson(json, key, val);
        
        /* switch (val.type()) {
         case  AnyValueType<Undefined>::value:
         case  AnyValueType<Nil>::value:
             json.put(key, val.toJson());
             break;
        
         case  AnyValueType<bool>::value:
             json.put(key, val.As<bool>());
             break;
        
         case  AnyValueType<int>::value:
             json.put(key, val.As<int>());
             break;
        
         case  AnyValueType<double>::value:
             json.put(key, val.As<double>());
             break;
        
         case  AnyValueType<std::string>::value:
             json.put(key, val.As<std::string>());
             break;
        
         case  AnyValueType<amo::u8json>::value:
             json.put(key, val.As<amo::u8json>());
             break;
        
         case  AnyValueType<std::vector<Any> >::value: {
             amo::u8json arr;
             arr.set_array();
             std::vector<Any> vec = val;
        
             for (size_t i = 0; i < vec.size(); ++i) {
                 AddAnyToJsonArray(arr, vec[i]);
             }
        
             json.put(key, arr);
         }
        
         break;
        
         default:
             break;
         }*/
        
    }
    
    void TypeConvertor::AddAnyToJsonArray(amo::u8json& json, Any& val) {
        return amo::util().addAnyToJsonArray(json, val);
        /*switch (val.type()) {
        case  AnyValueType<Undefined>::value:
        case  AnyValueType<Nil>::value:
            json.push_back(val.toJson());
            break;
        
        case  AnyValueType<bool>::value:
            json.push_back(val.As<bool>());
            break;
        
        case  AnyValueType<int>::value:
            json.push_back(val.As<int>());
            break;
        
        case  AnyValueType<double>::value:
            json.push_back(val.As<double>());
            break;
        
        case  AnyValueType<std::string>::value:
            json.push_back(val.As<std::string>());
            break;
        
        case  AnyValueType<amo::u8json>::value:
            json.push_back(val.As<amo::u8json>());
            break;
        
        default:
            break;
        }
        
        return;*/
    }
    
    std::string TypeConvertor::transferObject(CefRefPtr<CefV8Value> pValue) {
        std::string transferJson;
        
        if (pValue->IsObject() || pValue->IsFunction()) {
            if (pValue->HasValue("transferJson")) {
                CefRefPtr<CefV8Value> pTransferJson;
                pTransferJson = pValue->GetValue("transferJson");
                transferJson = pTransferJson->GetStringValue();
            }
            
            if (transferJson.empty() && pValue->HasValue("transferObject")) {
                CefRefPtr<CefV8Value> pTransferJson;
                pTransferJson = pValue->GetValue("transferJson");
                transferJson = pTransferJson->GetStringValue();
            }
        }
        
        return transferJson;
        
    }
    
    std::string TypeConvertor::V8ValueType(CefRefPtr<CefV8Value> object) {
        if (object->IsArray()) {
            return "Array";
        } else if (object->IsBool()) {
            return "Bool";
        } else if (object->IsDate()) {
            return "Date";
        } else if (object->IsDouble()) {
            return "Double";
        } else if (object->IsFunction()) {
            return "Function";
        } else if (object->IsInt()) {
            return "Int";
        } else if (object->IsNull()) {
            return "NULL";
        } else if (object->IsObject()) {
            return "Object";
        } else if (object->IsString()) {
            return "String";
        } else if (object->IsUInt()) {
            return "Uint";
        } else if (object->IsUndefined()) {
            return "Undefined";
        } else if (object->IsValid()) {
            return "Valid";
        } else {
            return "Unknown";
        }
    }
    
    CefRefPtr<CefV8Value> TypeConvertor::toV8Value(Any& any) {
        char type = any.type();
        CefRefPtr<CefV8Value> retval;
        
        switch (type) {
        case AnyValueType<amo::Undefined>::value:
            retval = CefV8Value::CreateUndefined();
            break;
            
        case AnyValueType<amo::Nil>::value:
            retval = CefV8Value::CreateNull();
            break;
            
        case AnyValueType<bool>::value:
            retval = CefV8Value::CreateBool((bool)any);
            break;
            
        case AnyValueType<int>::value:
            retval = CefV8Value::CreateInt((int)any);
            break;
            
        case AnyValueType<int64_t>::value:
            retval =   Int64ToObject(any.As<int64_t>());
            break;
            
        case AnyValueType<double>::value:
            retval = CefV8Value::CreateDouble((double)any);
            break;
            
        case AnyValueType<std::string>::value:
            retval = CefV8Value::CreateString(any.As<std::string>());
            break;
            
        case AnyValueType<amo::blob>::value: {
            amo::blob blob = any.As<amo::blob>();
            blob.read_header();
            amo::u8json opt;
            opt.put("mime", blob.mime());
            
            CefRefPtr<CefV8Value> ptrOpt = 	JsonToObject(opt);
            std::string data = blob.read();
            retval = CreateBlobByString(data, ptrOpt);
            break;
        }
        
        case AnyValueType<amo::bigstr>::value: {
            amo::bigstr blob = any.As<amo::bigstr>();
            retval = CefV8Value::CreateString(blob.read());
            break;
        }
        
        
        case  AnyValueType<amo::u8json>::value: {
    
            // 返回一个JS Object
            amo::u8json json = (amo::u8json)any;
            
            if (json.is_array()) {
                std::vector<amo::u8json> vec = json.to_array();
                CefRefPtr<CefV8Value> retvalArr = CefV8Value::CreateArray(vec.size());
                
                for (size_t i = 0; i < vec.size(); ++i) {
                    amo::u8json& p = vec[i];
#if CHROME_VERSION_BUILD >= 2840
                    CefRefPtr<CefV8Value> object = CefV8Value::CreateObject(NULL, NULL);
#else
                    CefRefPtr<CefV8Value> object = CefV8Value::CreateObject(NULL);
#endif
                    
                    object = ParseJsonToObject(p);
                    
                    if (!object->IsUndefined()) {
                        retvalArr->SetValue(i, object);
                    }
                }
                
                retval = retvalArr;
            } else {
                retval =  ParseJsonToObject(json);
            }
            
            break;
        }
        
        case AnyValueType<std::vector<Any> >::value: {
            std::vector<Any> vec = any;
            
            retval = CefV8Value::CreateArray(vec.size());
            
            for (size_t i = 0; i < vec.size(); ++i) {
                retval->SetValue(i, toV8Value(vec[i]));
            }
            
            break;
        }
        
        /*      case AnyValueType<std::vector<Any>>::value: {
                  std::vector<Any> vec = any;
    
                  if (!vec.empty()) {
                      CefRefPtr<CefV8Value> retvalArr = CefV8Value::CreateArray(vec.size());
    
                      for (size_t i = 0; i < vec.size(); ++i) {
    
                          CefRefPtr<CefV8Value> object = toV8Value(vec[i]);
                          retvalArr->SetValue(i, object);
                      }
    
                      retval = retvalArr;
                  }
    
              }
              break;*/
    
        default:
            retval = CefV8Value::CreateUndefined();
            break;
        }
        
        return retval;
    }
    
    CefRefPtr<CefV8Value> TypeConvertor::toV8Value(
        std::shared_ptr<AnyArgsList> args,
        int index) {
        
        Any& any = args->getValue(index);
        return toV8Value(any);
        
    }
    
    int TypeConvertor::GetBlobSize(CefRefPtr<CefV8Value> pValue) {
    
        CefRefPtr<CefV8Value> objectIsBlob;
        objectIsBlob = pContext->GetGlobal()->GetValue("objectIsBlob");
        
        if (!objectIsBlob) {
            return  0;
        }
        
        CefV8ValueList list;
        list.push_back(pValue);
        CefRefPtr<CefV8Value> retval = objectIsBlob->ExecuteFunction(objectIsBlob,
                                       list);
                                       
        if (!retval) {
            return 0;
        }
        
        if (!retval->IsInt()) {
            return 0;
        }
        
        return retval->GetIntValue();
    }
    
    void TypeConvertor::ReadDataFromBlob(const std::string& blobName,
                                         CefRefPtr<CefV8Value> pValue) {
        CefRefPtr<CefV8Value> readDataFromBlob;
        readDataFromBlob = pContext->GetGlobal()->GetValue("readDataFromBlob");
        
        if (!readDataFromBlob) {
            return ;
        }
        
        CefV8ValueList list;
        list.push_back(CefV8Value::CreateString(blobName));
        list.push_back(pValue);
        CefRefPtr<CefV8Value> retval = readDataFromBlob->ExecuteFunction(
                                           readDataFromBlob,
                                           list);
        return;
    }
    
    CefRefPtr<CefV8Value> TypeConvertor::getClassObject(int64_t nID,
            CefRefPtr<CefFrame> frame) {
            
            
        if (!frame) {
            frame = CefV8Context::GetCurrentContext()->GetFrame();
        }
        
        int64_t nFrameID = frame->GetIdentifier();
        CefRefPtr<CefV8Value> retval;
        auto iter = m_oObjectMap.find(std::make_pair(nFrameID, nID));
        
        if (iter != m_oObjectMap.end()) {
            retval = iter->second;
        }
        
        return retval;
    }
    
    CefRefPtr<CefV8Value> TypeConvertor::getClassObject(int64_t nFrameID,
            int64_t nID) {
        CefRefPtr<CefV8Value> retval;
        auto iter = m_oObjectMap.find(std::make_pair(nFrameID, nID));
        
        if (iter != m_oObjectMap.end()) {
            retval = iter->second;
        }
        
        return retval;
    }
    
    TypeConvertor::ObjectContainer TypeConvertor::getClassObject(
        const std::vector<int64_t>& frames,
        int64_t nID) {
        ObjectContainer vec;
        
        for (auto& p : m_oObjectMap) {
            int nFrameID = p.first.first;
            
            // 如果FrameID在所级的合集内，那么查找Frame里面有没有ObjectID
            if (std::find(frames.begin(), frames.end(), nFrameID) != frames.end()
                    && p.first.second == nID) {
                vec.push_back(std::make_pair(p.first.first, p.second));
            }
            
        }
        
        return vec;
    }
    
    
    amo::u8json TypeConvertor::ObjectToJson(CefRefPtr<CefV8Value> pV8Value) {
    
        if (!pFrame) {
            pFrame = pContext->GetFrame();
        }
        
        
        
        /*std::vector<CefString> vvv;
        pV8Value->GetKeys(vvv);
        
        for (auto& p : vvv) {
        $clog(amo::cdevel << p.ToString() << amo::endl;);
        }
        
        CefRefPtr<CefV8Value> pFunction = pV8Value->GetValue("nativeObject");
        std::string ssss = V8ValueType(pFunction);
        
        if (pFunction && pFunction->IsFunction()) {
        
        CefRefPtr<CefV8Value> pFuncToJson = pV8Value->GetValue("nativeToJson");
        
        if (pFuncToJson && pFuncToJson->IsFunction()) {
        
        CefRefPtr<CefV8Value> retal;
        CefRefPtr<CefV8Exception> exp;
        CefV8ValueList list;
        CefRefPtr<CefV8Value> val;
        val = retal->ExecuteFunctionWithContext(pFrame->GetV8Context(),
        NULL,
        list);
        
        if (val->IsString()) {
        return amo::u8json(val->GetStringValue());
        } else {
        return amo::u8json();
        }
        
        } else {
        return amo::u8json();
        }
        }*/
        
        
        CefRefPtr<CefV8Value> retal;
        CefRefPtr<CefV8Exception> exp;
        std::string js = "(function(){return objectToString;})()";
#if CHROME_VERSION_BUILD >= 2840
        bool bOk = pFrame->GetV8Context()->Eval(js, CefString(), 0, retal, exp);
#else
        bool bOk = pFrame->GetV8Context()->Eval(js, retal, exp);
#endif
        
        
        if (bOk && retal && retal->IsFunction()) {
            CefV8ValueList list;
            list.push_back(pV8Value);
            CefRefPtr<CefV8Value> val;
            val = retal->ExecuteFunctionWithContext(pFrame->GetV8Context(),
                                                    NULL,
                                                    list);
                                                    
            if (val->IsString()) {
                return amo::u8json(val->GetStringValue());
            }
        }
        
        return amo::u8json();
    }
    
    CefRefPtr<CefV8Value> TypeConvertor::JsonToObject(const std::string& str) {
        CefRefPtr<CefV8Value> stringToObject;
        stringToObject = pContext->GetGlobal()->GetValue("stringToObject");
        
        if (!stringToObject) {
            return CefV8Value::CreateUndefined();
        }
        
        CefV8ValueList list;
        list.push_back(CefV8Value::CreateString(str));
        return stringToObject->ExecuteFunction(stringToObject, list);
    }
    
    CefRefPtr<CefV8Value> TypeConvertor::JsonToObject(amo::u8json& json) {
        std::string str = json.to_string();
        return JsonToObject(str);
    }
    
    CefRefPtr<CefV8Value> TypeConvertor::Int64ToObject(int64_t nVal) {
        CefRefPtr<CefV8Value> int64ToObject;
        int64ToObject = pContext->GetGlobal()->GetValue("parseInt");
        
        if (!int64ToObject) {
            return CefV8Value::CreateUndefined();
        }
        
        std::string str = std::to_string(nVal);
        
        CefV8ValueList list;
        list.push_back(CefV8Value::CreateString(str));
        return int64ToObject->ExecuteFunction(int64ToObject, list);
    }
    
    
    CefRefPtr<CefV8Value> TypeConvertor::CreateBlobByString(const std::string& str,
            CefRefPtr<CefV8Value> opt) {
        CefRefPtr<CefV8Value> createBlobByString;
        createBlobByString = pContext->GetGlobal()->GetValue("createBlobByString");
        
        if (!createBlobByString) {
            return CefV8Value::CreateUndefined();
        }
        
        CefV8ValueList list;
        list.push_back(CefV8Value::CreateString(str));
        list.push_back(opt);
        return createBlobByString->ExecuteFunction(createBlobByString, list);
    }
    
    CefRefPtr<CefV8Value> TypeConvertor::CreateBlobByArray(const
            std::vector<std::string>& vec,
            CefRefPtr<CefV8Value> opt) {
        CefRefPtr<CefV8Value> createBlobByArray;
        createBlobByArray = pContext->GetGlobal()->GetValue("createBlobByArray");
        
        if (!createBlobByArray) {
            return CefV8Value::CreateUndefined();
        }
        
        CefRefPtr<CefV8Value> arr = 	CefV8Value::CreateArray(vec.size());
        
        for (size_t i = 0; i < vec.size(); ++i) {
            arr->SetValue(i, CefV8Value::CreateString(vec[i]));
        }
        
        CefV8ValueList list;
        list.push_back(arr);
        list.push_back(opt);
        return createBlobByArray->ExecuteFunction(createBlobByArray, list);
    }
    
    Any TypeConvertor::toAny(CefRefPtr<CefV8Value> pValue) {
        if (!pValue || pValue->IsUndefined()) {
            return Undefined();
        } else if (pValue->IsNull()) {
            return amo::Nil();
        } else if (pValue->IsBool()) {
            return pValue->GetBoolValue();
        } else if (pValue->IsInt()) {
            return pValue->GetIntValue();
        } else if (pValue->IsDouble()) {
            return pValue->GetDoubleValue();
        } else if (pValue->IsString()) {
            return pValue->GetStringValue().ToString();
        }  else if (pValue->IsArray()) {
            std::vector<Any> vec;
            
            for (int i = 0; i < pValue->GetArrayLength(); ++i) {
                vec.push_back(toAny(pValue->GetValue(i)));
            }
            
            return vec;
        } else   {
            int blobsize = GetBlobSize(pValue);
            
            if (blobsize > 0) {
                auto mgr = BlobManager<PID_RENDERER>::getInstance();
                auto blob = mgr->create(blobsize);
                blob->write_header();
                ReadDataFromBlob(blob->get_name(), pValue);
                return *blob;
            }
        }
        
        
        if (pValue->IsObject()) {
            return ParseObjectToJson(pValue);
        }
        
        return Undefined();
    }
    
    void TypeConvertor::addClassObject(int64_t nID,
                                       CefRefPtr<CefV8Value> pObject) {
        /*std::string ss = V8ValueType(pObject);*/
        CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
        CefRefPtr<CefFrame> pFrame = context->GetFrame();
        int64_t nFrameID = pFrame->GetIdentifier();
        m_oObjectMap[std::pair<int64_t, int64_t>(nFrameID, nID)] = pObject;
        //m_oObjectMap.erase({ nFrameID, nID });
        
    }
    
    void TypeConvertor::clearClassObject(int64_t nFrameID) {
        std::vector<std::pair<int64_t, int64_t> > keys;
        
        for (auto& p : m_oObjectMap) {
            if (p.first.first == nFrameID) {
                keys.push_back(p.first);
            }
        }
        
        for (auto& p : keys) {
            m_oObjectMap.erase(p);
        }
    }
    
    void TypeConvertor::removeClassObject(int64_t nFrameID, int64_t nObjectID) {
        auto frame = BrowserManager<PID_RENDERER>::GetFrameByID(nFrameID);
        CefRefPtr<CefV8Context> pContext;
        
        if (frame) {
            pContext = frame->GetV8Context();
        }
        
        if (pContext) {
            // 如果是C++在调用JS,那么需要Context环境
            pContext->Enter();
        }
        
        auto ptr = m_oObjectMap.find({ nFrameID, nObjectID });
        
        if (ptr != m_oObjectMap.end()) {
            auto pObject = ptr->second;
            
            if (pContext && pObject && pObject->IsObject()) {
                JsClassV8Handler* pHandler = dynamic_cast<JsClassV8Handler*>
                                             (pObject->GetUserData().get());
                                             
                if (pHandler != NULL) {
                    auto vec = pHandler->getFuncMgr().toMap();
                    
                    for (auto& p : vec) {
                        CefString strKey = p.first;
                        
                        if (pObject->HasValue(strKey)) {
                            pObject->GetValue(strKey)->SetUserData(NULL);
                            pObject->DeleteValue(strKey);
                        }
                    }
                }
            }
            
            pObject->SetUserData(NULL);
            
        }
        
        
        m_oObjectMap.erase({ nFrameID, nObjectID });
        
        if (pContext) {
            pContext->Exit();
        }
        
        
    }
    
}

