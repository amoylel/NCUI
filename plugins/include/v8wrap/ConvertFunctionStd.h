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
#include "internal/ConvertFunction.h"

namespace v8wrap
{
	template<class R>
	struct Convert<std::function<R()> > : public Internal::ConvertStdFunction<R()>
	{
		static std::function<R()> FromJS(const v8::Handle<v8::Value> & js)
		{
			auto ctx(v8::Context::GetCurrent());
			V8WRAP_ASSERT(!ctx.IsEmpty());

			auto global(ctx->Global());
			auto fn(v8::Handle<v8::Function>::Cast(js));
			
			return [fn, global]() -> R {
				return Internal::invoke<R>(global, fn, 0, NULL);
			};
		}
	};

	template<class R, class P0>
	struct Convert<std::function<R(P0)> > : public Internal::ConvertStdFunction<R(P0)>
	{
		static std::function<R(P0)> FromJS(const v8::Handle<v8::Value> & js)
		{
			auto ctx(v8::Context::GetCurrent());
			V8WRAP_ASSERT(!ctx.IsEmpty());

			auto global(ctx->Global());
			auto fn(v8::Handle<v8::Function>::Cast(js));
			
			return [fn, global](P0 a0) -> R {
				v8::Handle<v8::Value> args[1];
				args[0] = Convert<P0>::ToJS(a0);

				return Internal::invoke<R>(global, fn, 1, args);
			};
		}
	};
	
	template<class R, class P0, class P1>
	struct Convert<std::function<R(P0,P1)> > : public Internal::ConvertStdFunction<R(P0,P1)>
	{
		static std::function<R(P0,P1)> FromJS(const v8::Handle<v8::Value> & js)
		{
			auto ctx(v8::Context::GetCurrent());
			V8WRAP_ASSERT(!ctx.IsEmpty());

			auto global(ctx->Global());
			auto fn(v8::Handle<v8::Function>::Cast(js));
			
			return [fn, global](P0 a0, P1 a1) -> R {
				v8::Handle<v8::Value> args[2];
				args[0] = Convert<P0>::ToJS(a0);
				args[1] = Convert<P1>::ToJS(a1);

				return Internal::invoke<R>(global, fn, 2, args);
			};
		}
	};

	template<class R, class P0, class P1, class P2>
	struct Convert<std::function<R(P0,P1,P2)> > : public Internal::ConvertStdFunction<R(P0,P1,P2)>
	{
		static std::function<R(P0,P1,P2)> FromJS(const v8::Handle<v8::Value> & js)
		{
			auto ctx(v8::Context::GetCurrent());
			V8WRAP_ASSERT(!ctx.IsEmpty());

			auto global(ctx->Global());
			auto fn(v8::Handle<v8::Function>::Cast(js));
			
			return [fn, global](P0 a0, P1 a1, P2 a2) -> R {
				v8::Handle<v8::Value> args[3];
				args[0] = Convert<P0>::ToJS(a0);
				args[1] = Convert<P1>::ToJS(a1);
				args[2] = Convert<P2>::ToJS(a2);

				return Internal::invoke<R>(global, fn, 3, args);
			};
		}
	};

	template<class R, class P0, class P1, class P2, class P3>
	struct Convert<std::function<R(P0,P1,P2,P3)> > : public Internal::ConvertStdFunction<R(P0,P1,P2,P3)>
	{
		static std::function<R(P0,P1,P2,P3)> FromJS(const v8::Handle<v8::Value> & js)
		{
			auto ctx(v8::Context::GetCurrent());
			V8WRAP_ASSERT(!ctx.IsEmpty());

			auto global(ctx->Global());
			auto fn(v8::Handle<v8::Function>::Cast(js));
			
			return [fn, global](P0 a0, P1 a1, P2 a2, P3 p3) -> R {
				v8::Handle<v8::Value> args[4];
				args[0] = Convert<P0>::ToJS(a0);
				args[1] = Convert<P1>::ToJS(a1);
				args[2] = Convert<P2>::ToJS(a2);
				args[3] = Convert<P3>::ToJS(a3);

				return Internal::invoke<R>(global, fn, 4, args);
			};
		}
	};

	template<class R, class P0, class P1, class P2, class P3, class P4>
	struct Convert<std::function<R(P0,P1,P2,P3,P4)> > : public Internal::ConvertStdFunction<R(P0,P1,P2,P3,P4)>
	{
		static std::function<R(P0,P1,P2,P3,P4)> FromJS(const v8::Handle<v8::Value> & js)
		{
			auto ctx(v8::Context::GetCurrent());
			V8WRAP_ASSERT(!ctx.IsEmpty());

			auto global(ctx->Global());
			auto fn(v8::Handle<v8::Function>::Cast(js));
			
			return [fn, global](P0 a0, P1 a1, P2 a2, P3 a3, P4 a4) -> R {
				v8::Handle<v8::Value> args[5];
				args[0] = Convert<P0>::ToJS(a0);
				args[1] = Convert<P1>::ToJS(a1);
				args[2] = Convert<P2>::ToJS(a2);
				args[3] = Convert<P3>::ToJS(a3);
				args[4] = Convert<P4>::ToJS(a4);
			
				return Internal::invoke<R>(global, fn, 5, args);
			};
		}
	};
}
