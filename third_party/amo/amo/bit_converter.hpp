// Created by amoylel on 11/18/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_BIT_CONVERTER_HPP__
#define AMO_BIT_CONVERTER_HPP__

#include <vector>
#include <string>
#include <utility>
#include <algorithm>

namespace amo {

    /**
     * @class	BitConverter
     *
     * @brief	摘要:
     * 			    将基础数据类型与字节数组相互转换。.
     */
    
    class bit_converter {
    
    public:
        /**
         * @fn	static bool BitConverter::IsLittleEndian()
         *
         * @brief	 指示数据在此计算机结构中存储时的字节顺序（“Endian”性质）。.
         *
         * @return	true if little endian, false if not.
         */
        
        static  bool IsLittleEndian() {
            int i = 0x11223344;
            char *p;
            
            p = (char *)&i;
            
            if (*p == 0x44) {
                //printf("Little endian\n");
                return true;
            } else {
                //printf("Big endian\n");
                return false;
            }
            
            return  false;
        }
        
        template<typename T>
        static T bytesToNumber(std::vector<int8_t> b, size_t len = 0) {
            T mask = (T)0xff;
            T val = 0;
            
            if (len == 0) {
                len = b.size();
            }
            
            if (len > b.size()) {
                len = b.size();
            }
            
            for (size_t i = 0; i < len; ++i) {
                val += ((b[i] & mask) << (i * 8));
            }
            
            return val;
        }
        
        template<typename T>
        static T toNumber(std::vector<int8_t>& value, size_t startIndex = 0,
                          size_t len = 0) {
            if (len == 0) {
                len = sizeof(T);
            }
            
            if (startIndex + len > value.size()) {
                return T();
            }
            
            std::vector<int8_t> vec;
            std::copy(value.begin() + startIndex, value.begin() + startIndex + len,
                      std::back_inserter(vec));
                      
            if (IsLittleEndian()) {
                std::reverse(vec.begin(), vec.end());
            }
            
            return bytesToNumber<T>(vec, len);
        }
        
        template<typename T>
        static std::vector<int8_t> numberToBytes(T val) {
            T mask = (T)0xff;
            
            int len = sizeof(T);
            std::vector<int8_t> vec(len, 0);
            
            for (int i = 0; i < len; i++) {
                vec[i] = (int8_t)(val >> (len * 8 - i * 8));
            }
            
            return vec;
        }
        
        
        //
        // 摘要:
        //     将指定的双精度浮点数转换为 64 位有符号整数。
        //
        // 参数:
        //   value:
        //     要转换的数字。
        //
        // 返回结果:
        //     64 位有符号整数，其值等于 value。
        
        static long DoubleToInt64Bits(double value) {
            return ((int64_t)value);
        }
        //
        // 摘要:
        //     以字节数组的形式返回指定的布尔值。
        //
        // 参数:
        //   value:
        //     一个布尔值。
        //
        // 返回结果:
        //     长度为 1 的字节数组。
        static std::vector<int8_t> GetBytes(bool value) {
            return numberToBytes((int8_t)value);
        }
        //
        // 摘要:
        //     以字节数组的形式返回指定的 Unicode 字符值。
        //
        // 参数:
        //   value:
        //     要转换的字符。
        //
        // 返回结果:
        //     长度为 2 的字节数组。
        static std::vector<int8_t> GetBytes(char value) {
            return numberToBytes(value);
        }
        //
        // 摘要:
        //     以字节数组的形式返回指定的 16 位有符号整数值。
        //
        // 参数:
        //   value:
        //     要转换的数字。
        //
        // 返回结果:
        //     长度为 2 的字节数组。
        
        static std::vector<int8_t> GetBytes(short value) {
            return numberToBytes(value);
        }
        //
        // 摘要:
        //     以字节数组的形式返回指定的 32 位有符号整数值。
        //
        // 参数:
        //   value:
        //     要转换的数字。
        //
        // 返回结果:
        //     长度为 4 的字节数组。
        
        static std::vector<int8_t> GetBytes(int value) {
            return numberToBytes(value);
        }
        //
        // 摘要:
        //     以字节数组的形式返回指定的 64 位有符号整数值。
        //
        // 参数:
        //   value:
        //     要转换的数字。
        //
        // 返回结果:
        //     长度为 8 的字节数组。
        
