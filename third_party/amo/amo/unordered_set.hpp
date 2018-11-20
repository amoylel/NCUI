#ifndef AMO_UNORDERED_SET_HPP__
#define AMO_UNORDERED_SET_HPP__



#ifdef BOOST_NO_CXX11_HDR_UNORDERED_SET
#include <amo/config.hpp>
#else
#include <unordered_set>
#endif
namespace amo
{
#ifdef BOOST_NO_CXX11_HDR_UNORDERED_SET
	using boost::unordered_set;
#else
	using std::unordered_set;
#endif
}

 
 

#endif // AMO_UNORDERED_SET_HPP__
