#include <iostream>

#include <gtest/gtest.h>

#include <amo/date_time.hpp>
#include <spdlog/details/os.h>

TEST(date_time, constructor) {
    amo::date_time t1;
    amo::date_time t2(2018, 10, 1, 0, 0, 0, 0);
    amo::date_time t3("2018-10-01 00:00:00");
    amo::date_time t4((int64_t)1538323200000LL);
    amo::date_time t5 = amo::date_time::from_utc(t4.to_utc());
    
    EXPECT_EQ(t1.timestamp(), amo::date_time::now());
    EXPECT_EQ(t2.timestamp(), t3.timestamp());
    EXPECT_EQ(t2.timestamp(), t4.timestamp());
    EXPECT_EQ(t2.timestamp(), t5.timestamp());
}
TEST(date_time, from_utc) {
    amo::date_time t1;
    amo::date_time t2 = amo::date_time::from_utc(t1.to_utc());
    EXPECT_EQ(t1.timestamp(), t2.timestamp());
}

TEST(date_time, to_utc) {
    amo::date_time t1;
    amo::date_time t2(2018, 10, 1, 0, 0, 0, 0);
    EXPECT_NE(t1.to_utc(), amo::date_time::now());
    EXPECT_EQ(t2.to_utc(), (int64_t)1538294400000LL);
}

TEST(date_time, milliseconds) {
    amo::date_time t1;
    amo::date_time t2(2018, 10, 1, 0, 0, 0, 0);
    EXPECT_EQ(t1.milliseconds(), amo::date_time::now() % 1000);
    EXPECT_EQ(t2.milliseconds(), 0);
}

TEST(date_time, seconds) {
    amo::date_time t1(2018, 10, 1, 0, 1, 1, 0);
    EXPECT_EQ(t1.seconds(), 1);
}

TEST(date_time, minutes) {
    amo::date_time t1(2018, 10, 1, 0, 1, 1, 0);
    EXPECT_EQ(t1.minutes(), 1);
}

TEST(date_time, hours) {
    amo::date_time t1(2018, 10, 1, 0, 1, 1, 0);
    EXPECT_EQ(t1.hours(), 0);
}

TEST(date_time, day_in_month) {
    amo::date_time t1(2018, 10, 1, 0, 1, 1, 0);
    EXPECT_EQ(t1.day_in_month(), 1);
}

TEST(date_time, month_in_year) {
    amo::date_time t1(2018, 10, 1, 0, 1, 1, 0);
    EXPECT_EQ(t1.month_in_year(), 10);
}

TEST(date_time, years) {
    amo::date_time t1(2018, 10, 1, 0, 1, 1, 0);
    EXPECT_EQ(t1.years(), 2018);
}

TEST(date_time, week_day) {
    amo::date_time t1(2018, 10, 1, 0, 1, 1, 0);
    EXPECT_EQ(t1.week_day(), 1);
}

TEST(date_time, day_in_year) {
    amo::date_time t1(2018, 1, 1, 0, 1, 1, 0);
    EXPECT_EQ(t1.day_in_year(), 0);
}


TEST(date_time, isLeap) {
    amo::date_time t1(2018, 1, 1, 0, 1, 1, 0);
    EXPECT_EQ(t1.isLeap(), false);
}


TEST(date_time, lastDayOfMonth) {
    amo::date_time t1(2018, 1, 1, 0, 1, 1, 0);
    EXPECT_EQ(t1.lastDayOfMonth(), 31);
}


