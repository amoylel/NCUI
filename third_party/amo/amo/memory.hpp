/*
 * Copyright (c) 2014, Peter Thorson. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the WebSocket++ Project nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL PETER THORSON BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef AMO_MEMORY_HPP
#define AMO_MEMORY_HPP
 
#ifdef BOOST_NO_CXX11_SMART_PTR
#include <amo/config.hpp>
#else
#include <memory>
#endif // BOOST_NO_CXX11_SMART_PTR



namespace amo {

#ifdef BOOST_NO_CXX11_SMART_PTR
	using boost::shared_ptr;
	using boost::weak_ptr;
	using boost::enable_shared_from_this;
	using boost::static_pointer_cast;
	using boost::dynamic_pointer_cast;
	using boost::const_pointer_cast;
	using boost::reinterpret_pointer_cast;
	using boost::make_shared;
	typedef boost::scoped_array<unsigned char> unique_ptr_uchar_array;
#else
	using std::shared_ptr;
	using std::weak_ptr;
	using std::enable_shared_from_this;
	using std::static_pointer_cast;
	using std::dynamic_pointer_cast;
	using std::const_pointer_cast;
	//using std::reinterpret_pointer_cast;
	using std::make_shared;
	typedef std::unique_ptr<unsigned char[]> unique_ptr_uchar_array;
#endif // BOOST_NO_CXX11_SMART_PTR
 
} // namespace amo

#endif // AMO_MEMORY_HPP
