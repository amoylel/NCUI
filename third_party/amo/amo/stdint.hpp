#ifndef AMO_STDINT_HPP
#define AMO_STDINT_HPP

#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS 1
#endif

#ifdef BOOST_HAS_STDINT_H
#include <stdint.h>
#else
#include <amo/config.hpp>
#include <boost/cstdint.hpp>

using boost::int8_t;
using boost::int_least8_t;
using boost::int_fast8_t;
using boost::uint8_t;
using boost::uint_least8_t;
using boost::uint_fast8_t;

using boost::int16_t;
using boost::int_least16_t;
using boost::int_fast16_t;
using boost::uint16_t;
using boost::uint_least16_t;
using boost::uint_fast16_t;

using boost::int32_t;
using boost::int_least32_t;
using boost::int_fast32_t;
using boost::uint32_t;
using boost::uint_least32_t;
using boost::uint_fast32_t;

#ifndef BOOST_NO_INT64_T
using boost::int64_t;
using boost::int_least64_t;
using boost::int_fast64_t;
using boost::uint64_t;
using boost::uint_least64_t;
using boost::uint_fast64_t;
#endif
using boost::intmax_t;
using boost::uintmax_t;

#endif // BOOST_HAS_STDINT_H

typedef unsigned int uint;

//#ifdef _MSC_VER
//typedef __int8  int8;
//typedef __int16 int16;
//typedef __int32 int32;
//typedef __int64 int64;
//
//typedef unsigned __int8  uint8;
//typedef unsigned __int16 uint16;
//typedef unsigned __int32 uint32;
//typedef unsigned __int64 uint64;
//#else
//typedef int8_t  int8;
//typedef int16_t int16;
//typedef int32_t int32;
//typedef int64_t int64;
//
//typedef uint8_t  uint8;
//typedef uint16_t uint16;
//typedef uint32_t uint32;
//typedef uint64_t uint64;
//#endif
#endif // AMO_STDINT_HPP