TEST(date_time, operatorOverride) {
    amo::date_time t1(2018, 1, 1, 0, 1, 1, 0);
    t1 =  t1 + amo::date_time::year(1);
    std::cout << t1.to_string() << std::endl;
    EXPECT_EQ(t1.years(), 2019);
    
    t1 = t1 + amo::date_time::month(12);
    std::cout << t1.to_string() << std::endl;
    EXPECT_EQ(t1.years(), 2020);
    EXPECT_EQ(t1.month_in_year(), 1);
    
    t1 = t1 + amo::date_time::day(32);
    std::cout << t1.to_string() << std::endl;
    EXPECT_EQ(t1.years(), 2020);
    EXPECT_EQ(t1.month_in_year(), 2);
    EXPECT_EQ(t1.day_in_month(), 2);
    
    t1 = t1 + amo::date_time::hour(49);
    std::cout << t1.to_string() << std::endl;
    EXPECT_EQ(t1.day_in_month(), 4);
    EXPECT_EQ(t1.hours(), 1);
    
    t1 = t1 + amo::date_time::minute(90);
    std::cout << t1.to_string() << std::endl;
    EXPECT_EQ(t1.hours(), 2);
    EXPECT_EQ(t1.minutes(), 31);
    
    t1 = t1 + amo::date_time::second(3661);
    std::cout << t1.to_string() << std::endl;
    EXPECT_EQ(t1.hours(), 3);
    EXPECT_EQ(t1.minutes(), 32);
    EXPECT_EQ(t1.seconds(), 2);
    
    t1 = t1 + 3661;
    std::cout << t1.to_string() << std::endl;
    EXPECT_EQ(t1.hours(), 3);
    EXPECT_EQ(t1.minutes(), 32);
    EXPECT_EQ(t1.seconds(), 5);
    EXPECT_EQ(t1.milliseconds(), 661);
    
    // operator -
    t1 = t1 - 3661;
    std::cout << t1.to_string() << std::endl;
    EXPECT_EQ(t1.hours(), 3);
    EXPECT_EQ(t1.minutes(), 32);
    EXPECT_EQ(t1.seconds(), 2);
    
    t1 = t1 - amo::date_time::second(3661);
    std::cout << t1.to_string() << std::endl;
    EXPECT_EQ(t1.hours(), 2);
    EXPECT_EQ(t1.minutes(), 31);
    
    t1 = t1 - amo::date_time::minute(90);
    std::cout << t1.to_string() << std::endl;
    EXPECT_EQ(t1.day_in_month(), 4);
    EXPECT_EQ(t1.hours(), 1);
    
    t1 = t1 - amo::date_time::hour(49);
    std::cout << t1.to_string() << std::endl;
    EXPECT_EQ(t1.years(), 2020);
    EXPECT_EQ(t1.month_in_year(), 2);
    EXPECT_EQ(t1.day_in_month(), 2);
    
    t1 = t1 - amo::date_time::day(32);
    std::cout << t1.to_string() << std::endl;
    EXPECT_EQ(t1.years(), 2020);
    EXPECT_EQ(t1.month_in_year(), 1);
    
    t1 = t1 - amo::date_time::month(12);
    std::cout << t1.to_string() << std::endl;
    EXPECT_EQ(t1.years(), 2019);
    
    t1 = t1 - amo::date_time::year(1);
    std::cout << t1.to_string() << std::endl;
    EXPECT_EQ(t1.years(), 2018);
}




TEST(date_time, to_string) {
    amo::date_time t1(2018, 1, 1, 0, 1, 1, 0);
    EXPECT_EQ(t1.to_string(), "2018-01-01 00:01:01");
}

TEST(date_time, format) {
    amo::date_time t1(2018, 1, 1, 0, 1, 1, 0);
    // 这种写法无法获取正确的结果
    EXPECT_NE(t1.format("yyyyMMdd_hmmss"), "20180101_00101");
    EXPECT_EQ(t1.format("yyyyMMdd_h*mm*ss", "*"), "20180101_00101");
    EXPECT_EQ(t1.format("yyyyMMdd_h*m*s", "*"), "20180101_011");
    EXPECT_EQ(t1.format("yyMdd_homos", "o"), "18101_011");
    EXPECT_EQ(t1.format("yyMdd_h_mm_ss.ms"), "18101_0_01_01.000");
}
