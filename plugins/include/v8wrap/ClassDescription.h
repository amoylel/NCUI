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

#include <v8.h>
#include <map>
#include <memory>
#include "ClassMethod.h"
#include "internal/ClassField.h"
#include "internal/Assert.h"

namespace v8wrap
{
	template<class CLASS>
	class ClassDescription
	{
	public:	
		template<class TYPE, TYPE CLASS:: * PTR>
		void Field(const char * name)
		{
			auto Prototype(FunctionTemplate()->PrototypeTemplate());
		/*	Prototype->SetAccessor(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), name), &Internal::Field<CLASS,TYPE>::Get<PTR>,
				&Internal::Field<CLASS,TYPE>::Set<PTR>);*/
		}

		template<class SIG>
		ClassMethod<SIG,CLASS> Method()
		{
			return ClassMethod<SIG, CLASS>();
		}

		static v8::Handle<v8::FunctionTemplate> FunctionTemplate()
		{
			static v8::Persistent<v8::FunctionTemplate> FnTemplate;
			if (FnTemplate.IsEmpty())
			{
				FnTemplate = v8::Persistent<v8::FunctionTemplate>::New(v8::Isolate::GetCurrent(), v8::FunctionTemplate::New(v8::Isolate::GetCurrent(), &Constructor));
				FnTemplate->InstanceTemplate()->SetInternalFieldCount(3);
				FnTemplate.MakeWeak(nullptr,&Internal::WeakCallback<int>);
			}
			return FnTemplate;
		}

	private:
		typedef std::map<CLASS *, v8::Persistent<v8::Object> > InstanceMap;

		static InstanceMap & Instances()
		{
			static InstanceMap instances;
			return instances;
		}

		static v8::Handle<v8::Value> Constructor(const v8::FunctionCallbackInfo<v8::Value> & args)
		{
			auto ctx = v8::Context::GetCurrent();
			V8WRAP_ASSERT(!ctx.IsEmpty());

			if (!args.IsConstructCall())
				return v8::Undefined();

			CLASS * inst = nullptr;
			std::shared_ptr<CLASS> * shared = nullptr;

			if (args.Length() == 1 && args[0]->IsExternal())
			{
				inst = Internal::external_cast<CLASS>(args[0]);
			}
			else if (args.Length() == 2 && args[0]->IsExternal())
			{
				shared = Internal::external_cast<std::shared_ptr<CLASS> >(args[0]);
				inst = shared->get();
			}
			else
			{
				inst = new CLASS;
			}

			InstanceMap & instances = Instances();
			if (instances.count(inst) > 0)
				return instances[inst];

			if (shared == nullptr)
				shared = new std::shared_ptr<CLASS>(inst);

			TypeId id = Internal::type_id<CLASS>();

			args.Holder()->SetPointerInInternalField(0, id);
			args.Holder()->SetPointerInInternalField(1, shared);
			args.Holder()->SetPointerInInternalField(2, inst);

			auto ret(v8::Persistent<v8::Object>::New(args.Holder()));
			ret.MakeWeak(shared, &WeakObjectCallback);

			instances[inst] = ret;

			return ret;
		}

		static void WeakObjectCallback(v8::Persistent<v8::Value> val, void * param)
		{
			std::shared_ptr<CLASS> * inst = static_cast<std::shared_ptr<CLASS> *>(param);

			InstanceMap & instances = Instances();
			instances.erase(inst->get());
			delete inst;

			val.Dispose();
			val.Clear();
		}
	};
}
