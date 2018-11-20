#ifndef AMO_AMO_HPP__
#define AMO_AMO_HPP__
#if defined(_WIN32) || defined(_WIN64) ||defined(WIN32) || defined(WIN64)
#	include <WinSock2.h>
#   include <windows.h>
#endif
#include <amo/config.hpp>
#include <amo/platforms.hpp>

//std
#include <amo/array.hpp>
#include <amo/atomic.hpp>
#include <amo/chrono.hpp>
#include <amo/codecvt.hpp>
#include <amo/functional.hpp>
#include <amo/future.hpp>
#include <amo/memory.hpp>
#include <amo/limits.hpp>
#include <amo/random.hpp>
#include <amo/ratio.hpp>
#include <amo/regex.hpp>
#include <amo/stdint.hpp>
#include <amo/system_error.hpp>
#include <amo/thread.hpp>
#include <amo/tuple.hpp>
#include <amo/type_traits.hpp>
#include <amo/unordered_map.hpp>
#include <amo/unordered_set.hpp>

//struct
#include <amo/any.hpp>
#include <amo/base64.hpp>
#include <amo/byte_buffer.hpp>
#include <amo/byte_order.hpp>
#include <amo/circular_buffer.hpp>
#include <amo/delegate.hpp>
#include <amo/exchanger.hpp>
#include <amo/format.hpp>
#include <amo/json.hpp>
#include <amo/launch.hpp>
#include <amo/logger.hpp>
#include <amo/looper_executor.hpp>
#include <amo/md5.hpp>
#include <amo/network.hpp>
#include <amo/noncopyable.hpp>
#include <amo/object.hpp>
#include <amo/pipe.hpp>
#include <amo/signals2.hpp>
#include <amo/singleton.hpp>
#include <amo/string.hpp>
#include <amo/string_builder.hpp>
#include <amo/template_macro.hpp>
#include <amo/time.hpp>
#include <amo/timer.hpp>
#include <amo/tribool.hpp>
#include <amo/uid.hpp>
#include <amo/utility.hpp>
#include <amo/uuid.hpp>
#include <amo/variant.hpp>

#endif // AMO_AMO_HPP__
