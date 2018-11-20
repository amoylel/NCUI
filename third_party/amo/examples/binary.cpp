#include <iostream>

#include <gtest/gtest.h>
#include <amo/binary.hpp>
#include <amo/string/string_utils.hpp>
#include <numeric>

TEST(binary, constructor) {
    {
        amo::binary b1((int8_t)125);
        amo::binary b2((int16_t)125);
        amo::binary b3((int32_t)56845);
        amo::binary b4((int64_t)127);
        
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("0111 1101", " ", ""));
        EXPECT_EQ(b2.to_string(), amo::string_utils::replace_c("0000 0000 0111 1101", " ", ""));
        EXPECT_EQ(b3.to_string(), amo::string_utils::replace_c("0000 0000 0000 0000 1101 1110 0000 1101", " ", ""));
        EXPECT_EQ(b4.to_string(), amo::string_utils::replace_c("0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0111 1111", " ", ""));
        
        EXPECT_EQ(b1.to_string(true), amo::string_utils::replace_c("111 1101", " ", ""));
        EXPECT_EQ(b2.to_string(true), amo::string_utils::replace_c("111 1101", " ", ""));
        EXPECT_EQ(b3.to_string(true), amo::string_utils::replace_c("1101 1110 0000 1101", " ", ""));
        EXPECT_EQ(b4.to_string(true), amo::string_utils::replace_c("111 1111", " ", ""));
    }
    
    {
        amo::binary b1((int8_t) - 125);
        amo::binary b2((int16_t) - 125);
        amo::binary b3((int32_t) - 56845);
        amo::binary b4((int64_t) - 127);
        
        
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("1000 0011", " ", ""));
        EXPECT_EQ(b2.to_string(), amo::string_utils::replace_c("1111 1111 1000 0011", " ", ""));
        EXPECT_EQ(b3.to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 0010 0001 1111 0011", " ", ""));
        EXPECT_EQ(b4.to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0001", " ", ""));
    }
    
    {
        amo::binary b1("1000 0011");
        amo::binary b2("1111 1111 1000 0011");
        amo::binary b3("1111 1111 1111 1111 0010 0001 1111 0011");
        amo::binary b4("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0001");
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("1000 0011", " ", ""));
        EXPECT_EQ(b2.to_string(), amo::string_utils::replace_c("1111 1111 1000 0011", " ", ""));
        EXPECT_EQ(b3.to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 0010 0001 1111 0011", " ", ""));
        EXPECT_EQ(b4.to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0001", " ", ""));
        
    }
}

TEST(binary, sub) {
    {
        amo::binary b1((int8_t)125);
        amo::binary b2((int16_t)125);
        amo::binary b3((int32_t)56845);
        amo::binary b4((int64_t)127);
        
        EXPECT_EQ(b1.sub(3, 4).to_string(), amo::string_utils::replace_c("1101", " ", ""));
        EXPECT_EQ(b2.sub(7, 4).to_string(), amo::string_utils::replace_c("0111", " ", ""));
        EXPECT_EQ(b2.sub(6, 4).to_string(), amo::string_utils::replace_c("1111", " ", ""));
        
        EXPECT_EQ(b3.sub(15, 13).to_string(), amo::string_utils::replace_c("1101 1110 0000 1", " ", ""));
        EXPECT_EQ(b4.sub(63, 60).to_string(), amo::string_utils::replace_c("0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0111", " ", ""));
        
        EXPECT_EQ(b1.sub(8, 2).to_string(true), "");
        EXPECT_EQ(b1.sub(12, 0).to_string(true),  "");
        EXPECT_EQ(b1.sub(12, 12).to_string(true),  "");
        EXPECT_EQ(b1.sub(7, 12).to_string(true), amo::string_utils::replace_c("111 1101", " ", ""));
    }
    
    {
        amo::binary b1((int8_t) - 125);
        amo::binary b2((int16_t) - 125);
        amo::binary b3((int32_t) - 56845);
        amo::binary b4((int64_t) - 127);
        
        
        EXPECT_EQ(b1.sub(3, 4).to_string(), amo::string_utils::replace_c("0011", " ", ""));
        EXPECT_EQ(b2.sub(11, 5).to_string(), amo::string_utils::replace_c("1111 1", " ", ""));
        EXPECT_EQ(b3.sub(31, 30).to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 0010 0001 1111 00", " ", ""));
        EXPECT_EQ(b4.sub(63, 48).to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 ", " ", ""));
    }
    
}

TEST(binary, operatorR) {
    {
        amo::binary b1((int8_t)125);
        amo::binary b2((int16_t)125);
        amo::binary b3((int32_t)56845);
        amo::binary b4((int64_t)127);
        
        EXPECT_EQ((int8_t)b1, 125);
        EXPECT_EQ((int8_t)b2, 125);
        EXPECT_EQ((int32_t)b2, 125);
        EXPECT_EQ((int32_t)b3, 56845);
        EXPECT_EQ((int16_t)b3, -8691);
        EXPECT_EQ((int16_t)b3, (int16_t)56845);
        
        
        EXPECT_EQ((int64_t)b4, 127);
        EXPECT_EQ((int32_t)b4, 127);
        EXPECT_EQ((int16_t)b4, 127);
        EXPECT_EQ((int8_t)b4, 127);
        
        
    }
    
    {
        amo::binary b1((int8_t) - 125);
        amo::binary b2((int16_t) - 125);
        amo::binary b3((int32_t) - 56845);
        amo::binary b4((int64_t) - 127);
        
        EXPECT_EQ((int8_t)b1, -125);
        EXPECT_EQ((int8_t)b2, -125);
        EXPECT_EQ((int32_t)b2, -125);
        EXPECT_EQ((int32_t)b3, -56845);
        EXPECT_EQ((int16_t)b3, 8691);
        int16_t cc = -56845;
        EXPECT_EQ((int16_t)b3, (int16_t) - 56845);
        
        
        EXPECT_EQ((int64_t)b4, -127);
        EXPECT_EQ((int32_t)b4, -127);
        EXPECT_EQ((int16_t)b4, -127);
        EXPECT_EQ((int8_t)b4, -127);
        
    }
    
    {
        amo::binary b1("1000 0011");
        amo::binary b2("1111 1111 1000 0011");
        amo::binary b3("1111 1111 1111 1111 0010 0001 1111 0011");
        amo::binary b4("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0001");
        
        EXPECT_EQ((int8_t)b1, -125);
        EXPECT_EQ((int8_t)b2, -125);
        EXPECT_EQ((int32_t)b2, -125);
        EXPECT_EQ((int32_t)b3, -56845);
        EXPECT_EQ((int16_t)b3, 8691);
        int16_t cc = -56845;
        EXPECT_EQ((int16_t)b3, (int16_t) - 56845);
        
        
        EXPECT_EQ((int64_t)b4, -127);
        EXPECT_EQ((int32_t)b4, -127);
        EXPECT_EQ((int16_t)b4, -127);
        EXPECT_EQ((int8_t)b4, -127);
        
    }
    
    {
        amo::binary b1("000 0011");
        amo::binary b2("0011");
        amo::binary b3("11");
        amo::binary b4("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0001");
        
        EXPECT_EQ((int8_t)b1, 3);
        EXPECT_EQ((int8_t)b2, 3);
        EXPECT_EQ((int32_t)b2, 3);
        EXPECT_EQ((int32_t)b3, 3);
        EXPECT_EQ((int16_t)b3, 3);
        
        EXPECT_EQ((int16_t)b3, (int16_t) 3);
        
        
        EXPECT_EQ((int64_t)b4, -127);
        EXPECT_EQ((int32_t)b4, -127);
        EXPECT_EQ((int16_t)b4, -127);
        EXPECT_EQ((int8_t)b4, -127);
        
    }
}



TEST(binary, ceil) {

    {
        amo::binary b1("000 0011");
        amo::binary b2("1011");
        amo::binary b3("111");
        amo::binary b4("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0001");
        
        b1.ceil();
        b2.ceil();
        b3.ceil();
        b4.ceil();
        
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("0000 0011", " ", ""));
        EXPECT_EQ(b2.to_string(), amo::string_utils::replace_c("0000 1011", " ", ""));
        EXPECT_EQ(b3.to_string(), amo::string_utils::replace_c("0000 0111", " ", ""));
        EXPECT_EQ(b4.to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0001", " ", ""));
        
        
        b1.ceil();
        b2.ceil();
        b3.ceil();
        b4.ceil();
        
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("0000 0000 0000 0011", " ", ""));
        EXPECT_EQ(b2.to_string(), amo::string_utils::replace_c("0000 0000 0000 1011", " ", ""));
        EXPECT_EQ(b3.to_string(), amo::string_utils::replace_c("0000 0000 0000 0111", " ", ""));
        EXPECT_EQ(b4.to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0001", " ", ""));
        
        b1.ceil();
        b2.ceil();
        b3.ceil();
        b4.ceil();
        
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("0000 0000 0000 0000 0000 0000 0000 0011", " ", ""));
        EXPECT_EQ(b2.to_string(), amo::string_utils::replace_c("0000 0000 0000 0000 0000 0000 0000 1011", " ", ""));
        EXPECT_EQ(b3.to_string(), amo::string_utils::replace_c("0000 0000 0000 0000 0000 0000 0000 0111", " ", ""));
        EXPECT_EQ(b4.to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0001", " ", ""));
        
        b1.ceil();
        b2.ceil();
        b3.ceil();
        b4.ceil();
        
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0011", " ", ""));
        EXPECT_EQ(b2.to_string(), amo::string_utils::replace_c("0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 1011", " ", ""));
        EXPECT_EQ(b3.to_string(), amo::string_utils::replace_c("0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0111", " ", ""));
        EXPECT_EQ(b4.to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0001", " ", ""));
        
    }
    
    
    {
        amo::binary b1("1000 0011");
        b1.ceil();
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("1111 1111 1000 0011", " ", ""));
        
        b1.ceil();
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 1111 1111 1000 0011", " ", ""));
        
        b1.ceil();
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0011", " ", ""));
        
        b1.ceil();
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0011", " ", ""));
        
    }
    
    {
        amo::binary b1("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0011 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0011");
        b1.ceil();
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0011 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0011", " ", ""));
        
        b1.ceil();
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0011 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0011", " ", ""));
        
        b1.ceil();
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0011 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0011", " ", ""));
        
    }
}


TEST(binary, floor) {

    {
        amo::binary b1("000 0011");
        amo::binary b2("1011");
        amo::binary b3("111");
        amo::binary b4("101 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0001");
        
        b1.floor();
        b2.floor();
        b3.floor();
        b4.floor();
        
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("000 0011", " ", ""));
        EXPECT_EQ(b2.to_string(), amo::string_utils::replace_c("1011", " ", ""));
        EXPECT_EQ(b3.to_string(), amo::string_utils::replace_c("111", " ", ""));
        EXPECT_EQ(b4.to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0001", " ", ""));
        
        
        b1.floor();
        b2.floor();
        b3.floor();
        b4.floor();
        
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("000 0011", " ", ""));
        EXPECT_EQ(b2.to_string(), amo::string_utils::replace_c("1011", " ", ""));
        EXPECT_EQ(b3.to_string(), amo::string_utils::replace_c("111", " ", ""));
        EXPECT_EQ(b4.to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 1111 1111 1000 0001", " ", ""));
        
        b1.floor();
        b2.floor();
        b3.floor();
        b4.floor();
        
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("000 0011", " ", ""));
        EXPECT_EQ(b2.to_string(), amo::string_utils::replace_c("1011", " ", ""));
        EXPECT_EQ(b3.to_string(), amo::string_utils::replace_c("111", " ", ""));
        EXPECT_EQ(b4.to_string(), amo::string_utils::replace_c("1111 1111 1000 0001", " ", ""));
        
        b1.floor();
        b2.floor();
        b3.floor();
        b4.floor();
        
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("000 0011", " ", ""));
        EXPECT_EQ(b2.to_string(), amo::string_utils::replace_c("1011", " ", ""));
        EXPECT_EQ(b3.to_string(), amo::string_utils::replace_c("111", " ", ""));
        EXPECT_EQ(b4.to_string(), amo::string_utils::replace_c("1000 0001", " ", ""));
    }
    
    
    {
        amo::binary b1("11 1000 0011");
        amo::binary b2("10 1111 1011");
        amo::binary b3("10 1110 0000 1110 1110");
        amo::binary b4("101 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0001");
        
        b1.floor();
        b2.floor();
        b3.floor();
        b4.floor();
        
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("1000 0011", " ", ""));
        EXPECT_EQ(b2.to_string(), amo::string_utils::replace_c("1111 1011", " ", ""));
        EXPECT_EQ(b3.to_string(), amo::string_utils::replace_c("1110 0000 1110 1110", " ", ""));
        EXPECT_EQ(b4.to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0001", " ", ""));
        
        
        b1.floor();
        b2.floor();
        b3.floor();
        b4.floor();
        
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("1000 0011", " ", ""));
        EXPECT_EQ(b2.to_string(), amo::string_utils::replace_c("1111 1011", " ", ""));
        EXPECT_EQ(b3.to_string(), amo::string_utils::replace_c("1110 1110", " ", ""));
        EXPECT_EQ(b4.to_string(), amo::string_utils::replace_c("1111 1111 1111 1111 1111 1111 1000 0001", " ", ""));
        
        b1.floor();
        b2.floor();
        b3.floor();
        b4.floor();
        
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("1000 0011", " ", ""));
        EXPECT_EQ(b2.to_string(), amo::string_utils::replace_c("1111 1011", " ", ""));
        EXPECT_EQ(b3.to_string(), amo::string_utils::replace_c("1110 1110", " ", ""));
        EXPECT_EQ(b4.to_string(), amo::string_utils::replace_c("1111 1111 1000 0001", " ", ""));
        
        
        
        b1.floor();
        b2.floor();
        b3.floor();
        b4.floor();
        
        EXPECT_EQ(b1.to_string(), amo::string_utils::replace_c("1000 0011", " ", ""));
        EXPECT_EQ(b2.to_string(), amo::string_utils::replace_c("1111 1011", " ", ""));
        EXPECT_EQ(b3.to_string(), amo::string_utils::replace_c("1110 1110", " ", ""));
        EXPECT_EQ(b4.to_string(), amo::string_utils::replace_c("1000 0001", " ", ""));
    }
}