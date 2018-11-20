//#include <iostream>
//#include <amo/delegate.hpp>
//
//void foo()
//{
//	std::cout << __FUNCTION__ << std::endl;
//}
//
//class abc
//{
//public:
//	int foo()
//	{
//		std::cout << __FUNCTION__ << std::endl;
//		return 1;
//	}
//
//	int foo1(int a)
//	{
//		return a;
//	}
//
//	int foo2(int a, int b)
//	{
//		return a + b;
//	}
//};
//
//#include <amo/template_macro.hpp>
//#include <amo/smart_ptr.hpp>
//#include <amo/looper_executor.hpp>
//#include <amo/launch.hpp>
//
//template<typename R, typename P1>
//amo::function<R(void)> make_delegate3(P1 p1)
//{
//	return amo::bind(p1);
//}
//template<typename R, typename P1, typename P2>
//amo::function<R(void)> make_delegate3(P1 p1, P2 p2)
//{
//	 
//	return amo::bind(p1, p2);
//}
//
//
//template<typename R, typename P1, typename P2, typename P3>
//amo::function<R(void)> make_delegate3(P1 p1, P2 p2, P3 p3)
//{
//	return amo::bind(p1, p2, p3);
//}
//
//template<typename R, typename P1, typename P2, typename P3, typename P4>
//amo::function<R(void)> make_delegate3(P1 p1, P2 p2, P3 p3, P4 p4)
//{
//	return amo::bind(p1, p2, p3, p4);
//}
//
//template<typename T>
//class TypeGet
//{
//public:
//	typedef T type;
//};
//
//template<>
//class TypeGet<void>
//{
//public:
//	typedef amo::nil type;
//};
//
//
//#define BIND_GEN222(val)\
//	template <typename R, typename F, typename CB  COMMA##val TEMPLATE_PARAM_TYPE_PURE##val>\
//	void async(F f, CB fn_cb COMMA##val FUNC_PARAM##val)\
//{ \
//	\
//		amo::async<TypeGet<R>::type>(async_executor(), amo::bind(f, dynamic_cast<T*>(this) COMMA##val FUNC_PARAM_VAL##val), fn_cb); \
//}\
//	 
//template<typename T>
//class launch2
//{
//public:
//	virtual ~launch2(){}
//
//	virtual amo::shared_ptr<amo::looper_executor> async_executor() = 0;
//
//	//template <typename R, typename F, typename CB   >
//	//void async(F f, CB fn_cb )
//	//{ 
//	//	amo::async<R>(async_executor(), amo::bind(f, dynamic_cast<T*>(this) COMMA##val FUNC_PARAM_VAL##val, fn_cb)); \
//	//}
//
//	BIND_GEN222(0)
//	BIND_GEN222(1)
//	BIND_GEN222(2)
//	BIND_GEN222(3)
//	BIND_GEN222(4)
//	BIND_GEN222(5)
//	BIND_GEN222(6)
//	BIND_GEN222(7)
//	BIND_GEN222(8)
//	BIND_GEN222(9)
//	BIND_GEN222(10)
//	BIND_GEN222(11)
//	BIND_GEN222(12)
//	BIND_GEN222(13)
//	BIND_GEN222(14)
//	BIND_GEN222(15)
//	BIND_GEN222(16)
//	BIND_GEN222(17)
//	BIND_GEN222(18)
//	BIND_GEN222(19)
//	BIND_GEN222(20)
//};
//
//class dd : public amo::launch<dd>
//	, public launch2<dd>
//{
//public:
//	dd()
//	{
//		m_executor.reset(new amo::looper_executor());
//	}
//	void foo()
//	{
//		std::cout << __FUNCTION__ << std::endl;
//	}
//
//	int foo1(int a)
//	{
//		
//		return a;
//	}
//
//	int foo2(int a, int b)
//	{
//		return a + b;
//	}
//
//	virtual amo::shared_ptr<amo::looper_executor> sync_executor() override
//	{
//		return m_executor;
//	}
//
//	virtual amo::shared_ptr<amo::looper_executor> async_executor() override
//	{
//		return m_executor;
//	}
//
//	amo::shared_ptr<amo::looper_executor> m_executor;
//};
//
//template<typename R, typename T>
//R gcc(T& t)
//{
//	return 3;
//}
//
// 
//void goo(void)
//{
//
//}
//
//void goo1(int d)
//{
//
//}
//
//void goo2(int d)
//{
//
//}
//
//
//template<typename T>
//static void ddg(int c, amo::function<void(T)> fn)
//{
//	return ;
//}
//
//
//static void ddg(int c, std::function<void()> fn)
//{
//	return;
//}
//
//
//
//int main(int argc, char** argv)
//{
//	TypeGet<void>::type c;
//	amo::function<void(void)> fnc(goo); 
//	//ddg(3, goo1);
//	//ddg<void>(3, fnc);
//	dd d;
//	d.sync<void>(&dd::foo);
//	std::cout << d.sync<int>(&dd::foo1, 3) << std::endl;
//	std::cout << d.sync<int>(&dd::foo2, 3, 54) << std::endl;
//	d.async<void>(&dd::foo, goo);
//	//d.async<int>(&dd::foo1, amo::bind(goo1, _1), 3);
//	//d.async<int>(&dd::foo2, amo::bind(goo2, _1), 3, 54);
//	return 0;
//	abc a;
//	GetParamsType<int(abc::*)(int, int)>::type fn54 = amo::bind(&abc::foo2, &a, _1, _2);
//	std::cout << fn54(3, 5) << std::endl;
//	auto fn = amo::make_delegate<void>(foo);
//	auto fn2 = amo::make_delegate<int>(&abc::foo, &a);
//	auto fn3 = amo::make_delegate<int>(&abc::foo2, &a);
//	fn();
//	std::cout << fn2() << std::endl;
//	std::cout << fn3(3, 4) << std::endl;
//	//auto fn4 = make_delegate2<void>(foo);
//	//auto fn5 = make_delegate2(&abc::foo, &a);
//	/*sync<void>(foo);
//	auto fn6 = sync<int>(&abc::foo, &a);
//	auto fn7 = sync<int>(&abc::foo1, &a, 4);
//	auto fn8 = sync<int>(&abc::foo2, &a, 3, 4);*/
//	//fn4();
//	
//	//std::cout << fn7 << std::endl;
//	//std::cout << fn8 << std::endl;
//	return 0;
//}