        static std::vector<int8_t> GetBytes(long value) {
            return numberToBytes(value);
        }
        //
        // 摘要:
        //     以字节数组的形式返回指定的 16 位无符号整数值。
        //
        // 参数:
        //   value:
        //     要转换的数字。
        //
        // 返回结果:
        //     长度为 2 的字节数组。
        
        static std::vector<int8_t> GetBytes(uint16_t value) {
            return numberToBytes(value);
        }
        //
        // 摘要:
        //     以字节数组的形式返回指定的 32 位无符号整数值。
        //
        // 参数:
        //   value:
        //     要转换的数字。
        //
        // 返回结果:
        //     长度为 4 的字节数组。
        
        static std::vector<int8_t> GetBytes(uint32_t value) {
            return numberToBytes(value);
        }
        //
        // 摘要:
        //     以字节数组的形式返回指定的 64 位无符号整数值。
        //
        // 参数:
        //   value:
        //     要转换的数字。
        //
        // 返回结果:
        //     长度为 8 的字节数组。
        
        static std::vector<int8_t> GetBytes(uint64_t value) {
            return numberToBytes(value);
        }
        //
        // 摘要:
        //     以字节数组的形式返回指定的单精度浮点值。
        //
        // 参数:
        //   value:
        //     要转换的数字。
        //
        // 返回结果:
        //     长度为 4 的字节数组。
        
        static std::vector<int8_t> GetBytes(float value) {
            return numberToBytes((int32_t)value);
        }
        //
        // 摘要:
        //     以字节数组的形式返回指定的双精度浮点值。
        //
        // 参数:
        //   value:
        //     要转换的数字。
        //
        // 返回结果:
        //     长度为 8 的字节数组。
        
        static std::vector<int8_t> GetBytes(double value) {
            return numberToBytes((int64_t)value);
        }
        //
        // 摘要:
        //     将指定的 64 位有符号整数转换成双精度浮点数。
        //
        // 参数:
        //   value:
        //     要转换的数字。
        //
        // 返回结果:
        //     双精度浮点数，其值等于 value。
        
        static double Int64BitsToDouble(long value) {
            return value;
        }
        //
        // 摘要:
        //     返回由字节数组中指定位置的一个字节转换来的布尔值。
        //
        // 参数:
        //   value:
        //     字节数组。
        //
        //   startIndex:
        //     value 内的起始位置。
        //
        // 返回结果:
        //     如果 value 中的 startIndex 处的字节非零，则为 true；否则为 false。
        //
        // 异常:
        //   T:System.ArgumentNullException:
        //     value 为 null。
        //
        //   T:System.ArgumentOutOfRangeException:
        //     startIndex 小于零或大于 value 减 1 的长度。
        static bool ToBoolean(std::vector<int8_t> value, int startIndex = 0) {
            return (toNumber<int8_t>(value, startIndex, sizeof(int8_t)) != 0);
        }
        //
        // 摘要:
        //     返回由字节数组中指定位置的两个字节转换来的 Unicode 字符。
        //
        // 参数:
        //   value:
        //     一个数组。
        //
        //   startIndex:
        //     value 内的起始位置。
        //
        // 返回结果:
        //     由两个字节构成、从 startIndex 开始的字符。
        //
        // 异常:
        //   T:System.ArgumentException:
        //     startIndex 等于 value 减 1 的长度。
        //
        //   T:System.ArgumentNullException:
        //     value 为 null。
        //
        //   T:System.ArgumentOutOfRangeException:
        //     startIndex 小于零或大于 value 减 1 的长度。
        static char ToChar(std::vector<int8_t> value, int startIndex = 0) {
            return toNumber<char>(value, startIndex, sizeof(char));
        }
        //
        // 摘要:
        //     返回由字节数组中指定位置的八个字节转换来的双精度浮点数。
        //
        // 参数:
        //   value:
        //     字节数组。
        //
        //   startIndex:
        //     value 内的起始位置。
        //
        // 返回结果:
        //     由八个字节构成、从 startIndex 开始的双精度浮点数。
        //
        // 异常:
        //   T:System.ArgumentException:
        //     startIndex 大于等于 value 减 7 的长度，且小于等于 value 减 1 的长度。
        //
        //   T:System.ArgumentNullException:
        //     value 为 null。
        //
        //   T:System.ArgumentOutOfRangeException:
        //     startIndex 小于零或大于 value 减 1 的长度。
        
