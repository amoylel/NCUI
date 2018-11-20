//#include <iostream>
//#include <amo/looper_executor.hpp>
//#include <amo/exchanger.hpp>
//#include <amo/functional.hpp>
//
//
//
//void foo(amo::shared_ptr<amo::exchanger<int> > result)
//{
//	int num = amo::exchanger<int>::exchange(result, 5);
//	std::cout << "data form main thread: " << num << std::endl;
//	num = amo::exchanger<int>::exchange(result, 6);
//	std::cout << "data form main thread: " << num << std::endl;
//	return ;
//}
//
//
//void goo(amo::shared_ptr<amo::exchanger<amo::nil> > result)
//{
//	std::cout << "wait for main thread." << std::endl;
//	amo::exchanger<amo::nil>::exchange(result, amo::nil());
//	std::cout << "continue..." << std::endl;
//}
//
//
//int main(int argc, char** argv)
//{
//	amo::looper_executor executor;
// 
//	amo::shared_ptr<amo::exchanger<int> > result(new amo::exchanger<int>());
//	amo::shared_ptr<amo::exchanger<amo::nil> > result2(new amo::exchanger<amo::nil>());
//
//	for (size_t i = 0; i < 1000; ++i)
//	{
//		executor.execute(amo::bind(foo, result));
//		int num = amo::exchanger<int>::exchange(result, 8);
//		amo::this_thread::sleep_for(amo::chrono::milliseconds(10));
//		std::cout << "data from sub thread: " << num << std::endl;
//		//amo::this_thread::sleep_for(amo::chrono::seconds(1));
//		num = amo::exchanger<int>::exchange(result, 9);
//		amo::this_thread::sleep_for(amo::chrono::milliseconds(10));
//		std::cout << "data from sub thread: " << num << std::endl;
//
//
//		executor.execute(amo::bind(goo, result2)/*, 7000*/);
//		//amo::this_thread::sleep_for(amo::chrono::milliseconds(5000));
//		amo::exchanger<amo::nil>::exchange(result2, amo::nil());
//	}
//	
//
//	system("pause");
//	return 0;
//}