#ifndef AMO_LIMITS_HPP__
#define AMO_LIMITS_HPP__


#ifdef BOOST_NO_CXX11_NUMERIC_LIMITS
#include <amo/config.hpp>
#else
#include <limits>
#endif

namespace amo
{
#ifdef BOOST_NO_CXX11_NUMERIC_LIMITS
	using boost::numeric_limits;
#else
	using std::numeric_limits;
#endif
}

#endif // AMO_LIMITS_HPP__

