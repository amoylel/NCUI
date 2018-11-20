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

#if defined(_MSC_VER)
#define DEBUG_BREAK __debugbreak()
#else
// Nothing yet...
#define DEBUG_BREAK
#endif

#define V8WRAP_UNUSED(x) do { (void)sizeof(x); } while(0)

#if defined(_DEBUG) || defined(V8WRAP_ASSERTS_ENABLED)
#define V8WRAP_ASSERT(cnd) \
	do \
	{ \
		if (!(cnd)) \
		{ \
			fprintf(stderr, "Assert(%s) failed at (%s:%d).\n", #cnd, __FILE__, __LINE__); \
			DEBUG_BREAK; \
		} \
	} while(0)

#define V8WRAP_ASSERT_MSG(cnd, msg, ...) \
	do \
	{ \
		if (!(cnd)) \
		{ \
			fprintf(stderr, "Assert(%s) failed at (%s:%d): " ## msg ## "\n", #cnd, __FILE__, __LINE__, __VA_ARGS__); \
			DEBUG_BREAK; \
		} \
	} while(0)

#else	
#define V8WRAP_ASSERT(cnd) do { V8WRAP_UNUSED(cnd); } while(0)
#define V8WRAP_ASSERT_MSG(cnd, msg, ...) do { V8WRAP_UNUSED(cnd); } while(0)
#endif
