#ifndef AMO_RATIO_HPP__
#define AMO_RATIO_HPP__
 
#ifdef BOOST_NO_CXX11_HDR_RATIO
#include <amo/config.hpp>
#else
#include <ratio>
#endif

namespace amo
{
#ifdef BOOST_NO_CXX11_HDR_RATIO
	using boost::ratio;
	using boost::ratio_equal;
	using boost::ratio_greater;
	using boost::ratio_greater_equal;
	using boost::ratio_less;
	using boost::ratio_less_equal;
	using boost::ratio_not_equal;
	using boost::ratio_multiply;
	using boost::ratio_divide;
#else
	using std::ratio;
	using std::ratio_equal;
	using std::ratio_greater;
	using std::ratio_greater_equal;
	using std::ratio_less;
	using std::ratio_less_equal;
	using std::ratio_not_equal;
	using std::ratio_multiply;
	using std::ratio_divide;
#endif
}




#endif // AMO_RATIO_HPP__
