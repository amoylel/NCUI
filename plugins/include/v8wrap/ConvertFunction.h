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
#include "internal/ConvertFunction.h"

namespace v8wrap
{	
	template<class R>
	struct Convert<R(*)()> : public Internal::ConvertFunction<R()>
	{};

	template<class R, class P0>
	struct Convert<R(*)(P0)> : public Internal::ConvertFunction<R(P0)>
	{};

	template<class R, class P0, class P1>
	struct Convert<R(*)(P0,P1)> : public Internal::ConvertFunction<R(P0,P1)>
	{};

	template<class R, class P0, class P1, class P2>
	struct Convert<R(*)(P0,P1,P2)> : public Internal::ConvertFunction<R(P0,P1,P2)>
	{};

	template<class R, class P0, class P1, class P2, class P3>
	struct Convert<R(*)(P0,P1,P2,P3)> : public Internal::ConvertFunction<R(P0,P1,P2,P3)>
	{};

	template<class R, class P0, class P1, class P2, class P3, class P4>
	struct Convert<R(*)(P0,P1,P2,P3,P4)> : public Internal::ConvertFunction<R(P0,P1,P2,P3,P4)>
	{};

	// A convenience function for adding a C++ function to a v8 Object
	//	- obviously this could be abused since it doesn't make sure we are trying to wrap a function
	template<class FnSig>
	void AddFunction(v8::Handle<v8::Object> & obj, const char * name, FnSig * fn)
	{
		obj->Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), name), Convert<FnSig*>::ToJS(fn));
	}
}
