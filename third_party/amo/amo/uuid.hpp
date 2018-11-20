#ifndef AMO_UUID_HPP__
#define AMO_UUID_HPP__


#include <string>
#include <random>
#include <amo/chrono.hpp>
#include <amo/stdint.hpp>
#include <amo/array.hpp>
#include <amo/uid.hpp>

namespace amo {

    class uuid {
    public:
        typedef amo::array<int32_t, 16> Bytes;
        typedef amo::array<char, 37> CStr;
        
        uuid(void) {
            init_uuid();
        }
        
        uuid(const uuid& rhs)
            :  ab(rhs.ab), cd(rhs.cd) {
            
        }
        
        uuid(const std::string &str) {
            set(str);
        }
        
        template<typename T>
        uuid(boost::optional<T> op_str) {
            ab = cd = 0;
        }
        
        uuid(boost::optional<const std::string&> op_str) {
            ab = cd = 0;
            
            if (!op_str) {
                return;
            }
            
            *this = from_string(*op_str);
        }
        
        
        uuid(boost::optional<amo::uuid> op) {
            ab = cd = 0;
            
            if (!op) {
                return;
            }
            
            *this = *op;
        }
        
        
        uuid(boost::optional<const amo::uuid> op) {
            ab = cd = 0;
            
            if (!op) {
                return;
            }
            
            *this = *op;
        }
        
        
        uuid(boost::optional<const amo::uuid&> op) {
            ab = cd = 0;
            
            if (!op) {
                return;
            }
            
            *this = *op;
        }
        
        void set(const std::string &str) {
            char s[2] = { 0 };
            int string_index[] = { 0, 2, 4, 6, 9, 11, 14, 16, 19, 21, 24, 26, 28, 30, 32, 34 };
            
            for (size_t i = 0; i < 16; ++i) {
                auto index = string_index[i];
                s[0] = str.at(index);
                s[1] = str.at(index + 1);
                
                if (i < 8) {
                    ab <<= 8;
                    ab += (int32_t) std::strtol(s, nullptr, 16);
                } else {
                    cd <<= 8;
                    cd += (int32_t) std::strtol(s, nullptr, 16);
                }
                
            }
        }
        
        
        
        
        
        std::string to_string() const {
            //static char const mask[] = "00000000-0000-0000-0000-000000000000";
            std::string str = "00000000-0000-0000-0000-000000000000";
            
            //CStr str;
            //memccpy(&str[0], mask, 0, str.size());
            int string_indexes[16] = { 0, 2, 4, 6, 9, 11, 14, 16, 19, 21, 24, 26, 28, 30, 32, 34 };
            auto hex_chars = "0123456789abcdef";
            
            uint64_t high = ab;
            uint64_t low = cd;
            
            amo::array<int32_t, 16> bytes_;
            
            for (int i = 0; i < 8; ++i) {
                bytes_[7 - i] = high & 0xff;
                high >>= 8;
                
            }
            
            for (int i = 0; i < 8; ++i) {
                bytes_[15 - i ] = low & 0xff;
                low >>= 8;
            }
            
            
            for (int32_t i = 0; i < 16; ++i) {
                auto string_index = string_indexes[i];
                int32_t dec = bytes_[i];
                str[string_index] = hex_chars[(dec >> 4) & (0xF)];
                str[string_index + 1] = hex_chars[(dec & 0xF)];
            }
            
            return str;
        }
        
        
        
        bool compare(const uuid &other) const {
            return (ab == other.ab && cd == other.cd);
        }
        
        bool operator==(const amo::uuid &other) const {
            return compare(other);
        }
        
        bool operator!=(const amo::uuid &other) const {
            return !compare(other);
            
        }
        
        bool operator < (const amo::uuid& other) const {
            return this->to_string() < other.to_string();
            //return true;
        }
        friend std::ostream& operator<<(std::ostream&os, const amo::uuid& id) {
            return os << id.to_string() ;
        }
        
        static amo::uuid nil() {
            return from_string("00000000-0000-0000-0000-000000000000");
        }
        
        static amo::uuid from_string(std::string str) {
            return amo::uuid(str);
        }
        
        static amo::uuid generate_uuid() {
            return amo::uuid();
        }
        
    private:
        void init_uuid() {
            static std::mt19937 random_engine((uint32_t)(amo::chrono::high_resolution_clock::now().time_since_epoch().count() & 0xffffffff));
            static std::uniform_int_distribution<uint32_t> bits(0, (uint32_t)(~0));
            
            ab = (uint64_t)bits(random_engine) << 32;
            ab |= bits(random_engine);
            cd = amo::uid::generate_uid();
            ab = (ab & 0xFFFFFFFFFFFF0FFFULL) | 0x0000000000004000ULL;
            cd = (cd & 0x3FFFFFFFFFFFFFFFULL) | 0x8000000000000000ULL;
            
        }
    public:
        uint64_t ab;
        uint64_t cd;
    };
}


#endif // AMO_UUID_HPP__