// Created by 苏元海 on 08/11/2016.
// Copyright (c) 2016 amoylel. All rights reserved.

#ifndef IME_SMART_MemoRY_POOL_HPP__
#define IME_SMART_MemoRY_POOL_HPP__


#include <deque>
#include <list>
#include <amo/smart_ptr.hpp>
#include <amo/thread.hpp>
#include <amo/looper_executor.hpp>
#include <amo/timer.hpp>

namespace amo
{
	template<typename T>
	class smart_memory_pool
	{
	public:
		typedef typename T::SmartType smart_type;
	public:
		smart_memory_pool(size_t count = 100000)
			: m_increment(count) 
		{
			m_total_size = 0;
			m_b_recovering = false;
			assert(m_increment > 0);
			m_recovery_executor.reset(new amo::looper_executor(1));
			m_not_used_pool.reset(new std::list<smart_type>());
			m_in_used_pool.reset(new std::list<smart_type>());
			m_recovering_pool.reset(new std::list<smart_type>());
			m_recovered_pool.reset(new std::list<smart_type>());
			increase(m_increment);
		}

		std::size_t size()
		{
			amo::unique_lock<amo::mutex> lock(m_mutex);
			return m_total_size;
		}

		smart_type get()
		{
			amo::unique_lock<amo::mutex> lock(m_mutex);
			if (!m_b_recovering && m_not_used_pool->size() < m_increment/2)
			{
				m_b_recovering = true;
				m_recovery_executor->push(amo::bind(&smart_memory_pool::recovery, this));
			}

			if (m_not_used_pool->size() < 20)
			{
				increase(m_increment);
			}

			smart_type ptr = m_not_used_pool->front();
			if (!ptr)
			{
				ptr = create_object();
				on_before_use(ptr);
			}
			
			m_not_used_pool->pop_front();
			m_in_used_pool->push_back(ptr);
			return ptr;
		}

		virtual smart_type create_object()
		{
			smart_type ptr(new T());
			return ptr;
		}

		virtual void on_before_use(smart_type ptr)
		{
			return;
		}

		void increase(size_t count)
		{
			size_t n_size = m_not_used_pool->size();
			m_not_used_pool->resize(n_size + count);
			m_total_size += count;
		}

		void recovery()
		{
			// 交换正在使用和正在回收指针

			{
				amo::unique_lock<amo::mutex> lock(m_mutex);
				m_b_recovering = true;
				m_recovering_pool.swap(m_in_used_pool);
			}

			for (auto iter = m_recovering_pool->begin(); iter != m_recovering_pool->end();)
			{
				if ((*iter).use_count() > 1)
				{
					++iter;
					continue;
				}
				smart_type ptr = *iter;
				on_before_use(ptr);
				m_recovered_pool->push_back(ptr);
				iter = m_recovering_pool->erase(iter);
			}

			{
				amo::unique_lock<amo::mutex> lock(m_mutex);
				//amo::timer t;
				m_in_used_pool->merge(*m_recovering_pool);
				m_not_used_pool->merge(*m_recovered_pool);
				m_recovered_pool->clear();
				m_recovering_pool->clear();
				m_b_recovering = false;
				//std::cout << "resdkafjdas user " << t.elapsed<amo::chrono::microseconds>() << std::endl;
			}
		}
	private:
		size_t m_total_size;
		size_t m_increment;														// 每次自增数
		amo::mutex m_mutex;														// 线程锁
		amo::shared_ptr<amo::looper_executor> m_recovery_executor;				// 回收线程
		bool m_b_recovering;													// 是否正在回收 
		amo::shared_ptr<std::list<smart_type> > m_not_used_pool;				// 未使用序列
		amo::shared_ptr<std::list<smart_type> > m_in_used_pool;					// 正在使用的序列
		amo::shared_ptr<std::list<smart_type> > m_recovering_pool;				// 正在回收的序列
		amo::shared_ptr<std::list<smart_type> > m_recovered_pool;				// 已经回收的序列

	};
}

#endif // IME_SMART_MemoRY_POOL_HPP__
