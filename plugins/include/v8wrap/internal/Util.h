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

namespace v8wrap
{
	typedef void * TypeId;

	namespace Internal
	{
		// Convenience function to extract and cast a pointer stored in a v8::External
		template<class TYPE>
		inline TYPE * external_cast(const v8::Handle<v8::Value> & val)
		{
			return static_cast<TYPE*>(v8::External::Cast(*val)->Value());
		}

		// Generic weak callback function, for deleting weakly held pointers
		template<class TYPE>
		inline void WeakCallback(v8::Persistent<v8::Value> val, void * param)
		{
			delete static_cast<TYPE*>(param);
			val.Dispose();
			val.Clear();
		}

		// Provides a unique type id for a given type
		template<class TYPE>
		struct type_id
		{
			operator TypeId ()
			{
				static int id;
				return &id;
			}
		};
	}
}
