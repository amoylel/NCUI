#ifndef AMO_ATOMIC_HPP__
#define AMO_ATOMIC_HPP__

// - atomic                   : building
#ifdef BOOST_NO_CXX11_HDR_ATOMIC
#include <boost/atomic.hpp>
#else
#include <atomic>
#endif

namespace amo
{
#ifdef BOOST_NO_CXX11_HDR_ATOMIC

	using boost::atomic;

	using boost::atomic_char;
	using boost::atomic_uchar;
	using boost::atomic_schar;
	using boost::atomic_uint8_t;
	using boost::atomic_int8_t;
	using boost::atomic_ushort;
	using boost::atomic_short;
	using boost::atomic_uint16_t;
	using boost::atomic_int16_t;
	using boost::atomic_uint;
	using boost::atomic_int;
	using boost::atomic_uint32_t;
	using boost::atomic_int32_t;
	using boost::atomic_ulong;
	using boost::atomic_long;
	using boost::atomic_uint64_t;
	using boost::atomic_int64_t;
#ifdef BOOST_HAS_LONG_LONG
	using boost::atomic_ullong;
	using boost::atomic_llong;
#endif
	using boost::atomic_address;
	using boost::atomic_bool;
	using boost::atomic_wchar_t;
#if !defined(BOOST_NO_CXX11_CHAR16_T)
	using boost::atomic_char16_t;
#endif
#if !defined(BOOST_NO_CXX11_CHAR32_T)
	using boost::atomic_char32_t;
#endif

	using boost::atomic_int_least8_t;
	using boost::atomic_uint_least8_t;
	using boost::atomic_int_least16_t;
	using boost::atomic_uint_least16_t;
	using boost::atomic_int_least32_t;
	using boost::atomic_uint_least32_t;
	using boost::atomic_int_least64_t;
	using boost::atomic_uint_least64_t;
	using boost::atomic_int_fast8_t;
	using boost::atomic_uint_fast8_t;
	using boost::atomic_int_fast16_t;
	using boost::atomic_uint_fast16_t;
	using boost::atomic_int_fast32_t;
	using boost::atomic_uint_fast32_t;
	using boost::atomic_int_fast64_t;
	using boost::atomic_uint_fast64_t;
	using boost::atomic_intmax_t;
	using boost::atomic_uintmax_t;

	using boost::atomic_size_t;
	using boost::atomic_ptrdiff_t;

#if defined(BOOST_HAS_INTPTR_T)
	using boost::atomic_intptr_t;
	using boost::atomic_uintptr_t;
#endif
#else
	using std::atomic;
	/*using std::atomic_int8_t;
	using std::atomic_uint8_t;
	using std::atomic_int16_t;
	using std::atomic_uint16_t;
	using std::atomic_int32_t;
	using std::atomic_uint32_t;
	using std::atomic_int64_t;
	using std::atomic_uint64_t;

	using std::atomic;
	using std::atomic_char;
	using std::atomic_schar;
	using std::atomic_uchar;
	using std::atomic_short;
	using std::atomic_ushort;
	using std::atomic_int;
	using std::atomic_uint;
	using std::atomic_long;
	using std::atomic_ulong;
	using std::atomic_llong;
	using std::atomic_ullong;
	using std::atomic_char16_t;
	using std::atomic_char32_t;
	using std::atomic_wchar_t;
	using std::atomic_int_least8_t;
	using std::atomic_uint_least8_t;
	using std::atomic_int_least16_t;
	using std::atomic_uint_least16_t;
	using std::atomic_int_least32_t;
	using std::atomic_uint_least32_t;
	using std::atomic_int_least64_t;
	using std::atomic_uint_least64_t;
	using std::atomic_int_fast8_t;
	using std::atomic_uint_fast8_t;
	using std::atomic_int_fast16_t;
	using std::atomic_uint_fast16_t;
	using std::atomic_int_fast32_t;
	using std::atomic_uint_fast32_t;
	using std::atomic_int_fast64_t;
	using std::atomic_uint_fast64_t;
	using std::atomic_intptr_t;
	using std::atomic_uintptr_t;
	using std::atomic_size_t;
	using std::atomic_ptrdiff_t;
	using std::atomic_intmax_t;
	using std::atomic_uintmax_t;*/
#endif
}

#endif // AMO_ATOMIC_HPP__