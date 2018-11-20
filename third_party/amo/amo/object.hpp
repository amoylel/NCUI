#ifndef AMO_OBJECT_HPP__
#define AMO_OBJECT_HPP__

#include <amo/uid.hpp>
#include <amo/logger.hpp>
#include <set>


namespace amo {
    class object {
    public:
        object() : valid_object(true) { }
        
        virtual ~object() {}
        
        operator bool() {
            return is_valid();
        }
        
        object& operator=(bool valid_) {
            set_valid(!valid_);
            return *this;
        }
        
        void set_last_error(int64_t last_error_) {
            last_error = last_error_;
        }
        
        const int64_t& get_last_error() const {
            return last_error;
        }
        
        amo::uid get_object_id() const {
            return object_id;
        }
        
        void set_valid(bool invalid = true) {
            valid_object = invalid;
        }
        
        bool is_valid() const {
            return valid_object;
        }
        
        
    protected:
        bool valid_object;
        int64_t last_error;
        amo::uid object_id;
    };
    
}



#endif // AMO_OBJECT_HPP__
