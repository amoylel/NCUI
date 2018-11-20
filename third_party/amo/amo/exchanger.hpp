#ifndef AMO_EXCHANGER_HPP__
#define AMO_EXCHANGER_HPP__
 
#include <iostream>
#include <amo/thread.hpp>
#include <amo/nil.hpp>
#include <amo/logger.hpp>

namespace amo
{
	// 用于两个线程同步交换数据
	template<typename T>
	class exchanger
	{
	public:
		static T exchange(amo::shared_ptr<exchanger<T> > exc, T value) 
		{
			return exc->exchange(value);
		}
	public:
		exchanger()
		{
			id = amo::this_thread::get_id();
			lhs_ready = rhs_ready = false;
		}

		~exchanger()
		{
			//TODO:释放资源
		}

		T exchange(T value)
		{
			 
			if(id == amo::this_thread::get_id())
			{
				amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
				lhs = value;
				set_lhs_ready(true);
				m_lhs_ready_condtion.notify_one();
				m_rhs_ready_condtion.wait(lock, amo::bind(&exchanger::is_rhs_ready, this));
				set_rhs_ready(false);
				return rhs;
			}
			else
			{
				amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
				rhs = value;
				set_rhs_ready(true);
				m_rhs_ready_condtion.notify_one();
				m_lhs_ready_condtion.wait(lock, amo::bind(&exchanger::is_lhs_ready, this));
				set_lhs_ready(false);
				return lhs;
			}
		}
	private:
		void set_rhs_ready(bool ready)
		{
			amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
			rhs_ready = ready;
		}

		void set_lhs_ready(bool ready)
		{
			amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
			lhs_ready = ready;
		}

		inline bool is_rhs_ready()
		{
			amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
			return rhs_ready;
		}

		inline bool is_lhs_ready()
		{
			amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
			return lhs_ready;
		}

	private:

		T lhs;
		T rhs;
		bool lhs_ready;
		bool rhs_ready;
		amo::thread::id id;
		amo::recursive_mutex  m_mutex;  
		amo::condition_variable_any m_rhs_ready_condtion;  
		amo::condition_variable_any m_lhs_ready_condtion;  
	};
}

#endif // AMO_EXCHANGER_HPP__
