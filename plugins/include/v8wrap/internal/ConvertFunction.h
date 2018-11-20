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
#include <functional>
#include "Util.h"
#include "Assert.h"

namespace v8wrap
{
	namespace Internal
	{
		// invoke is used for converting a javascript function for use in c++ as an std::function
		// when this conversion occurs, we create a lambda to provide some sugar.  This lambda calls 
		// the invoke function after first converting the arguments from c++ to js values
		template<class R>
		static R invoke(v8::Handle<v8::Object> ctx, v8::Handle<v8::Function> fn, int argc, v8::Handle<v8::Value> args[])
		{
			return Convert<R>::FromJS(fn->Call(ctx, argc, args));
		}

		template<>
		static void invoke<void>(v8::Handle<v8::Object> ctx, v8::Handle<v8::Function> fn, int argc, v8::Handle<v8::Value> args[])
		{
			fn->Call(ctx, argc, args);
		}

		// The function structure contains the actual invocation callbacks invoked by v8 when some piece of
		// of js calls our wrapped functions.  The code will extract the function pointer (or the std::function pointer)
		// from the passed in arguments, convert the arguments from js to c++ and then call the c++ side function,
		// grabbing the result and converting it back to js
		template<class SIG> struct Function;

		template<class R>
		struct Function<R()>
		{
			static v8::Handle<v8::Value> CallNative(const v8::FunctionCallbackInfo<v8::Value>& args)
			{
				V8WRAP_ASSERT(args.Length() == 0);

				auto fn = external_cast<R()>(args.Data());
				return Convert<R>::ToJS(fn());
			}

			static v8::Handle<v8::Value> CallStd(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 0);

				auto fn = external_cast<std::function<R()> >(args.Data());
				return Convert<R>::ToJS((*fn)());
			}
		};

		template<>
		struct Function<void()>
		{
			static v8::Handle<v8::Value> CallNative(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 0);

				auto fn = external_cast<void()>(args.Data());
				fn();
				return v8::Undefined(v8::Isolate::GetCurrent());
			}

			static v8::Handle<v8::Value> CallStd(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 0);

