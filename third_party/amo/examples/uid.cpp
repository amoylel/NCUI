//#include <iostream>
//
//#include <amo/uid.hpp>
//#include <amo/looper_executor.hpp>
//#include <amo/launch.hpp>
//#include <amo/timer.hpp>
//
//
//std::vector<int64_t> oSet1;
//std::vector<int64_t> oSet2;
//std::vector<int64_t> oSet3;
//std::vector<int64_t> oSet4;
//std::vector<int64_t> oSet5;
//void foo1()
//{
//	for (size_t i = 0; i < 10000000; ++i)
//	{
//		oSet1.push_back(amo::uid::generate_uid());
//	} 
//}
//
//void foo2()
//{
//	for (size_t i = 0; i < 10000000; ++i)
//	{
//		oSet2.push_back(amo::uid::generate_uid());
//	} 
//}
//
//void foo3()
//{
//	for (size_t i = 0; i < 10000000; ++i)
//	{
//		oSet3.push_back(amo::uid::generate_uid());
//	} 
//}
//
//void foo4()
//{
//	for (size_t i = 0; i < 10000000; ++i)
//	{
//		oSet4.push_back(amo::uid::generate_uid());
//	} 
//}
//
//void foo5()
//{
//	for (size_t i = 0; i < 10000000; ++i)
//	{
//		oSet5.push_back(amo::uid::generate_uid());
//	}
//}
//
//int64_t generate() {
//
//	static int64_t sequence_ = 0;
//	static int64_t lastTimestamp = 0;
//	static int8_t mac = 23;
//	static int64_t lastVal = 0;
//	int64_t now = amo::chrono::duration_cast<amo::chrono::nanoseconds>(amo::chrono::system_clock::now().time_since_epoch()).count();
//  
//	if (sequence_ == 0){
//		lastTimestamp = now;
//	}
//	int64_t val = 0x7fffffffffffffffL & (lastTimestamp << 22);
//	val |= (mac & 0xf) << 19; 		//中间十位是机器代码 
//	val |= sequence_++ & 0x3FFFF;	//最后12位是squenceID
//	if (sequence_ == 0x40000){
//		sequence_ = 0;
//	}
//	if (val < 0) {
//		std::cout << 32 << std::endl;
//	}
//	if (val <= lastVal){
//		std::cout << 32 << std::endl;
//	}
//	lastVal = val;
//	return val;
//}
//
//int main(int argc, char** argv)
//{
//	amo::timer tt;
//
//	for (size_t i = 0; i < 100000000; ++i)
//	{
//		generate();
//	}
//
//	std::cout << tt.elapsed() << std::endl;
//	return 0;
//	//int64_t sq = 0;
//	//std::vector<int64_t> vvv;
//	//for (int i = 0; i < 10000000; ++i)
//	//{
//	//	/*++sq;
//	//	sq = sq % 0xfff;
//	//	if (sq > 4090)
//	//	std::cout << sq << std::endl;*/
//	//	int64_t now = amo::chrono::duration_cast<amo::chrono::nanoseconds>(amo::chrono::system_clock::now().time_since_epoch()).count();
//	//	vvv.push_back(now);
//	//}
//	//auto iter = std::unique(vvv.begin(), vvv.end());
//	//vvv.erase(iter, vvv.end());
//	//std::cout << vvv.size() << std::endl;
//	//return 0;
//	
//	std::cout << amo::uid::generate_uid() << std::endl;
//	std::vector<int64_t> vvv;
//	for (int i = 9; i < 100000000; ++i)
//	{
//		amo::uid::generate_uid();
//		//vvv.push_back(amo::uid::generate_uid());
//	}
//	std::cout << "dd" << tt.elapsed() << std::endl;
//	return 0;
//	for (size_t i = 0; i < 20; ++i)
//	{
//		amo::shared_ptr<amo::looper_executor> executor1(new amo::looper_executor());
//		amo::shared_ptr<amo::looper_executor> executor2(new amo::looper_executor());
//		amo::shared_ptr<amo::looper_executor> executor3(new amo::looper_executor());
//		amo::shared_ptr<amo::looper_executor> executor4(new amo::looper_executor());
//		amo::shared_ptr<amo::looper_executor> executor5(new amo::looper_executor());
//		amo::uid::generate_uid();
//		amo::timer t;
//		executor1->push(foo1);
//		executor2->push(foo2);
//		executor3->push(foo3);
//		executor4->push(foo4);
//		//executor5->push(foo5);
//		amo::this_thread::sleep_for(amo::chrono::seconds(1));
//		 
//		executor1.reset();
//		executor2.reset();
//		executor3.reset();
//		executor4.reset();
//		executor5.reset();
//		//amo::this_thread::sleep_for(amo::chrono::seconds(10));
//		 
//		std::cout << t.elapsed() << std::endl;
//
//		std::vector<int64_t> vec;
//		vec.insert(vec.end(), oSet1.begin(), oSet1.end());
//		vec.insert(vec.end(), oSet2.begin(), oSet2.end());
//		vec.insert(vec.end(), oSet3.begin(), oSet3.end());
//		vec.insert(vec.end(), oSet4.begin(), oSet4.end());
//		vec.insert(vec.end(), oSet5.begin(), oSet5.end());
//		std::cout << "total:" << vec.size() << std::endl;
//		auto iter = std::unique(vec.begin(), vec.end());
//		vec.erase(iter, vec.end());
//		std::cout << "total:" << vec.size() << std::endl;
//		if (vec.size() != 40000000)
//		{
//			std::cout << "err" << std::endl;
//			std::cout << "err" << std::endl;
//			std::cout << "err" << std::endl;
//			std::cout << "err" << std::endl;
//			std::cout << "err" << std::endl;
//		}
//		oSet1.clear();
//		oSet2.clear();
//		oSet3.clear();
//		oSet4.clear();
//		oSet5.clear();
//	}
//	
//	return 0;
//}