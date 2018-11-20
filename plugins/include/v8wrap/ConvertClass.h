/*
Copyright (c) 2012 Chris Lentini
http://divergentcoder.com

Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to use, 
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the 
Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <memory>
#include <v8.h>
#include "ClassDescription.h"

namespace v8wrap
{
	template<class CLASS>
	struct Convert<CLASS *>
	{
		static CLASS * FromJS(const v8::Local<v8::Value> & js)
		{
			V8WRAP_ASSERT(js->IsObject());

			if (!js->IsObject())
				return nullptr;

			auto obj(v8::Local<v8::Object>::Cast(js));
			if (obj->InternalFieldCount() != 3)
				return nullptr;

			TypeId id = obj->GetPointerFromInternalField(0);
			TypeId target = Internal::type_id<CLASS>();
			V8WRAP_ASSERT(id == target);
			if (id != target)
				return nullptr;

			return static_cast<CLASS *>(obj->GetPointerFromInternalField(2));
		}
		
		static v8::Handle<v8::Value> ToJS(CLASS * inst)
		{
			auto fn = ClassDescription<CLASS>::FunctionTemplate()->GetFunction();
			v8::Handle<v8::Value> ext = v8::External::New(inst);
			v8::Local<v8::Object> jsref = fn->NewInstance(1, &ext);

			return jsref;
		}
	};

	template<class CLASS>
	struct Convert<std::shared_ptr<CLASS> *>
	{
		static std::shared_ptr<CLASS> * FromJS(const v8::Local<v8::Value> & js)
		{
			V8WRAP_ASSERT(js->IsObject());

			if (!js->IsObject())
				return nullptr;

			auto obj(v8::Local<v8::Object>::Cast(js));
			if (obj->InternalFieldCount() != 3)
				return nullptr;

			TypeId id = obj->GetPointerFromInternalField(0);
			TypeId target = Internal::type_id<CLASS>();
			V8WRAP_ASSERT(id == target);
			if (id != target)
				return NULL;

			return static_cast<std::shared_ptr<CLASS> *>(obj->GetPointerFromInternalField(1));
		}
		
		static v8::Handle<v8::Value> ToJS(std::shared_ptr<CLASS> * inst)
		{
			auto fn = ClassDescription<CLASS>::FunctionTemplate()->GetFunction();
			v8::Handle<v8::Value> ext = v8::External::New(inst->get());
			v8::Local<v8::Object> jsref = fn->NewInstance(1, &ext);

			return jsref;
		}
	};

	template<class CLASS>
	struct Convert<const std::shared_ptr<CLASS> &>
	{
		static const std::shared_ptr<CLASS> & FromJS(const v8::Local<v8::Value> & js)
		{
			V8WRAP_ASSERT(js->IsObject());

			v8::Local<v8::Object> obj(v8::Local<v8::Object>::Cast(js));
			V8WRAP_ASSERT(obj->InternalFieldCount() == 3);

			TypeId id = obj->GetPointerFromInternalField(0);
			TypeId target = Internal::type_id<CLASS>();
			V8WRAP_ASSERT(id == target);

			auto shared = static_cast<std::shared_ptr<CLASS> *>(obj->GetPointerFromInternalField(1));
			V8WRAP_ASSERT(shared != nullptr);

			return *shared;
		}
		
		static v8::Handle<v8::Value> ToJS(const std::shared_ptr<CLASS> & inst)
		{
			auto fn = ClassDescription<CLASS>::FunctionTemplate()->GetFunction();
			v8::Handle<v8::Value> ext[2] = { v8::External::New(&inst), v8::Undefined() };
			v8::Local<v8::Object> jsref = fn->NewInstance(2, ext);

			return jsref;
		}
	};
}
