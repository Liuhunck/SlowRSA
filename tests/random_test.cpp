#include <gtest/gtest.h>
#include <rsa/random.h>

class RandomTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        BNRandom::initRandom();
    }

    void TearDown() override
    {
    }
};

TEST_F(RandomTest, RandomByteTest)
{
    unsigned char c = BNRandom::getRandByte();
    ASSERT_TRUE(c <= 0xFF);
}

TEST_F(RandomTest, RandomWordTest)
{
    for (int i = 0; i < 8; ++i)
    {
        BIT c = BNRandom::getRandWord();
        BigInt::debug(c);
    }
}

TEST_F(RandomTest, RandomIntTest)
{
    int max_test = 4096;
    for (int i = 1; i <= max_test; ++i)
    {
        BigInt p = BNRandom::getRandInt(i);
        ASSERT_EQ(p.bits(), i);
        p = BNRandom::getRandInt(i, false);
        ASSERT_TRUE(p.bits() <= i);
    }
}

TEST_F(RandomTest, RandomPrimeTest)
{
    int max_test = 512;
    for (int i = max_test; i <= max_test; ++i)
    {
        BigInt p = BNRandom::getRandPrime(i);
        p.debug();
        ASSERT_TRUE(p.bits() >= i);
    }
}