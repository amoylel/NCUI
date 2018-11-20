//#include <iostream>
//#include <amo/looper_executor.hpp>
//#include <amo/functional.hpp>
//#include <amo/launch.hpp>
//
//
//
//
//int sum(int a, int b)
//{
//	std::cout << __FUNCTION__ << " = " << a + b << std::endl;
//	return a + b;
//}
//
//void foo_nil()
//{
//	std::cout << __FUNCTION__ << std::endl;
//}
//
//class launch_test
//{
//public:
//	void foo()
//	{
//		std::cout << __FUNCTION__ << std::endl;
//	}
//
//	void goo()
//	{
//		std::cout << __FUNCTION__ << std::endl;
//	}
//};
//
//void on_foo(int ret)
//{
//	std::cout << __FUNCTION__ << ret << std::endl;
//}
//
//int foo()
//{
//	std::cout << "foo.." << std::endl;
//	return 10;
//}
//
//void gooo()
//{
//	std::cout << __FUNCTION__ << std::endl;
//}
// 
// 
// 
//int main(int argc, char** argv)
//{
//	if (typeid(void) == typeid(void))
//	{
//		std::cout << "OK" << std::endl;
//	}
// 
//	amo::shared_ptr<amo::looper_executor> executor(new amo::looper_executor()); 
//	
//	//async
//	amo::async<int>(executor, foo);
//
//	amo::async<void>(executor, gooo);
//	amo::this_thread::sleep_for(amo::chrono::seconds(1));
//	return 0;
//
//	//amo::sync<void>(executor, gooo);
//	////int
//	//std::cout << "sync: " << amo::sync<int>(executor, foo) << std::endl;
//	//std::cout << "sync: " << amo::sync(executor, amo::function<int()>(foo)) << std::endl;
//	//std::cout << "sync: " << amo::sync<int>(executor, amo::bind(sum, 3, 6) ) << std::endl;
//	////void
//	//amo::sync<void>(executor, foo_nil);
//	//launch_test test;
//	//amo::async(executor, amo::bind(&launch_test::goo, &test));
//
//	//executor->request_stop();
//	system("pause");
//	return 0;
//}