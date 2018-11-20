#ifndef amo_tuple_h__
#define amo_tuple_h__

#ifdef BOOST_NO_CXX11_HDR_TUPLE
#include <amo/config.hpp>
#else
#include <tuple>
#endif
namespace amo
{
#ifdef BOOST_NO_CXX11_HDR_TUPLE
	using boost::tuple;
	using boost::tie;
	using boost::get;
	using boost::make_tuple;
#else
	using std::tuple;
	using std::make_tuple;
	using std::tie;
	using std::get;
#endif
}

#endif // tuple_h__
