#ifndef AMO_RANDOM_HPP
#define AMO_RANDOM_HPP
 
#include <limits>
// - random                   : building
#ifdef BOOST_NO_CXX11_HDR_RANDOM
#include <amo/config.hpp>
#else
#include <random>
#endif

// uniform_smallint:在小整数域内的均匀分布  
// uniform_int:在整数域上的均匀分布  
// uniform_01:在区间[0,1]上的实数连续均匀分布  
// uniform_real:在区间[min,max]上的实数连续均匀分布  
// bernoulli_distribution:伯努利分布  
// binomial_distribution:二项分布  
// cauchy_distribution:柯西（洛伦兹）分布  
// gamma_distribution:伽马分布  
// poisson_distribution:泊松分布  
// geometric_distribution:几何分布  
// triangle_distribution:三角分布  
// exponential_distribution:指数分布  
// normal_distribution:正态分布  
// lognormal_distribution:对数正态分布  
// uniform_on_sphere:球面均匀分布  

 
namespace amo
{
#ifdef BOOST_NO_CXX11_HDR_RANDOM
	using boost::random::random_device;
	using boost::random::uniform_int_distribution;
	using boost::random::uniform_real_distribution;
	using boost::random::normal_distribution;
#else
	using std::random_device;
	using std::uniform_int_distribution;
	using std::uniform_real_distribution;
	using std::normal_distribution;

#endif
	 
	 
	template<typename T = uniform_int_distribution<int> >
	class random
	{
	public:
		random(typename T::result_type min = 0, typename T::result_type max = (std::numeric_limits<typename T::result_type>::max)())
			: m_min(min)
			, m_max(max)
			, gen(min, max)
		{

		}

		typename T::result_type operator()()
		{
			return gen(rng);
		}
	private:
		random_device rng;
		T gen;
		typename T::result_type m_min;
		typename T::result_type m_max;
	};

	template<>
	class random < int >
	{
	public:
		random(int min = 0, int max = (std::numeric_limits<int>::max)())
			: m_min(min)
			, m_max(max)
			, gen(min, max)
		{

		}

		int operator()()
		{
			return gen(rng);
		}

	private:
		random_device rng;
		uniform_int_distribution<> gen;
		int m_min;
		int m_max;
	};

	template<>
	class random<double>
	{
	public:
		random(double min = 0, double max = (std::numeric_limits<double>::max)())
			: m_min(min)
			, m_max(max)
			, gen(min, max)
		{

		}

		double operator()()
		{
			return gen(rng);
		}

	private:
		random_device rng;
		uniform_real_distribution<> gen;
		double m_min;
		double m_max;
	};


	template<>
	class random < float >
	{
	public:
		random(float min = 0, float max = (std::numeric_limits<float>::max)())
			: m_min(min)
			, m_max(max)
			, gen(min, max)
		{

		}

		float operator()()
		{
			return static_cast<float>(gen(rng));
		}

	private:
		random_device rng;
		uniform_real_distribution<> gen;
		float m_min;
		float m_max;
	};
}
#endif // AMO_RANDOM_DEVICE_HPP
