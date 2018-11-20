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
#include <stdint.h>

namespace v8wrap
{
	template<class TYPE> struct Convert;

	template<>
	struct Convert<int32_t>
	{
		static int32_t FromJS(const v8::Handle<v8::Value> & js)
		{
			return js->Int32Value();
		}

		static v8::Handle<v8::Value> ToJS(int32_t val)
		{
			return v8::Int32::New(v8::Isolate::GetCurrent(), val);
		}
	};

	template<>
	struct Convert<uint32_t>
	{
		static uint32_t FromJS(const v8::Handle<v8::Value> & js)
		{
			return js->Uint32Value();
		}

		static v8::Handle<v8::Value> ToJS(uint32_t val)
		{
			return v8::Uint32::New(v8::Isolate::GetCurrent(), val);
		}
	};

	template<>
	struct Convert<float>
	{
		static float FromJS(const v8::Handle<v8::Value> & js)
		{
			return (float)js->NumberValue();
		}

		static v8::Handle<v8::Value> ToJS(float val)
		{
			return v8::Number::New(v8::Isolate::GetCurrent(), (double)val);
		}
	};

	template<>
	struct Convert<bool>
	{
		static bool FromJS(const v8::Handle<v8::Value> & js)
		{
			return js->BooleanValue();
		}

		static v8::Handle<v8::Value> ToJS(bool val)
		{
			return v8::Boolean::New(v8::Isolate::GetCurrent(), val);
		}
	};

	template<>
	struct Convert<std::string>
	{
		static std::string FromJS(const v8::Handle<v8::Value> & js)
		{
			static const std::string empty;
			v8::String::Utf8Value utf(js->ToString());
			const char * str = *utf;
			return str ? std::string(str, utf.length()) : empty;
		}
		
		static v8::Handle<v8::Value> ToJS(const std::string & str)
		{
			return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), str.c_str());
		}
	};
	
	template<>
	struct Convert<const std::string &> : public Convert<std::string>
	{};
}

