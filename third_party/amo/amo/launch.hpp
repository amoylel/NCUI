#ifndef AMO_LAUNCH_HPP__
#define AMO_LAUNCH_HPP__

 
#include <amo/functional.hpp>
#include <amo/memory.hpp>
#include <amo/looper_executor.hpp>
#include <amo/logger.hpp>
#include <amo/exchanger.hpp>
#include <amo/chrono.hpp>

#include <amo/template_macro.hpp>

#define SYNC_LAUNCH_FUN(val)\
	template <typename R, typename F  COMMA##val TEMPLATE_PARAM_TYPE_PURE##val>\
	R sync(F f COMMA##val FUNC_PARAM##val)\
{ \
	return amo::sync<R>(sync_executor(), amo::bind(f, dynamic_cast<T*>(this) COMMA##val FUNC_PARAM_VAL##val));\
}\

namespace amo
{
	// 同步执行器
	template<typename T>
	class sync_runner
	{
	public:
		T operator()(amo::shared_ptr<amo::exchanger<T> > result, amo::function<T()> fn)
		{
			return amo::exchanger<T>::exchange(result, fn());
		}
	};

	// 同步执行器的void物化版本
	template<>
	class sync_runner < amo::nil >
	{
	public:
		void operator()(amo::shared_ptr< amo::exchanger<amo::nil> > result, amo::function<void()> fn)
		{
			fn();
			amo::exchanger<amo::nil>::exchange(result, amo::nil());
		}
	};

 

	// 异步执行器
	template<typename T>
	class async_runner
	{
	public:
		void operator()(amo::function<T()> fn, amo::function<void(T)> fn_cb)
		{
			T ret = fn();
			if (fn_cb) fn_cb(ret);
		}
	};

	// 异步执行void特化版本
	template<>
	class async_runner < amo::nil >
	{
	public:
		void operator()(amo::function<void()> fn, amo::function<void(void)> fn_cb)
		{
			fn();
			if (fn_cb) fn_cb();
		}
	};

	template<>
	class async_runner < void>
	{
	public:
		void operator()(amo::function<void()> fn, amo::function<void(void)> fn_cb)
		{
			fn();
			if (fn_cb) fn_cb();
		}
	};

	// 同步执行函数
	template<typename T>
	static T sync(amo::shared_ptr<looper_executor> executor, amo::function<T()> fn, uint64_t delay = 0)
	{
		if(!executor)  return T();
		if(executor->check_on_looper_thread())
		{
			if(delay <= 0)
			{
				return fn();
			}
			else
			{
				// 不能在同一线程中进行数据交换
				throw std::runtime_error("method is not called on valid thread");
			}
		}
		
		amo::shared_ptr<amo::exchanger<T> > result(new amo::exchanger<T>());
		sync_runner<T> run;
		executor->execute(amo::bind(&sync_runner<T>::operator (), &run, result, fn), delay);
		return amo::exchanger<T>::exchange(result, T());
		
		
	}
	 
	// 同步执行函数 void 特化版
	template<>
	static void sync<void>(amo::shared_ptr<looper_executor> executor, amo::function<void()> fn, uint64_t delay)
	{
		if(!executor) return ; 
		if(executor->check_on_looper_thread())
		{
			if(delay <= 0)
			{
				return fn();
			}
			else
			{
				// 不能在同一线程中进行数据交换
				throw std::runtime_error("method is not called on valid thread");
			}
		}
		amo::shared_ptr<amo::exchanger<amo::nil> > result(new amo::exchanger<amo::nil>());
		sync_runner<amo::nil> run;
		executor->execute(amo::bind(&sync_runner<amo::nil>::operator (), &run, result, fn), delay);
		amo::exchanger<amo::nil>::exchange(result,  amo::nil());
		return;
	}


	template<typename T>
	static T sync(amo::shared_ptr<looper_executor> executor, amo::function<T()> fn, amo::chrono::time_point<amo::chrono::system_clock> execute_time)
	{
		if(!executor)  return T();
		if(executor->check_on_looper_thread())
		{
			if(execute_time <= amo::chrono::system_clock::now())
			{
				return fn();
			}
			else
			{
				// 不能在同一线程中进行数据交换
				throw std::runtime_error("method is not called on valid thread");
			}
		}
		amo::shared_ptr<amo::exchanger<T> > result(new amo::exchanger<T>());
		sync_runner<T> run;
		executor->execute(amo::bind(&sync_runner<T>::operator (), &run, result, fn), execute_time);
		return amo::exchanger<T>::exchange(result, T());
	}

	template<>
	static void sync<void>(amo::shared_ptr<looper_executor> executor, amo::function<void()> fn, amo::chrono::time_point<amo::chrono::system_clock> execute_time)
	{
		if(!executor) return ; 
		if(executor->check_on_looper_thread())
		{
			if(execute_time <= amo::chrono::system_clock::now())
			{
				return fn();
			}
			else
			{
				// 不能在同一线程中进行数据交换
				throw std::runtime_error("method is not called on valid thread");
			}
		}
		amo::shared_ptr<amo::exchanger<amo::nil> > result(new amo::exchanger<amo::nil>());
		sync_runner<amo::nil> run;
		executor->execute(amo::bind(&sync_runner<amo::nil>::operator (), &run, result, fn), execute_time);
		amo::exchanger<amo::nil>::exchange(result,  amo::nil());
		return;

	}

	//template<typename T>
	//static void async(amo::shared_ptr<looper_executor> executor, amo::function<T()> fn)
	//{
	//	if (!executor)  return;
	//	async_runner<T> run;
	//	executor->execute(amo::bind(&async_runner<T>::operator (), &run, fn, amo::function<void(T)>()), 0);
	//}

	//template<>
	//static void async<void>(amo::shared_ptr<looper_executor> executor, amo::function<void()> fn)
	//{
	//	if (!executor) return;
	//	async_runner<amo::nil> run;
	//	executor->execute(amo::bind(&async_runner<amo::nil>::operator (), &run, fn, amo::function<void()>()), 0);
	//}

	template<typename T>
	static void async(amo::shared_ptr<looper_executor> executor, amo::function<T()> fn, amo::function<void(T)> fn_cb = amo::function<void(T)>(), uint64_t delay = 0)
	{
		if(!executor)  return; 
		async_runner<T> run;
		executor->execute(amo::bind(&async_runner<T>::operator (), &run, fn, fn_cb), delay);
	}
	 
	
	static void async(amo::shared_ptr<looper_executor> executor, amo::function<void()> fn, amo::function<void()> fn_cb = amo::function<void()>(), uint64_t delay = 0)
	{
		if (!executor) return;
		async_runner<amo::nil> run;
		executor->execute(amo::bind(&async_runner<amo::nil>::operator (), &run, fn, fn_cb), delay);
	}

	template<typename T>
	static void async(amo::shared_ptr<looper_executor> executor, amo::function<T()> fn, amo::function<void(T)> fn_cb, amo::chrono::time_point<amo::chrono::system_clock> execute_time)
	{
		if(!executor)  return; 
		async_runner<T> run;
		executor->execute(amo::bind(&async_runner<T>::operator (), &run, fn, fn_cb), execute_time);
	}

	 
	static void async(amo::shared_ptr<looper_executor> executor, amo::function<void()> fn, amo::function<void(void)> fn_cb, amo::chrono::time_point<amo::chrono::system_clock> execute_time)
	{
		if(!executor) return ;
		async_runner<amo::nil> run;
		executor->execute(amo::bind(&async_runner<amo::nil>::operator (), &run, fn, fn_cb), execute_time);
	}

	template<typename T>
	static void exec(amo::shared_ptr<looper_executor> executor, amo::function<T()> fn, uint64_t delay = 0)
	{
		if (!executor)  return;
		executor->execute(fn, delay);
	}

	static void exec(amo::shared_ptr<looper_executor> executor, amo::function<void()> fn, uint64_t delay = 0)
	{
		if (!executor) return; 
		executor->execute(fn, delay);
	}

	template<typename T>
	static void exec(amo::shared_ptr<looper_executor> executor, amo::function<T()> fn, amo::chrono::time_point<amo::chrono::system_clock> execute_time)
	{
		if (!executor)  return; 
		executor->execute(fn, execute_time);
	}

	static void exec(amo::shared_ptr<looper_executor> executor, amo::function<void()> fn, amo::chrono::time_point<amo::chrono::system_clock> execute_time)
	{
		if (!executor) return;
		executor->execute(fn, execute_time);
	}

	static bool check_on_looper_thread (amo::shared_ptr<amo::looper_executor>& executor)
	{
		if(!executor) return false;
		return executor->check_on_looper_thread();
	}

	static void check_if_called_on_valid_thread(amo::shared_ptr<amo::looper_executor>& executor)
	{
		if(!check_on_looper_thread(executor)) 
		{
			throw std::runtime_error("method is not called on valid thread");
		}
	}

	template<typename T>
	class launch
	{
	public:
		virtual ~launch(){}

		virtual amo::shared_ptr<amo::looper_executor> sync_executor() = 0;

		SYNC_LAUNCH_FUN(0)
		SYNC_LAUNCH_FUN(1)
		SYNC_LAUNCH_FUN(2)
		SYNC_LAUNCH_FUN(3)
		SYNC_LAUNCH_FUN(4)
		SYNC_LAUNCH_FUN(5)
		SYNC_LAUNCH_FUN(6)
		SYNC_LAUNCH_FUN(7)
		SYNC_LAUNCH_FUN(8)
		SYNC_LAUNCH_FUN(9)
		SYNC_LAUNCH_FUN(10)
		SYNC_LAUNCH_FUN(11)
		SYNC_LAUNCH_FUN(12)
		SYNC_LAUNCH_FUN(13)
		SYNC_LAUNCH_FUN(14)
		SYNC_LAUNCH_FUN(15)
		SYNC_LAUNCH_FUN(16)
		SYNC_LAUNCH_FUN(17)
		SYNC_LAUNCH_FUN(18)
		SYNC_LAUNCH_FUN(19)
		SYNC_LAUNCH_FUN(20)

	};
}


#endif // AMO_LAUNCH_HPP__
