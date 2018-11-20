//#include <iostream>
//
//#include <amo/sigsolt.hpp>
//
//void foo()
//{
//	std::cout << __FUNCTION__ << std::endl;
//}
//
//void foo1(int a)
//{
//	std::cout << __FUNCTION__ << std::endl;
//}
//void foo2(int a, int b)
//{
//	std::cout << __FUNCTION__ << std::endl;
//}
//void foo3(int a, int b, int c)
//{
//	std::cout << __FUNCTION__ << std::endl;
//}
//
//void foo4(int a, int& b)
//{
//	b = a; 
//}
//
//void foo5(amo::shared_ptr<int> ptr)
//{
//	*ptr = 3;
//}
//
//int foo7(int a)
//{
//	return a;
//}
//
//class abcdef
//{
//
//};
//int64_t foo8(abcdef a)
//{
//	return 234324;
//}
//#include <amo/nil.hpp>
//
//
//
//int main(int argc, char** argv)
//{
//	amo::optional<int> opt;
//	if (opt)
//	{
//		int c = *opt;
//	}
//	std::cout << typeid(int&).name() << std::endl;
//	amo::signal sig;
//	amo::slot slt;
//	slt.listen<void(void)>(1, amo::bind(foo));
//	slt.listen<void(int)>(2, amo::bind(foo1, amo::placeholders::_1));
//	slt.listen<void(int, int)>(3, amo::bind(foo2, amo::placeholders::_1, ::_2));
//	slt.listen<void(int, int, int)>(4, amo::bind(foo3, amo::placeholders::_1, ::_2, ::_3));
//	slt.listen<void(int, int&)>(5, amo::bind(foo4, _1, _2));
//	slt.listen<void(amo::shared_ptr<int>)>(6, amo::bind(foo5, _1));
//	slt.listen<int(int)>(7, amo::bind(foo7, _1));
//	slt.listen<int64_t(abcdef)>(8, amo::bind(foo8, _1));
//	sig.connect(&slt);
//	//sig.emit<void>(1);
//	/*sig.emit<void>(2, 3);
//	sig.emit<void>(3, 4, 5);
//	sig.emit<void>(4, 5, 6, 7);*/
//	int c = 0;
//	//sig.emit<void>(5, 3, c);
//	amo::shared_ptr<int> ptr(new int(0));
//	auto p = sig.emit<amo::nil>(2, 3);
//	auto p2 = sig.emit<int>(7,7);
//	abcdef cddd;
//	auto p3 = sig.emit<int64_t>(8, cddd);
//	int64_t ddew = *p3;
//	int cb = *p2;
//	return 0;
//}