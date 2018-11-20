// Created by amoylel on 07/10/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NODEV8HANDLER_H__
#define AMO_NODEV8HANDLER_H__

#include "nan/nan.h"
#include "ipc/Any.hpp"

namespace amo{
	class NodeV8Handler : public Nan::ObjectWrap{
	public:
		NodeV8Handler(){
			m_nID = 0;
		}
		void setName(const std::string& strName)
		{
			m_strClassName = strName;
		}

		std::string& getName()
		{
			return m_strClassName;
		}

		int64_t getID() const { return m_nID; }

		void setID(int64_t val) { m_nID = val; }

		static std::string ObjectToString(v8::Local<v8::Value> value)
		{
			v8::String::Utf8Value utf8_value(value);
			return std::string(*utf8_value);
		}

		virtual amo::Any emit(IPCMessage::SmartType anyMessage){
			return amo::Undefined();
		}
	protected:
		int64_t m_nID;																					//!< 对象ID
		std::string m_strClassName;																	//!< 当前类名
	};
}

#endif // AMO_NODEV8HANDLER_H__
