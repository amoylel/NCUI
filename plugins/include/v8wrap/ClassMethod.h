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
#include "internal/Assert.h"

namespace v8wrap
{
	template<class SIG, class CLASS> struct ClassMethod;

	template<class R, class CLASS>
	struct ClassMethod<R(), CLASS>
	{
	public:
		template<R (CLASS::*Ptr)()>
		void Set(const char * name)
		{
			auto Prototype(ClassDescription<CLASS>::FunctionTemplate()->PrototypeTemplate());
			//Prototype->Set(v8::Isolate::GetCurrent(), name, v8::FunctionTemplate::New(&CallStruct<R>::Call<Ptr>)->GetFunction());
		}

	private:
		template<class T> struct CallStruct
		{
			template<T (CLASS::*Ptr)()>
			static v8::Handle<v8::Value> Call(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 0);
				CLASS * inst = Convert<CLASS*>::FromJS(args.This());
				return Convert<T>::ToJS((inst->*Ptr)());
			}
		};

		template<> struct CallStruct<void>
		{
			template<void (CLASS::*Ptr)()>
			static v8::Handle<v8::Value> Call(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 0);
				CLASS * inst = Convert<CLASS*>::FromJS(args.This());
				(inst->*Ptr)();
				return v8::Undefined();
			}
		};
	};

	template<class R, class P0, class CLASS>
	struct ClassMethod<R(P0), CLASS>
	{
	public:
		template<R (CLASS::*Ptr)(P0)>
		void Set(const char * name)
		{
			auto Prototype(ClassDescription<CLASS>::FunctionTemplate()->PrototypeTemplate());
			Prototype->Set(name, v8::FunctionTemplate::New(&CallStruct<R>::Call<Ptr>)->GetFunction());
		}

	private:
		template<class T> struct CallStruct
		{
			template<T (CLASS::*Ptr)(P0)>
			static v8::Handle<v8::Value> Call(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 1);
				CLASS * inst = Convert<CLASS*>::FromJS(args.This());
				return Convert<T>::ToJS((inst->*Ptr)(Convert<P0>::FromJS(args[0])));
			}
		};

		template<> struct CallStruct<void>
		{
			template<void (CLASS::*Ptr)(P0)>
			static v8::Handle<v8::Value> Call(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 1);
				CLASS * inst = Convert<CLASS*>::FromJS(args.This());
				(inst->*Ptr)(Convert<P0>::FromJS(args[0]));
				return v8::Undefined();
			}
		};
	};

	template<class R, class P0, class P1, class CLASS>
	struct ClassMethod<R(P0,P1), CLASS>
	{
	public:
		template<R (CLASS::*Ptr)(P0,P1)>
		void Set(const char * name)
		{
			auto Prototype(ClassDescription<CLASS>::FunctionTemplate()->PrototypeTemplate());
			Prototype->Set(name, v8::FunctionTemplate::New(&CallStruct<R>::Call<Ptr>)->GetFunction());
		}

	private:
		template<class T> struct CallStruct
		{
			template<T (CLASS::*Ptr)(P0,P1)>
			static v8::Handle<v8::Value> Call(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 2);
				CLASS * inst = Convert<CLASS*>::FromJS(args.This());
				return Convert<T>::ToJS((inst->*Ptr)(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1])));
			}
		};

		template<> struct CallStruct<void>
		{
			template<void (CLASS::*Ptr)(P0,P1)>
			static v8::Handle<v8::Value> Call(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 2);
				CLASS * inst = Convert<CLASS*>::FromJS(args.This());
				(inst->*Ptr)(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]));
				return v8::Undefined();
			}
		};
	};

	template<class R, class P0, class P1, class P2, class CLASS>
	struct ClassMethod<R(P0,P1,P2), CLASS>
	{
	public:
		template<R (CLASS::*Ptr)(P0,P1,P2)>
		void Set(const char * name)
		{
			auto Prototype(ClassDescription<CLASS>::FunctionTemplate()->PrototypeTemplate());
			Prototype->Set(name, v8::FunctionTemplate::New(&CallStruct<R>::Call<Ptr>)->GetFunction());
		}

	private:
		template<class T> struct CallStruct
		{
			template<T (CLASS::*Ptr)(P0,P1,P2)>
			static v8::Handle<v8::Value> Call(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 3);
				CLASS * inst = Convert<CLASS*>::FromJS(args.This());
				return Convert<T>::ToJS((inst->*Ptr)(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]), 
					Convert<P2>::FromJS(args[2])));
			}
		};

		template<> struct CallStruct<void>
		{
			template<void (CLASS::*Ptr)(P0,P1,P2)>
			static v8::Handle<v8::Value> Call(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 3);
				CLASS * inst = Convert<CLASS*>::FromJS(args.This());
				(inst->*Ptr)(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]), Convert<P2>::FromJS(args[2]));
				return v8::Undefined();
			}
		};
	};

	template<class R, class P0, class P1, class P2, class P3, class CLASS>
	struct ClassMethod<R(P0,P1,P2,P3), CLASS>
	{
	public:
		template<R (CLASS::*Ptr)(P0,P1,P2,P3)>
		void Set(const char * name)
		{
			auto Prototype(ClassDescription<CLASS>::FunctionTemplate()->PrototypeTemplate());
			Prototype->Set(name, v8::FunctionTemplate::New(&CallStruct<R>::Call<Ptr>)->GetFunction());
		}

	private:
		template<class T> struct CallStruct
		{
			template<T (CLASS::*Ptr)(P0,P1,P2,P3)>
			static v8::Handle<v8::Value> Call(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 4);
				CLASS * inst = Convert<CLASS*>::FromJS(args.This());
				return Convert<T>::ToJS((inst->*Ptr)(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]), 
					Convert<P2>::FromJS(args[2]), Convert<P3>::FromJS(args[3])));
			}
		};

		template<> struct CallStruct<void>
		{
			template<void (CLASS::*Ptr)(P0,P1,P2,P3)>
			static v8::Handle<v8::Value> Call(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 4);
				CLASS * inst = Convert<CLASS*>::FromJS(args.This());
				(inst->*Ptr)(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]), Convert<P2>::FromJS(args[2]),
					Convert<P3>::FromJS(args[3]));
				return v8::Undefined();
			}
		};
	};

	template<class R, class P0, class P1, class P2, class P3, class P4, class CLASS>
	struct ClassMethod<R(P0,P1,P2,P3,P4), CLASS>
	{
	public:
		template<R (CLASS::*Ptr)(P0,P1,P2,P3,P4)>
		void Set(const char * name)
		{
			auto Prototype(ClassDescription<CLASS>::FunctionTemplate()->PrototypeTemplate());
			Prototype->Set(name, v8::FunctionTemplate::New(&CallStruct<R>::Call<Ptr>)->GetFunction());
		}

	private:
		template<class T> struct CallStruct
		{
			template<T (CLASS::*Ptr)(P0,P1,P2,P3,P4)>
			static v8::Handle<v8::Value> Call(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 5);
				CLASS * inst = Convert<CLASS*>::FromJS(args.This());
				return Convert<T>::ToJS((inst->*Ptr)(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]), 
					Convert<P2>::FromJS(args[2]), Convert<P3>::FromJS(args[3]), Convert<P4>::FromJS(args[4])));
			}
		};

		template<> struct CallStruct<void>
		{
			template<void (CLASS::*Ptr)(P0,P1,P2,P3,P4)>
			static v8::Handle<v8::Value> Call(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 5);
				CLASS * inst = Convert<CLASS*>::FromJS(args.This());
				(inst->*Ptr)(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]), Convert<P2>::FromJS(args[2]),
					Convert<P3>::FromJS(args[3]), Convert<P4>::FromJS(args[4]));
				return v8::Undefined();
			}
		};
	};
}