        static double ToDouble(std::vector<int8_t> value, int startIndex = 0) {
            return (double)toNumber<int64_t>(value, startIndex, sizeof(double));
        }
        //
        // 摘要:
        //     返回由字节数组中指定位置的两个字节转换来的 16 位有符号整数。
        //
        // 参数:
        //   value:
        //     字节数组。
        //
        //   startIndex:
        //     value 内的起始位置。
        //
        // 返回结果:
        //     由两个字节构成、从 startIndex 开始的 16 位有符号整数。
        //
        // 异常:
        //   T:System.ArgumentException:
        //     startIndex 等于 value 减 1 的长度。
        //
        //   T:System.ArgumentNullException:
        //     value 为 null。
        //
        //   T:System.ArgumentOutOfRangeException:
        //     startIndex 小于零或大于 value 减 1 的长度。
        
        static int16_t ToInt16(std::vector<int8_t> value, int startIndex = 0) {
            return toNumber<int16_t>(value, startIndex, sizeof(int16_t));
        }
        //
        // 摘要:
        //     返回由字节数组中指定位置的四个字节转换来的 32 位有符号整数。
        //
        // 参数:
        //   value:
        //     字节数组。
        //
        //   startIndex:
        //     value 内的起始位置。
        //
        // 返回结果:
        //     由四个字节构成、从 startIndex 开始的 32 位有符号整数。
        //
        // 异常:
        //   T:System.ArgumentException:
        //     startIndex 大于等于 value 减 3 的长度，且小于等于 value 减 1 的长度。
        //
        //   T:System.ArgumentNullException:
        //     value 为 null。
        //
        //   T:System.ArgumentOutOfRangeException:
        //     startIndex 小于零或大于 value 减 1 的长度。
        
        static int32_t ToInt32(std::vector<int8_t> value, int startIndex = 0) {
            return toNumber<int32_t>(value, startIndex, sizeof(int32_t));
        }
        //
        // 摘要:
        //     返回由字节数组中指定位置的八个字节转换来的 64 位有符号整数。
        //
        // 参数:
        //   value:
        //     字节数组。
        //
        //   startIndex:
        //     value 内的起始位置。
        //
        // 返回结果:
        //     由八个字节构成、从 startIndex 开始的 64 位有符号整数。
        //
        // 异常:
        //   T:System.ArgumentException:
        //     startIndex 大于等于 value 减 7 的长度，且小于等于 value 减 1 的长度。
        //
        //   T:System.ArgumentNullException:
        //     value 为 null。
        //
        //   T:System.ArgumentOutOfRangeException:
        //     startIndex 小于零或大于 value 减 1 的长度。
        
        static int64_t ToInt64(std::vector<int8_t> value, int startIndex = 0) {
            return toNumber<int64_t>(value, startIndex, sizeof(int64_t));
        }
        //
        // 摘要:
        //     返回由字节数组中指定位置的四个字节转换来的单精度浮点数。
        //
        // 参数:
        //   value:
        //     字节数组。
        //
        //   startIndex:
        //     value 内的起始位置。
        //
        // 返回结果:
        //     由四个字节构成、从 startIndex 开始的单精度浮点数。
        //
        // 异常:
        //   T:System.ArgumentException:
        //     startIndex 大于等于 value 减 3 的长度，且小于等于 value 减 1 的长度。
        //
        //   T:System.ArgumentNullException:
        //     value 为 null。
        //
        //   T:System.ArgumentOutOfRangeException:
        //     startIndex 小于零或大于 value 减 1 的长度。
        
        static float ToSingle(std::vector<int8_t> value, int startIndex = 0) {
            return (float)toNumber<int32_t>(value, startIndex);
        }
        
        static char intToChar(const int& val) {
            if (val > 10) {
                return 'A' + 10 - val;
            } else {
                return '0' + val;
            }
        }
        
