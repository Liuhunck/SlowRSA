#include <gtest/gtest.h>
#include <rsa/big_integer.h>
#include <rsa/random.h>

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
    ASSERT_EQ(system("python3 ../../scripts/big_integer_test_gen.py div -n 10 -m 50000 -s 0"), 0);
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

TEST_F(BigIntegerTest, RshiftTest)
{
    ASSERT_EQ(system("python3 ../../scripts/big_integer_test_gen.py rshift -n 1000 -m 10000 -s 0"), 0);
    std::ifstream f("/tmp/big_integer_test_rshift");
    ASSERT_TRUE(f.is_open());

    std::string line;
    while (std::getline(f, line))
    {
        BigInt x = BigInt(line);
        ASSERT_EQ(x.toString(), line);

        ASSERT_TRUE(std::getline(f, line));
        int b = std::stoi(line);

        ASSERT_TRUE(std::getline(f, line));
        BigInt y = BigInt(line);
        ASSERT_EQ(y.toString(), line);

        ASSERT_EQ(x >> b, y);
        ASSERT_EQ(x >>= b, y);
    }
    f.close();
}

TEST_F(BigIntegerTest, CompareTest)
{
    int max_test = 1000;
    for (int i = 0; i < max_test; ++i)
    {
        int x = rand() - rand();
        int y = rand() - rand();
        std::cerr << x << " " << y << std::endl;
        ASSERT_EQ(BigInt(x) < y, x < y);
        ASSERT_EQ(BigInt(x) > y, x > y);
        ASSERT_EQ(BigInt(x) == y, x == y);

        ASSERT_EQ(BigInt(x) < x, x < x);
        ASSERT_EQ(BigInt(x) > x, x > x);
        ASSERT_EQ(BigInt(x) == x, x == x);

        ASSERT_EQ(BigInt() < x, 0 < x);
        ASSERT_EQ(BigInt() > x, 0 > x);
        ASSERT_EQ(BigInt() == x, 0 == x);

        ASSERT_EQ(BigInt(y) < 0, y < 0);
        ASSERT_EQ(BigInt(y) > 0, y > 0);
        ASSERT_EQ(BigInt(y) == 0, y == 0);
    }
}

TEST_F(BigIntegerTest, ModPowTest)
{
    ASSERT_EQ(system("python3 ../../scripts/big_integer_test_gen.py modpow -n 10 -m 1000 -s 0"), 0);
    std::ifstream f("/tmp/big_integer_test_modpow");
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
        BigInt m = BigInt(line);
        ASSERT_EQ(m.toString(), line);

        ASSERT_EQ(x.modPow(y, z), m);
    }
    f.close();
}

TEST_F(BigIntegerTest, BigCmpTest)
{
    ASSERT_EQ(system("python3 ../../scripts/big_integer_test_gen.py bigcmp -n 10000 -m 1000 -s 0"), 0);
    std::ifstream f("/tmp/big_integer_test_bigcmp");
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
        std::stringstream c(line);
        bool c0, c1, c2;
        ASSERT_TRUE(c >> c0 >> c1 >> c2);

        ASSERT_TRUE(std::getline(f, line));
        std::stringstream d(line);
        bool d0, d1, d2;
        ASSERT_TRUE(d >> d0 >> d1 >> d2);

        x.debug();
        y.debug();

        ASSERT_EQ(x > y, c0);
        ASSERT_EQ(x == y, c1);
        ASSERT_EQ(x < y, c2);

        ASSERT_EQ(x > BigInt(), d0);
        ASSERT_EQ(x == BigInt(), d1);
        ASSERT_EQ(x < BigInt(), d2);

        ASSERT_EQ(BigInt() < x, d0);
        ASSERT_EQ(BigInt() == x, d1);
        ASSERT_EQ(BigInt() > x, d2);

        ASSERT_EQ(x > x, false);
        ASSERT_EQ(x == x, true);
        ASSERT_EQ(x < x, false);
    }
    f.close();
}