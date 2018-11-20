#ifndef AMO_TEXT_HPP__
#define AMO_TEXT_HPP__

#define  TEXT_GEN(val)\
struct text##val\
{\
public:\
	text##val()\
	{\
		reset();\
	}\
	void reset()\
	{\
		memset(&t, 0, (1 << val) - 1);\
	}\
	char* str()\
	{\
		return t;\
	}\
	const char* str() const\
	{\
		return t;\
	}\
	\
	char t[(1 << val) -1];\
};


extern "C"
{
	TEXT_GEN(8)  //256 -1
	TEXT_GEN(10) //1024 - 1
	TEXT_GEN(12) //4096 - 1
	TEXT_GEN(16) //65536 - 1

	typedef text8 text;
	typedef text10 text1k;
	typedef text12 text4k;
	typedef text16 text64k;
}

#endif // AMO_TEXT_HPP__
