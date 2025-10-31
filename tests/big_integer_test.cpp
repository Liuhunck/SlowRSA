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

    // fprintf(stderr, "%zu\n", a.digits.size());
    // for (auto x : a.digits)
    //     fprintf(stderr, "%#llX ", x);
    // fprintf(stderr, "\n");

    auto to = a.toString();
    ASSERT_EQ(to, ch);

    const std::string x = "-0x1220866154878AAAFFCCCBBB";
    a = BigInt(x);
    to = a.toString();
    ASSERT_EQ(to, x);
}

TEST_F(BigIntegerTest, AddTest)
{
    system("python3 ../scripts/big_integer_test_gen add 100");
    std::ifstream f("/tmp/big_integer_test_gen_add.txt");
    ASSERT_TRUE(f.is_open());

    std::string line;
    while (std::getline(f, line))
    {
        BigInt x = BigInt(line);
        ASSERT_TRUE(std::getline(f, line));
        BigInt y = BigInt(line);
        ASSERT_TRUE(std::getline(f, line));
        BigInt z = BigInt(line);
        ASSERT_EQ(x + y, z);
    }
}