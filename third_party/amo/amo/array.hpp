#ifndef AMO_ARRAY_HPP__
#define AMO_ARRAY_HPP__

#ifdef BOOST_NO_CXX11_HDR_ARRAY
#include <amo/config.hpp>
#else
#include <array>
#endif // BOOST_NO_CXX11_HDR_ARRAY

namespace amo {

#ifdef BOOST_NO_CXX11_HDR_ARRAY
using boost::array;
#else
using std::array;
#endif // BOOST_NO_CXX11_HDR_ARRAY

} // namespace amo

#endif // AMO_ARRAY_HPP__
