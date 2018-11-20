
#include <iostream>

#include <amo/json.hpp>
#include <gtest/gtest.h>
#include <amo/path.hpp>
#include <string.hpp>


TEST(json, constructor) {

    std::stringstream stream;
    stream << "3";
    amo::json json1;
    amo::json json2(false);
    amo::json json3("\"3\"");
    amo::json json4("{\"aa\":1}");
    amo::json json5({ {"aa", 1}, {"bb", "2"} });
    amo::json json6(json5);
    amo::json json7(stream);
    
    amo::json json8 = amo::json::from_file("test.json");
    amo::json json9 = json8;
    std::cout << json3.get_type() << std::endl;
    EXPECT_EQ(json1.is_object(), true);
    EXPECT_EQ(json2.is_array(), true);
    EXPECT_EQ(json3.is_string(), true);
    EXPECT_EQ(json4.is_object(), true);
    EXPECT_EQ(json5.is_object(), true);
    EXPECT_EQ(json6.is_object(), true);
    EXPECT_EQ(json7.is_number(), true);
    EXPECT_EQ(json8.is_valid(), false);
    EXPECT_EQ(json9.is_valid(), false);
    
    std::cout << json5.to_string() << std::endl;
}


TEST(json, join) {


    amo::json json1;
    json1.put("aa", 1);
    json1.put("bb", "2");
    amo::json json2;
    json2.put("aa", 3);
    
    json1.join(json2);
    EXPECT_EQ(json1.get<int>("aa"), 3);
    
}



TEST(json, put_child) {

    amo::json json1;
    json1.put("aa", 1);
    json1.put("bb", "2");
    amo::json json2;
    json2.put("aa", 3);
    json1.put_child("cc", json2);
    EXPECT_EQ(json1.get_child("cc").get<int>("aa"), 3);
    
}


TEST(json, contains_key) {

    amo::json json1;
    json1.put("aa", 1);
    json1.put("bb", "2");
    amo::json json2;
    json2.put("aa", 3);
    json1.put_child("cc", json2);
    EXPECT_EQ(json1.get_child("cc").contains_key("aa"), true);
    EXPECT_EQ(json1.contains_key("cc"), true);
    EXPECT_EQ(json1.contains_key("dd"), false);
}



TEST(json, isSameKey) {

    amo::json json1;
    json1.put("aa", 1);
    json1.put("bb", "2");
    amo::json json2;
    json2.put("aA", 3);
    
    json1.ignoreCase(false);
    json2.ignoreCase(true);
    
    EXPECT_EQ(json1.isSameKey("dd", "Dd"), false);
    EXPECT_EQ(json1.isSameKey("dd", "dd"), true);
    
    EXPECT_EQ(json2.isSameKey("dd", "Dd"), true);
    EXPECT_EQ(json2.isSameKey("dd", "dd"), true);
}

TEST(json, keys) {

    amo::json json1;
    json1.put("aa", 1);
    json1.put("bb", "2");
    
    
    EXPECT_EQ(json1.keys().size(), 2);
}

TEST(json, values) {

    amo::json json1;
    json1.put("aa", 1);
    json1.put("bb", "2");
    
    EXPECT_EQ(json1.values().size(), 2);
}


TEST(json, put_get) {
    {
        amo::uuid id;
        amo::date_time now;
        amo::json json1;
        json1.put("aa", 1);
        json1.put("bb", "2");
        json1.put("cc", true);
        json1.put("dd", 1.1);
        json1.put("ee", id);
        json1.put("ff", now);
        json1.put("gg", amo::json());
        std::cout << json1.to_string() << std::endl;
        
        EXPECT_EQ(json1.get<amo::date_time>("ff").to_string(), now.to_string());
        EXPECT_EQ(json1.get<amo::uuid>("ee").to_string(), id.to_string());
    }
    
    {
        amo::uuid id;
        amo::date_time now;
        amo::json json1;
        json1.put(1);
        json1.put("2");
        json1.put(true);
        json1.put(1.1);
        
        json1.put(now);
        
        
        
        EXPECT_EQ(json1.get<amo::date_time>().to_string(), now.to_string());
        json1.put(id);
        EXPECT_EQ(json1.get<amo::uuid>().to_string(), id.to_string());
        json1.put(amo::json());
    }
    
    {
        amo::uuid id;
        amo::date_time now;
        amo::u8json json1;
        json1.put("aa", 1);
        json1.put("bb", "2");
        json1.put("cc", true);
        json1.put("dd", 1.1);
        json1.put("ee", id);
        json1.put("ff", now);
        json1.put("gg", amo::json());
        json1.put(u8"中文KEY", u8"中文值");
        std::cout << amo::u8string(json1.to_string(), true).to_locale() << std::endl;
        
        EXPECT_EQ(json1.get<amo::date_time>("ff").to_string(), now.to_string());
        EXPECT_EQ(json1.get<amo::uuid>("ee").to_string(), id.to_string());
        EXPECT_EQ(json1.get<std::string>(u8"中文KEY"), u8"中文值");
    }
    
    {
        amo::uuid id;
        amo::date_time now;
        amo::u8json json1;
        json1.put(1);
        json1.put("2");
        json1.put(true);
        json1.put(1.1);
        
        json1.put(now);
        
        
        
        EXPECT_EQ(json1.get<amo::date_time>().to_string(), now.to_string());
        json1.put(id);
        EXPECT_EQ(json1.get<amo::uuid>().to_string(), id.to_string());
        json1.put(u8"中文值");
        EXPECT_EQ(json1.get<std::string>(), u8"中文值");
        
    }
}

TEST(json, to_array) {
    amo::json json1;
    json1.set_array();
    json1.push_back(1);
    json1.push_back("1");
    json1.push_back(amo::json());
    
    std::vector<amo::json> vec = json1.to_array();
    
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0].get<int>(), 1);
    EXPECT_EQ(vec[1].get<std::string>(), "1");
    EXPECT_EQ(vec[2].get<amo::json>().is_object(), true);
}

TEST(json, to_string) {
    amo::json json1;
    json1.set_array();
    json1.push_back(1);
    json1.push_back("1");
    json1.push_back(amo::json());
    
    std::cout << json1.to_string() << std::endl;
}

TEST(json, to_file) {
    amo::json json1;
    json1.set_array();
    json1.push_back(1);
    json1.push_back("1");
    json1.push_back(amo::json());
    json1.to_file("./test2.json");
    EXPECT_EQ(amo::path("./test2.json").exists(), true);
    
}

TEST(json, format) {
    amo::json json1;
    json1.set_array();
    json1.push_back(1);
    json1.push_back("1");
    amo::json json2;
    json2.put("aa", 3);
    json2.put("bb", true);
    json2.put("cc", "dddd");
    json1.push_back(json2);
    std::cout << json1.format() << std::endl;
    
}