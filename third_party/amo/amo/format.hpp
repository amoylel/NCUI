#ifndef AMO_FORMAT_HPP__
#define AMO_FORMAT_HPP__



#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY
#endif


#include <amo/config.hpp>
#include <fmt/format.h>

namespace amo {
    using namespace fmt;
    
    typedef fmt::Writer writer;
    typedef fmt::WWriter wwriter;
    typedef fmt::StringRef string_ref;
    typedef fmt::WCStringRef wstring_ref;
    typedef fmt::CStringRef cstring_ref;
    typedef fmt::WStringRef wcstring_ref;
    typedef fmt::MemoryWriter memory_writer;
    typedef fmt::WMemoryWriter wmemory_writer;
    typedef fmt::ArrayWriter array_writer;
    typedef fmt::WArrayWriter warray_writer;
    
}
//namespace amo = fmt;




#endif // AMO_FORMAT_HPP__
