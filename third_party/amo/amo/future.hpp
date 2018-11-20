#ifndef AMO_FUTURE_HPP__
#define AMO_FUTURE_HPP__

#ifdef BOOST_NO_CXX11_HDR_FUTURE
#include <amo/config.hpp>
#else
#include <future>
#endif

namespace amo
{
#ifdef BOOST_NO_CXX11_HDR_FUTURE
	using boost::future;
	using boost::shared_future;
	using boost::promise;
#else
	using std::future;
	using std::shared_future;
	using std::promise;
	
#endif
}

#endif // AMO_FUTURE_HPP__

