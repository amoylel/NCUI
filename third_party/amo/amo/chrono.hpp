#ifndef AMO_CHRONO_HPP__
#define AMO_CHRONO_HPP__

#include <amo/stdint.hpp>
#include <amo/ratio.hpp>
// - chrono                   : building
#ifdef BOOST_NO_CXX11_HDR_CHRONO
#include <amo/config.hpp>
#else
#include <chrono>
#endif

namespace amo
{
	namespace chrono
	{
#ifdef BOOST_NO_CXX11_HDR_CHRONO
		using namespace boost::chrono; 
#else
		using namespace std::chrono;  
#endif
		typedef amo::chrono::duration
			<int, amo::ratio_multiply<amo::ratio<24>, amo::chrono::hours::period> > days;

		typedef amo::chrono::duration
			<int, amo::ratio_multiply<amo::ratio<7>, days::period> > weeks;

		typedef amo::chrono::duration
			<int, amo::ratio_multiply<amo::ratio<146097, 400>, days::period> > years;

		typedef  amo::chrono::duration
			<int, amo::ratio_divide<years::period, amo::ratio<12> > > months;

	}
 
  
} // namespace amo
 
 
#endif // AMO_CHRONO_HPP__
