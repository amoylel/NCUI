//#include <iostream>
//
//#include <amo/looper_executor.hpp>
//#include <amo/functional.hpp>
//
//
//namespace
//{
//	void foo()
//	{
//		std::cout << __FUNCTION__ << std::endl;
//	}
//
//	void foo(int a, int b)
//	{
//		std::cout << __FUNCTION__ << a << "--" << b << std::endl;
//	}
//
//
//	class test
//	{
//	public:
//		void foo()
//		{
//			std::cout << __FUNCTION__ << std::endl;
//		}
//		void foo(int a, int b)
//		{
//			std::cout << __FUNCTION__ << a << "--" << b << std::endl;
//		}
//	};
//
//
//}
//
//
//int main(int argc, char** argv)
//{
//	amo::looper_executor executor;
//	executor.request_start();
//	executor.execute(amo::bind(foo));
//	executor.execute(amo::bind(foo, 3, 65));
//	test t;
//	executor.execute(amo::bind(&test::foo, &t));
//	executor.execute(amo::bind(&test::foo, &t, 4, 7));
//
//	//delay
//	executor.execute(amo::bind(foo), 600);
//
//	//lambda
//#if 0//_AMO_NO_CPP11_
//	//executor.execute(std::cout << 3 << std::endl;);
//#else
//	executor.execute([](){std::cout << 3 << std::endl;});
//	amo::function<void(int, int)> fn = [&](int a, int b){std::cout << a << ":" << b << std::endl;};
//	executor.execute(amo::bind(fn, 3, 6));
//	executor.execute(amo::bind(amo::function<void(int, int)>([](int a, int b){
//		std::cout << a << ":" << b << std::endl;
//	}), 31, 61));
//
//#endif
//
//	amo::this_thread::sleep_for(amo::chrono::seconds(5));
//	system("pause");
//}