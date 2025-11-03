#include <gtest/gtest.h>
#include <rsa/big_integer.h>
#include <iostream>
#include <fstream>

class BigIntegerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(BigIntegerTest, ToFromString)
{
    const char *ch = "0x8946541324844FA861635978465149876579846523458923458792389457923745896238945789273495872938475982374895738925AAAAA7984654679846FFFAAACCCCCCCCCCCCCBCFF9874651";
    auto a = BigInt(ch);

    auto to = a.toString();
    ASSERT_EQ(to, ch);

    const std::string x = "-0x1220866154878AAAFFCCCBBB";
    a = BigInt(x);
    to = a.toString();
    ASSERT_EQ(to, x);
}

TEST_F(BigIntegerTest, AddTest)
{
    ASSERT_EQ(system("python3 ../../scripts/big_integer_test_gen.py add -n 20 -m 1000000 -s 0"), 0);
    std::ifstream f("/tmp/big_integer_test_add");
    ASSERT_TRUE(f.is_open());

    std::string line;
    while (std::getline(f, line))
    {
        BigInt x = BigInt(line);
        ASSERT_EQ(x.toString(), line);

        ASSERT_TRUE(std::getline(f, line));
        BigInt y = BigInt(line);
        ASSERT_EQ(y.toString(), line);

        ASSERT_TRUE(std::getline(f, line));
        BigInt z = BigInt(line);
        ASSERT_EQ(z.toString(), line);

        ASSERT_EQ(x + y, z);
    }
    f.close();
}

TEST_F(BigIntegerTest, SubTest)
{
    ASSERT_EQ(system("python3 ../../scripts/big_integer_test_gen.py sub -n 20 -m 1000000 -s 0"), 0);
    std::ifstream f("/tmp/big_integer_test_sub");
    ASSERT_TRUE(f.is_open());

    std::string line;
    while (std::getline(f, line))
    {
        BigInt x = BigInt(line);
        ASSERT_EQ(x.toString(), line);

        ASSERT_TRUE(std::getline(f, line));
        BigInt y = BigInt(line);
        ASSERT_EQ(y.toString(), line);

        ASSERT_TRUE(std::getline(f, line));
        BigInt z = BigInt(line);
        ASSERT_EQ(z.toString(), line);

        ASSERT_EQ(x - y, z);
    }
    f.close();
}

TEST_F(BigIntegerTest, MulTest)
{
    ASSERT_EQ(system("python3 ../../scripts/big_integer_test_gen.py mul -n 30 -m 100000 -s 0"), 0);
    std::ifstream f("/tmp/big_integer_test_mul");
    ASSERT_TRUE(f.is_open());

    std::string line;
    while (std::getline(f, line))
    {
        BigInt x = BigInt(line);
        ASSERT_EQ(x.toString(), line);

        ASSERT_TRUE(std::getline(f, line));
        BigInt y = BigInt(line);
        ASSERT_EQ(y.toString(), line);

        ASSERT_TRUE(std::getline(f, line));
        BigInt z = BigInt(line);
        ASSERT_EQ(z.toString(), line);

        ASSERT_EQ(x * y, z);
    }
    f.close();
}

TEST_F(BigIntegerTest, DivTest)
{
    ASSERT_EQ(system("python3 ../../scripts/big_integer_test_gen.py div -n 10 -m 100000 -s 0"), 0);
    std::ifstream f("/tmp/big_integer_test_div");
    ASSERT_TRUE(f.is_open());

    std::string line;
    while (std::getline(f, line))
    {
        BigInt x = BigInt(line);
        ASSERT_EQ(x.toString(), line);

        ASSERT_TRUE(std::getline(f, line));
        BigInt y = BigInt(line);
        ASSERT_EQ(y.toString(), line);

        ASSERT_TRUE(std::getline(f, line));
        BigInt z = BigInt(line);
        ASSERT_EQ(z.toString(), line);

        ASSERT_TRUE(std::getline(f, line));
        BigInt zm = BigInt(line);
        ASSERT_EQ(zm.toString(), line);

        auto [xdy, xmy] = x.divAndMod(y);
        ASSERT_EQ(xdy, z);
        ASSERT_EQ(xmy, zm);

        ASSERT_EQ(x / y, z);
        ASSERT_EQ(x % y, zm);
    }
    f.close();
}