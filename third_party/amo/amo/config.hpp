// Created by amoylel on 17/10/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_CONFIG_73D8AB37_1EB4_43CD_A2C5_6ABAECBBA759_HPP__
#define AMO_CONFIG_73D8AB37_1EB4_43CD_A2C5_6ABAECBBA759_HPP__



// Include standard C++ headers.
// This is the minimal set of required headers to make -based codes compile.
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <cmath>
#include <cfloat>
#include <climits>
#include <ctime>
#include <exception>
#include <algorithm>

// Detect/configure OS variables.
//
// Define 'AMO_OS' to: '0' for an unknown OS (will try to minize library dependencies).
//                      '1' for a Unix-like OS (Linux, Solaris, BSD, MacOSX, Irix, ...).
//                      '2' for Microsoft Windows.
//                      (auto-detection is performed if 'AMO_OS' is not set by the user).
#ifndef AMO_OS
#if defined(unix)        || defined(__unix)      || defined(__unix__) \
 || defined(linux)       || defined(__linux)     || defined(__linux__) \
 || defined(sun)         || defined(__sun) \
 || defined(BSD)         || defined(__OpenBSD__) || defined(__NetBSD__) \
 || defined(__FreeBSD__) || defined (__DragonFly__) \
 || defined(sgi)         || defined(__sgi) \
 || defined(__MACOSX__)  || defined(__APPLE__) \
 || defined(__CYGWIN__)
#define AMO_OS 1
#elif defined(_MSC_VER) || defined(WIN32)  || defined(_WIN32) || defined(__WIN32__) \
   || defined(WIN64)    || defined(_WIN64) || defined(__WIN64__)
#define AMO_OS 2
#else
#define AMO_OS 0
#endif
#elif !(AMO_OS==0 || AMO_OS==1 || AMO_OS==2)
#error AMO Library: Invalid configuration variable 'AMO_OS'.
#error (correct values are '0 = unknown OS', '1 = Unix-like OS', '2 = Microsoft Windows').
#endif


// Disable silly warnings on some Microsoft VC++ compilers.
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4127)
#pragma warning(disable:4244)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
#pragma warning(disable:4319)
#pragma warning(disable:4512)
#pragma warning(disable:4571)
#pragma warning(disable:4640)
#pragma warning(disable:4706)
#pragma warning(disable:4710)
#pragma warning(disable:4800)
#pragma warning(disable:4804)
#pragma warning(disable:4820)
#pragma warning(disable:4996)

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE 1
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE 1
#endif
#endif



#if defined(_WIN32) || defined(_WIN64) ||defined(WIN32) || defined(WIN64)
#	include <WinSock2.h>
#   include <windows.h>
#endif
//以下类需要编译使用
//- atomic : building
//- chrono : building
//- container : building
//- context : building
//- coroutine : building
//- coroutine2 : building
//- date_time : building
//- exception : building
//- filesystem : building
//- graph : building
//- graph_parallel : building
//- iostreams : building
//- locale : building
//- log : building
//- math : building
//- mpi : building
//- program_options : building
//- python : building
//- random : building
//- regex : building
//- serialization : building
//- signals : building
//- system : building
//- test : building
//- thread : building
//- timer : building
//- type_erasure : building
//- wave : building

#include <boost/config.hpp>
#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/convert.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/signals2.hpp>
#include <boost/ref.hpp>
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/limits.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/scoped_array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/pointer_cast.hpp>
#include <boost/ratio.hpp>
#include <boost/cstdint.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/aligned_storage.hpp>
#include <boost/optional/optional.hpp>
#include <boost/variant.hpp>
#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/bimap.hpp>
#include <boost/utility.hpp>
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/future.hpp>
#include <boost/optional.hpp>


#if defined(_WIN32) || defined(_WIN64) ||defined(WIN32) || defined(WIN64)
#	include <WinSock2.h>
#   include <windows.h>
#   include <process.h>
#   include <iphlpapi.h>
#   pragma comment(lib,"iphlpapi.lib")
#   define $windows $yes
#ifndef OS_WIN
#define OS_WIN
#endif
#elif defined(__FreeBSD__) || defined(__NetBSD__) || \
	defined(__OpenBSD__) || defined(__MINT__) || defined(__bsdi__)
#   include <ifaddrs.h>
#   include <net/if_dl.h>
#   include <sys/socket.h>
#   include <sys/time.h>
#   include <sys/types.h>
#   define $bsd $yes
#ifndef OS_BSD
#define OS_BSD
#endif
#elif (defined(__APPLE__) && defined(__MACH__))
#   include <ifaddrs.h>
#   include <net/if_dl.h>
#   include <sys/socket.h>
#   include <sys/time.h>
#   include <sys/types.h>
#   include <unistd.h>
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"
#   define $osx $yes
#ifndef OS_OSX
#define OS_OSX
#endif
#elif defined(__linux__)
#   include <arpa/inet.h>
#   include <net/if.h>
#   include <netinet/in.h>
#   include <sys/ioctl.h>
#   include <sys/socket.h>
#   include <sys/sysinfo.h>
#   include <sys/time.h>
#   include <unistd.h>
#	include <sys/types.h>
#	include <netinet/in.h>
#   define $linux $yes
#ifndef OS_LINUX
#define OS_LINUX
#endif
#else //elif defined(__unix__)
#   if defined(__VMS)
#      include <ioctl.h>
#      include <inet.h>
#   else
#      include <sys/ioctl.h>
#      include <arpa/inet.h>
#   endif
#   if defined(sun) || defined(__sun)
#      include <sys/sockio.h>
#   endif
#   include <net/if.h>
#   include <net/if_arp.h>
#   include <netdb.h>
#   include <netinet/in.h>
#   include <sys/socket.h>
#   include <sys/time.h>
#   include <sys/types.h>
#   include <unistd.h>
#   if defined(__VMS)
namespace {
    enum { MAXHOSTNAMELEN = 64 };
}
#   endif
#   define $unix $yes
#ifndef OS_UNIX
#define OS_UNIX
#endif
#endif
#include <iostream>

#ifdef _MSC_VER
#   define $msvc  $yes
#endif

#if defined(__GNUC__) && (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__ <= 40902 )
namespace std {
    static std::string put_time(const std::tm* tmb, const char* fmt) {
        std::string s(128, '\0');
        
        while (!strftime(&s[0], s.size(), fmt, tmb)) {
            s.resize(s.size() + 128);
        }
        
        return s;
    }
}
#endif

////////////////////////////////////////////////////////////////////////////////////



#ifdef  $windows
#define $welse   $no
#else
#define $windows $no
#define $welse   $yes
#endif

#ifdef  $bsd
#define $belse   $no
#else
#define $bsd     $no
#define $belse   $yes
#endif

#ifdef  $linux
#define $lelse   $no
#else
#define $linux   $no
#define $lelse   $yes
#endif

#ifdef  $unix
#define $uelse   $no
#else
#define $unix    $no
#define $uelse   $yes
#endif

#ifdef  $osx
#define $oelse   $no
#else
#define $osx     $no
#define $oelse   $yes
#endif

#ifdef  $msvc
#define $melse   $no
#else
#define $msvc    $no
#define $melse   $yes
#endif

#define $yes(...) __VA_ARGS__
#define $no(...)



#endif //AMO_CONFIG_73D8AB37_1EB4_43CD_A2C5_6ABAECBBA759_HPP__
