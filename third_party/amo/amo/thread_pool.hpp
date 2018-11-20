#ifndef AMO_THREADPOOL_HPP__
#define AMO_THREADPOOL_HPP__

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <amo/logger.hpp>

namespace amo
{
	 
	class thread_pool 
	{
	public:
	 
		thread_pool::thread_pool(size_t threads = 1)
			: stop(false)
		{
			init(threads);
		}
		 
		bool is_running() 
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			return !stop;
		}

		template<class F>
		std::future < typename std::result_of<F()>::type > push(F& f, int thead_id = 0)
		{
			using return_type = typename std::result_of<F()>::type;

			auto task = std::make_shared< std::packaged_task<return_type()> >(f);

			std::future<return_type> res = task->get_future();

			std::unique_lock<std::mutex> lock(queue_mutex);
			// don't allow enqueueing after stopping the pool
			if (stop) throw std::runtime_error("push on stopped thead_pool");

			tasks.emplace([task](){ (*task)(); });


			condition.notify_one();
			return res;
		}


		// the destructor joins all threads
		~thread_pool()
		{
			{
				std::unique_lock<std::mutex> lock(queue_mutex);
				stop = true;
			}
			condition.notify_all();
			for (std::thread &worker : workers)
				worker.join();
		}

		std::vector<std::thread::id> get_ids()
		{
			 
			std::vector<std::thread::id> vec;
			for (auto& p : workers)
			{
				vec.push_back(p.get_id());
			}
			return vec;
			
		}

		void check_if_called_on_valid_thread()
		{
			if (!check_on_thread_pool())
			{
				throw std::runtime_error("method is not called on valid thread");
			}
		}

		bool check_on_thread_pool()
		{
			for (auto& p : workers)
			{
				if (amo::this_thread::get_id() == p.get_id()) return true;
			}
			return false;
			 
		}
	protected:
		void init(size_t threads)
		{
			for (size_t i = 0; i < threads; ++i)
			{
				workers.emplace_back(std::bind(&thread_pool::run, this));
				 
			}
				
		}


		void run()
		{
			 
			try
			{
				while (true)
				{
					std::function<void()> task;

					{
						std::unique_lock<std::mutex> lock(this->queue_mutex);
						this->condition.wait(lock,
							[this]{ return this->stop || !this->tasks.empty(); });
						if (this->stop && this->tasks.empty())
							return;
						task = std::move(this->tasks.front());
						this->tasks.pop();
					}

					task();
				}
			}
			catch (std::exception& e)
			{
				amo::cfatal("[{0}]{1}", amo::this_thread::get_id(), e.what());
				//return;
			}
			
		}
	private:
		std::vector< std::thread > workers;
		
		std::queue< std::function<void()> > tasks;

		std::mutex queue_mutex;
		std::condition_variable condition;
		bool stop;
	};

}

#endif
