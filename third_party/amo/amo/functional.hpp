#ifndef AMO_FUNCTIONAL_HPP
#define AMO_FUNCTIONAL_HPP

#ifndef BOOST_NO_CXX11_HDR_FUNCTIONAL
#define BOOST_NO_CXX11_HDR_FUNCTIONAL
#endif
#ifdef BOOST_NO_CXX11_HDR_FUNCTIONAL
#include <amo/config.hpp>
#else
#include <functional>
#endif


namespace amo
{
#ifdef BOOST_NO_CXX11_HDR_FUNCTIONAL
	using boost::function;
	using boost::bind;
	using boost::ref;
	using boost::hash;
	namespace placeholders {
		/// \todo this feels hacky, is there a better way?
		using ::_1;
		using ::_2;
		using ::_3;
	}
	//namespace placeholders = boost::placeholders;
	// See above definition for more details on what this is and why it exists
#define _AMO_REF(x) boost::ref(x)

	template <typename T>
	void clear_function(T & x) {
		x.clear();
	}
#else
	using std::function;
	using std::bind;
	using std::ref;
	using std::hash;
	namespace placeholders {
		/// \todo this feels hacky, is there a better way?
		using std::placeholders::_1;
		using std::placeholders::_2;
		using std::placeholders::_3;
	}
	//namespace placeholders = boost::placeholders;
	// See above definition for more details on what this is and why it exists
#define _AMO_REF(x) std::ref(x)

	template <typename T>
	void clear_function(T & x) {
		x.clear();
	}
#endif
	
 
}
 
#endif // AMO_FUNCTIONAL_HPP
