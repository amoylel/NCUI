#ifndef AMO_STRING_BUILDER_HPP__
#define AMO_STRING_BUILDER_HPP__

#include <list>
#include <string>
#include <numeric>

namespace amo {

    // Subset of http://msdn.microsoft.com/en-us/library/system.text.stringbuilder.aspx
    
    class string_builder {
        typedef std::list<std::string> container_t; // Reasons not to use vector below.
        typedef std::string::size_type size_type; // Reuse the size type in the string.
        container_t m_Data;
        size_type m_totalSize;
        void append_impl(const std::string &src) {
            m_Data.push_back(src);
            m_totalSize += src.size();
        }
        // No copy constructor, no assignement.
        string_builder(const string_builder &);
        string_builder & operator = (const string_builder &);
    public:
        string_builder(const std::string &src) {
            if (!src.empty()) {
                m_Data.push_back(src);
            }
            
            m_totalSize = src.size();
        }
        string_builder() {
            m_totalSize = 0;
        }
        
        template<typename T>
        string_builder& operator+(const T& val) {
            std::stringstream stream;
            stream << val;
            append_impl(stream.str());
            return *this;
        }
        
        template<typename T>
        string_builder& operator+=(const T& val) {
            std::stringstream stream;
            stream << val;
            append_impl(stream.str());
            return *this;
        }
        
        operator std::string() {
            return to_string();
        }
        
        friend std::ostream& operator << (std::ostream&os, const string_builder& sb) {
            return os << sb.to_string();
        }
        
        
        string_builder & append(const std::string &src) {
            append_impl(src);
            return *this; // allow chaining.
        }
        
        string_builder& append(const char* val, size_t size) {
            append_impl(std::string(val, size));
            return *this;
        }
        
        string_builder & append(const char& src) {
            std::string str;
            str += src;
            append_impl(str);
            return *this; // allow chaining.
        }
        // This one lets you add any STL container to the string builder.
        template<class inputIterator>
        string_builder & add(const inputIterator &first,
                             const inputIterator &afterLast) {
            // std::for_each and a lambda look like overkill here.
            // <b>Not</b> using std::copy, since we want to update m_totalSize too.
            for (inputIterator f = first; f != afterLast; ++f) {
                append_impl(*f);
            }
            
            return *this; // allow chaining.
        }
        string_builder & append_line(const std::string &src) {
            static char lineFeed[] = { 10, 0 }; // C++ 11. Feel the love!
            m_Data.push_back(src + lineFeed);
            m_totalSize += 1 + src.size();
            return *this; // allow chaining.
        }
        string_builder & append_line() {
            static char lineFeed[] = { 10, 0 };
            m_Data.push_back(lineFeed);
            ++m_totalSize;
            return *this; // allow chaining.
        }
        
        // TODO: AppendFormat implementation. Not relevant for the article.
        
        // Like C# StringBuilder.ToString()
        // Note the use of reserve() to avoid reallocations.
        std::string to_string() const {
            std::string result;
            // The whole point of the exercise!
            // If the container has a lot of strings, reallocation (each time the result grows) will take a serious toll,
            // both in performance and chances of failure.
            // I measured (in code I cannot publish) fractions of a second using 'reserve', and almost two minutes using +=.
            result.reserve(m_totalSize + 1);
            
            //  result = std::accumulate(m_Data.begin(), m_Data.end(), result); // This would lose the advantage of 'reserve'
            for (auto iter = m_Data.begin(); iter != m_Data.end(); ++iter) {
                result += *iter;
            }
            
            return result;
        }
        
        // like javascript Array.join()
        std::string join(const std::string &delim) const {
            if (delim.empty()) {
                return to_string();
            }
            
            std::string result;
            
            if (m_Data.empty()) {
                return result;
            }
            
            // Hope we don't overflow the size type.
            size_type st = (delim.size() * (m_Data.size() - 1)) + m_totalSize + 1;
            result.reserve(st);
            // If you need reasons to love C++11, here is one.
            struct adder {
                std::string m_Joiner;
                adder(const std::string &s): m_Joiner(s) {
                    // This constructor is NOT empty.
                }
                // This functor runs under accumulate() without reallocations, if 'l' has reserved enough memory.
                std::string operator()(std::string &l, const std::string &r) {
                    l += m_Joiner;
                    l += r;
                    return l;
                }
            } adr(delim);
            auto iter = m_Data.begin();
            // Skip the delimiter before the first element in the container.
            result += *iter;
            return std::accumulate(++iter, m_Data.end(), result, adr);
        }
        
    };
}

#endif // AMO_STRING_BUILDER_HPP__
