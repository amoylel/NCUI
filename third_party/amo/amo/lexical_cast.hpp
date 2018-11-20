#ifndef AMO_LEXICAL_CAST_HPP
#define AMO_LEXICAL_CAST_HPP


#include <amo/config.hpp>


namespace amo {
    template <typename Target, typename Source>
    inline Target lexical_cast(const Source &arg) {
        try {
            return boost::lexical_cast<Target>(arg);
        } catch (boost::bad_lexical_cast&) {
            return Target();
            //return  boost::value_initialized<Target> ();
        }
        
    }
}

#endif // AMO_LEXICAL_CAST_HPP
