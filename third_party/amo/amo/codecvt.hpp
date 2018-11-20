#ifndef AMO_CODECVT_HPP__
#define AMO_CODECVT_HPP__
 
// - locale                   : building
#ifdef BOOST_NO_CXX11_HDR_CODECVT
#include <amo/config.hpp>
#else
#include <codecvt>
#endif
namespace amo
{
#ifdef BOOST_NO_CXX11_HDR_CODECVT
	/*using boost::locale;
	using boost::codecvt;*/
#else
	using std::locale;
	using std::codecvt;

#endif
}

#endif // AMO_CODECVT_HPP__
