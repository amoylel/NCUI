// Created by 苏元海 on 02/14/2016.
// Copyright (c) 2016 amoylel. All rights reserved.

#ifndef IME_STRINGIFY_HPP__
#define IME_STRINGIFY_HPP__
 
#include <map>
#include <set>
#include <vector>
#include <ostream>
#include <string>

#include <amo/lexical_cast.hpp>
#include <amo/foreach.hpp>
 
namespace amo 
{ 

template <class T>
std::string stringify(const T& val, size_t truncate = 0);

template <class T>
void streamstringify(std::ostream& os, const T& val, size_t = 0);

template <class T1, class T2>
void streamstringify(std::ostream& os, const std::pair<T1, T2>& val, size_t truncate = 0);

template <class T>
void streamstringify(std::ostream& os, const std::set<T>& val, size_t truncate = 0);

template <class T>
void streamstringify(std::ostream& os, const std::vector<T>& val, size_t truncate = 0);

template <class T1, class T2>
void streamstringify(std::ostream& os, const std::map<T1, T2>& val, size_t truncate = 0);

/**
 * Turn complex data structures into plain strings.
 *
 * Usage is pretty simple: just call stringify with an arbitrarily nested data
 * structure argument and it will turn its contents into a string representation.
 * It's a bit like Perl's Data::Dumper for C++.
 *
 * You can add support for your own types by overloading the template.
 */
template <class T>
std::string stringify(const T& val, size_t truncate)
{
   std::ostringstream oss;
   streamstringify(oss, val, truncate);
   return oss.str();
}

template <class T>
void streamstringify(std::ostream& os, const T& val, size_t)
{
   os << val;
}

template <class T1, class T2>
void streamstringify(std::ostream& os, const std::pair<T1, T2>& val, size_t truncate)
{
   os << "(";
   streamstringify(os, val.first, truncate);
   os << ", ";
   streamstringify(os, val.second, truncate);
   os << ")";
}

template <class T>
void streamstringify(std::ostream& os, const std::set<T>& val, size_t truncate)
{
   size_t count = 0;
   os << "(";
   foreach (const T& v, val)
   {
      if (count)
         os << ", ";
      streamstringify(os, v, truncate);
      if (++count == truncate && count < val.size())
      {
         os << ", <+" << val.size() - count << ">";
         break;
      }
   }
   os << ")";
}

template <class T>
void streamstringify(std::ostream& os, const std::vector<T>& val, size_t truncate)
{
   size_t count = 0;
   os << "[";
   foreach (const T& v, val)
   {
      if (count)
         os << ", ";
      streamstringify(os, v, truncate);
      if (++count == truncate && count < val.size())
      {
         os << ", <+" << val.size() - count << ">";
         break;
      }
   }
   os << "]";
}

template <class T1, class T2>
void streamstringify(std::ostream& os, const std::map<T1, T2>& val, size_t truncate)
{
   typedef typename std::map<T1, T2>::value_type type_pair;
   size_t count = 0;
   os << "{";
   foreach (const type_pair& p, val)
   {
      if (count)
         os << ", ";
      streamstringify(os, p, truncate);
      if (++count == truncate && count < val.size())
      {
         os << ", <+" << val.size() - count << ">";
         break;
      }
   }
   os << "}";
}

}

#endif // IME_STRINGIFY_HPP__