        //
        // 摘要:
        //     将指定的字节子数组的每个元素的数值转换为它的等效十六进制字符串表示形式。
        //
        // 参数:
        //   value:
        //     字节数组。
        //
        //   startIndex:
        //     value 内的起始位置。
        //
        //   length:
        //     要转换的 value 中的数组元素数。
        //
        // 返回结果:
        //     由以连字符分隔的十六进制对构成的 System.String，其中每一对表示 value 的子数组中对应的元素；例如“7F-2C-4A”。
        //
        // 异常:
        //   T:System.ArgumentNullException:
        //     value 为 null。
        //
        //   T:System.ArgumentOutOfRangeException:
        //     startIndex 或 length 小于零。- 或 -startIndex 大于零且大于等于 value 的长度。
        //
        //   T:System.ArgumentException:
        //     startIndex 和 length 的组合不指定 value 中的位置；也就是说，startIndex 参数大于 value 的长度减去 length
        //     参数。
        static std::string ToString(std::vector<int8_t> value,
                                    const std::string& placeholders = "", size_t startIndex = 0,
                                    size_t length = 0) {
            std::string str;
            
            if (length <= 0 || length > value.size()) {
                length = value.size();
            }
            
            for (size_t i = startIndex; i < length; ++i) {
                auto& p = value[i];
                int c1 = p & 0xf0;
                int c2 = p & 0xf;
                str += intToChar(c1);
                str += intToChar(c2);
                
                if (i < length - 1) {
                    str += placeholders;
                }
            }
            
            return str;
        }
        //
        // 摘要:
        //     返回由字节数组中指定位置的两个字节转换来的 16 位无符号整数。
        //
        // 参数:
        //   value:
        //     字节数组。
        //
        //   startIndex:
        //     value 内的起始位置。
        //
        // 返回结果:
        //     由两个字节构成、从 startIndex 开始的 16 位无符号整数。
        //
        // 异常:
        //   T:System.ArgumentException:
        //     startIndex 等于 value 减 1 的长度。
        //
        //   T:System.ArgumentNullException:
        //     value 为 null。
        //
        //   T:System.ArgumentOutOfRangeException:
        //     startIndex 小于零或大于 value 减 1 的长度。
        
        static uint16_t ToUInt16(std::vector<int8_t> value, int startIndex = 0) {
            return toNumber<uint16_t>(value, startIndex, sizeof(uint16_t));
        }
        
        /**
         * @fn	static uint BitConverter::ToUInt32(std::vector<int8_t> value, int startIndex);
         *
         * @brief	摘要:
         * 			    返回由字节数组中指定位置的四个字节转换来的 32 位无符号整数。
         *
         * 			参数:
         * 			  value:
         * 			    字节数组。
         *
         * 			  startIndex:
         * 			    value 内的起始位置。
         *
         * 			返回结果:
         * 			    由四个字节构成、从 startIndex 开始的 32 位无符号整数。
         *
         * 			异常:
         * 			  T:System.ArgumentException:
         * 			    startIndex 大于等于 value 减 3 的长度，且小于等于 value 减 1 的长度。
         *
         * 			  T:System.ArgumentNullException:
         * 			    value 为 null。
         *
         * 			  T:System.ArgumentOutOfRangeException:
         * 			    startIndex 小于零或大于 value 减 1 的长度。.
         *
         * @param	value	  	The value.
         * @param	startIndex	The start index.
         *
         * @return	The given data converted to an uint.
         */
        
        
        static uint32_t ToUInt32(std::vector<int8_t> value, int startIndex = 0) {
            return toNumber<uint32_t>(value, startIndex, sizeof(uint32_t));
        }
        //
        // 摘要:
        //     返回由字节数组中指定位置的八个字节转换来的 64 位无符号整数。
        //
        // 参数:
        //   value:
        //     字节数组。
        //
        //   startIndex:
        //     value 内的起始位置。
        //
        // 返回结果:
        //     由八个字节构成、从 startIndex 开始的 64 位无符号整数。
        //
        // 异常:
        //   T:System.ArgumentException:
        //     startIndex 大于等于 value 减 7 的长度，且小于等于 value 减 1 的长度。
        //
        //   T:System.ArgumentNullException:
        //     value 为 null。
        //
        //   T:System.ArgumentOutOfRangeException:
        //     startIndex 小于零或大于 value 减 1 的长度。
        
        static uint64_t ToUInt64(std::vector<int8_t> value, int startIndex = 0) {
            return toNumber<uint64_t>(value, startIndex, sizeof(uint64_t));
        }
    };
}

#endif // AMO_BIT_CONVERTER_HPP__