				auto fn = external_cast<std::function<void()> >(args.Data());
				(*fn)();
				return v8::Undefined(v8::Isolate::GetCurrent());
			} 
		};

		template<class R, class P0>
		struct Function<R(P0)>
		{
			static v8::Handle<v8::Value> CallNative(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 1);

				auto fn = external_cast<R(P0)>(args.Data());
				return Convert<R>::ToJS(fn(Convert<P0>::FromJS(args[0])));
			}

			static v8::Handle<v8::Value> CallStd(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 1);

				auto fn = external_cast<std::function<R(P0)> >(args.Data());
				return Convert<R>::ToJS((*fn)(Convert<P0>::FromJS(args[0])));
			}
		};

		template<class P0>
		struct Function<void(P0)>
		{
			static v8::Handle<v8::Value> CallNative(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 1);

				auto fn = external_cast<void(P0)>(args.Data());
				fn(Convert<P0>::FromJS(args[0]));
				return v8::Undefined(v8::Isolate::GetCurrent());
			}

			static v8::Handle<v8::Value> CallStd(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 1);

				auto fn = external_cast<std::function<void(P0)> >(args.Data());
				(*fn)(Convert<P0>::FromJS(args[0]));
				return v8::Undefined(v8::Isolate::GetCurrent());
			} 
		};

		template<class R, class P0, class P1>
		struct Function<R(P0,P1)>
		{
			static v8::Handle<v8::Value> CallNative(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 2);

				auto fn = external_cast<R(P0,P1)>(args.Data());
				return Convert<R>::ToJS(fn(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1])));
			}

			static v8::Handle<v8::Value> CallStd(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 2);

				auto fn = external_cast<std::function<R(P0,P1)> >(args.Data());
				return Convert<R>::ToJS((*fn)(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1])));
			}
		};

		template<class P0, class P1>
		struct Function<void(P0,P1)>
		{
			static v8::Handle<v8::Value> CallNative(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 2);

				auto fn = external_cast<void(P0,P1)>(args.Data());
				fn(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]));
				return v8::Undefined(v8::Isolate::GetCurrent());
			}

			static v8::Handle<v8::Value> CallStd(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 2);

				auto fn = external_cast<std::function<void(P0,P1)> >(args.Data());
				(*fn)(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]));
				return v8::Undefined(v8::Isolate::GetCurrent());
			} 
		};

		template<class R, class P0, class P1, class P2>
		struct Function<R(P0,P1,P2)>
		{
			static v8::Handle<v8::Value> CallNative(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 3);

				auto fn = external_cast<R(P0,P1,P2)>(args.Data());
				return Convert<R>::ToJS(fn(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]),
					Convert<P2>::FromJS(args[2])));
			}

			static v8::Handle<v8::Value> CallStd(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 3);

				auto fn = external_cast<std::function<R(0,P1,P2)> >(args.Data());
				return Convert<R>::ToJS((*fn)(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]),
					Convert<P2>::FromJS(args[2])));
			}
		};

		template<class P0, class P1, class P2>
		struct Function<void(P0,P1,P2)>
		{
			static v8::Handle<v8::Value> CallNative(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 3);

				auto fn = external_cast<void(P0,P1,P2)>(args.Data());
				fn(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]), Convert<P2>::FromJS(args[2]));
				return v8::Undefined(v8::Isolate::GetCurrent());
			}

			static v8::Handle<v8::Value> CallStd(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 3);

				auto fn = external_cast<std::function<void(P0,P1,P2)> >(args.Data());
				(*fn)(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]), Convert<P2>::FromJS(args[2]));
				return v8::Undefined(v8::Isolate::GetCurrent());
			} 
		};

		template<class R, class P0, class P1, class P2, class P3>
		struct Function<R(P0,P1,P2,P3)>
		{
			static v8::Handle<v8::Value> CallNative(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 4);

				auto fn = external_cast<R(P0,P1,P2,P3)>(args.Data());
				return Convert<R>::ToJS(fn(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]),
					Convert<P2>::FromJS(args[2]), Convert<P3>::FromJS(args[3])));
			}

			static v8::Handle<v8::Value> CallStd(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 4);

				auto fn = external_cast<std::function<R(P0,P1,P2,P3)> >(args.Data());
				return Convert<R>::ToJS((*fn)(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]),
					Convert<P2>::FromJS(args[2]), Convert<P3>::FromJS(args[3])));
			}
		};

		template<class P0, class P1, class P2, class P3>
		struct Function<void(P0,P1,P2,P3)>
		{
			static v8::Handle<v8::Value> CallNative(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 4);

				auto fn = external_cast<void(P0,P1,P2,P3)>(args.Data());
				fn(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]), Convert<P2>::FromJS(args[2]),
					Convert<P3>::FromJS(args[3]));
				return v8::Undefined(v8::Isolate::GetCurrent());
			}

			static v8::Handle<v8::Value> CallStd(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 4);

				auto fn = external_cast<std::function<void(P0,P1,P2,P3)> >(args.Data());
				(*fn)(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]), Convert<P2>::FromJS(args[2]),
					Convert<P3>::FromJS(args[3]));
				return v8::Undefined(v8::Isolate::GetCurrent());
			} 
		};

		template<class R, class P0, class P1, class P2, class P3, class P4>
		struct Function<R(P0,P1,P2,P3,P4)>
		{
			static v8::Handle<v8::Value> CallNative(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 5);

				auto fn = external_cast<R(P0,P1,P2,P3,P4)>(args.Data());
				return Convert<R>::ToJS(fn(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]),
					Convert<P2>::FromJS(args[2]), Convert<P3>::FromJS(args[3]), Convert<P4>::FromJS(args[4])));
			}

			static v8::Handle<v8::Value> CallStd(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 5);

				auto fn = external_cast<std::function<R(P0,P1,P2,P3,P4)> >(args.Data());
				return Convert<R>::ToJS((*fn)(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]),
					Convert<P2>::FromJS(args[2]), Convert<P3>::FromJS(args[3]), Convert<P5>::FromJS(args[4])));
			}
		};

		template<class P0, class P1, class P2, class P3, class P4>
		struct Function<void(P0,P1,P2,P3,P4)>
		{
			static v8::Handle<v8::Value> CallNative(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 5);

				auto fn = external_cast<void(P0,P1,P2,P3,P4)>(args.Data());
				fn(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]), Convert<P2>::FromJS(args[2]),
					Convert<P3>::FromJS(args[3]), Convert<P4>::FromJS(args[4]));
				return v8::Undefined(v8::Isolate::GetCurrent());
			}

			static v8::Handle<v8::Value> CallStd(const v8::FunctionCallbackInfo<v8::Value> & args)
			{
				V8WRAP_ASSERT(args.Length() == 5);

				auto fn = external_cast<std::function<void(P0,P1,P2,P3,P4)> >(args.Data());
				(*fn)(Convert<P0>::FromJS(args[0]), Convert<P1>::FromJS(args[1]), Convert<P2>::FromJS(args[2]),
					Convert<P3>::FromJS(args[3]), Convert<P4>::FromJS(args[4]));
				return v8::Undefined(v8::Isolate::GetCurrent());
			} 
		};

		// ConvertFunction and ConvertStdFunction are conveniences
		// used as base classes for the Convert struct specializations
		// that convert between js and c++ functions

		template<class SIG>
		struct ConvertFunction
		{
			typedef SIG * FnPtr;

			static FnPtr FromJS(const v8::Handle<v8::Value> & js)
			{
				static_assert(false, "No conversion from JS to function pointer available.  Please convert to a std::function instead.");
				return nullptr;
			}

			static v8::Handle<v8::Value> ToJS(FnPtr fn)
			{
				auto callback = &Internal::Function<SIG>::CallNative;
				auto arg = v8::External::New(fn);
				return v8::FunctionTemplate::New(callback, arg)->GetFunction();
			}
		};

		template<class SIG>
		struct ConvertStdFunction
		{
			static v8::Handle<v8::Value> ToJS(const std::function<SIG> & fn)
			{
				auto callback = &Internal::Function<SIG>::CallStd;
				auto copy = new std::function<SIG>(fn);
				auto ext = v8::External::New(copy);
				auto arg(v8::Persistent<v8::External>::New(ext));
				arg.MakeWeak(copy, &Internal::WeakCallback<std::function<SIG>>);
				return v8::FunctionTemplate::New(callback, arg)->GetFunction();
			}
		};
	}
}
