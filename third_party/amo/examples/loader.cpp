//#include <amo/amo.hpp>
//#include <amo/loader.hpp>
//
// 
//
//extern "C"
//{
//	typedef int(*fnWin32Project1)();
//	typedef const char* (*Proc_fnTestDll2)(const char*);
//	typedef int* (*ggg)();
//}
//
// 
//
//using namespace std;
//
// 
//#include "../Win32Project2/Win32Project2.h"
//
//int* goo(){
//	return new int(3);
//}
//
// 
//template<typename R>
//class emit
//{
//public:
//	amo::optional<R> exec()
//	{
//		typedef R(*fnType)(); 
//		fnType fn = goo;
//		return fn();
//	}
//		 
//}; 
//
//#include <windows.h>
//
//class ABCE
//{
//public:
//	~ABCE()
//	{
//		std::cout << "fhdfasd" << std::endl;
//	}
//};
//
//template<typename T>
//amo::shared_ptr<T> fffff()
//{
//	amo::optional<T*> t = new ABCE();
//	return amo::shared_ptr<T>(*t);
//}
//
//int main(int argc, char** argv)
//{
//	amo::shared_ptr<ABCE> ccgg = fffff<ABCE>();
//	ABCE* ccd = new ABCE();
//	{
//		amo::shared_ptr<ABCE> b(ccd);;
//	}
//	
//	ggg g = goo;
//	g();
//
//	amo::loader runner3;
//	runner3.load("libcef_node.dll");
//	amo::loader runner4;
//	runner4.load("libcef_node.dll");
//
//	void* lib_handle = LoadLibraryA("Win32Project2.dll");
//	void* sc =(void *)GetProcAddress((HMODULE)lib_handle, "GetXyz");
//	DWORD cc = GetLastError();
//	int* t = *emit<int*>().exec();
//	amo::loader runner;
//	runner.load("Win32Project2.dll");
////	std::cout << *runner.emit<std::string>("foo", 3) << std::endl;
//	std::cout << *(runner.exec<int>("foo")) << std::endl;
//	std::cout << *(runner.exec<int>("foo1", 44)) << std::endl;
//	std::cout << *runner.exec<bool>("foo2", 3, 5) << std::endl;
//	std::cout << *runner.exec<double>("foo3", 4, "2342") << std::endl;
//	std::cout << *runner.exec<text>("foo4", 5, 3.4f, 3.45, "2343")->str() << std::endl;
//	(runner.exec<amo::nil>("foo5", 44));
//	runner.exec<bool>("foo21", 3, 5);
//	IXyz* aaa = *runner.exec<IXyz*>("GetXyz");
//	std::cout << aaa->Foo(3) << std::endl;
//	std::cout << aaa->GetStr() << std::endl;
//	//amo::shared_ptr<IXyz> aaage = runner.get<IXyz>();
//	amo::shared_ptr<IXyz> aaage2  = runner.get<IXyz>(34);
// 
//	amo::loader loader;
//	bool b = loader.load("user32.dll");
//	b = loader.has_symbol("SetWindowPos");
//
//	amo::optional<BOOL> dd =loader.exec<BOOL>("ShowWindow", (HWND)NULL, 4);
//	if (dd)
//	{
//		BOOL c = *dd;
//	}
//
//	//callback = std::bind(&foo, "test", std::placeholders::_1, 3.f);
//	//register_callback(wrapper); // <-- How to do this?
//	//Listen("a", std::function<void(std::string)>(fdd));
//
//	//using namespace std;
//
//	//Test test;
//
//	//try {
//	//	test.Load(_T("Win32Project2.dll"));
//	//}
//	//catch (const ASL::LibraryNotFoundException& e)
//	//{
//	//	cout << "Lib not found " << e.what() << endl;
//	//}
//	//catch (const ASL::SymbolNotFoundException& e) {
//	//	cout << "Sym not found " << e.what() << endl;
//	//}
//
//	////assert(test.shouldFail == NULL);
//	//cout << test.foo() << endl;
//	////cout << test.fnTestDll2("hELLO, WORLD") << endl;
//	//// testFunc函数的签名由此处的实参类型推导出来，int为其返回值类型,  
//	//// 这种调用方式并不安全，慎用！  
//	////cout << test.testFunc<int>(ASL_ARGS_T（(int)1, (int)2.f)) << endl;
//	//test.Unload();
//	return 0;
//}
// 