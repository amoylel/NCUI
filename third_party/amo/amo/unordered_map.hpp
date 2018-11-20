#ifndef AMO_UNORDERED_MAP_HPP__
#define AMO_UNORDERED_MAP_HPP__



#ifdef BOOST_NO_CXX11_HDR_UNORDERED_MAP
#include <amo/config.hpp>
#else
#include <unordered_map>
#endif
namespace amo
{
#ifdef BOOST_NO_CXX11_HDR_UNORDERED_MAP
	using boost::unordered_map;
#else
	using std::unordered_map;
#endif
}


#endif // AMO_UNORDERED_MAP_HPP__
