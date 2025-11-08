#include <gtest/gtest.h>
#include <rsa/rsa_core.h>

class RSACoreTest : public ::testing::Test
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

TEST_F(RSACoreTest, RSACoreTest)
{
    RSAPrivateKey pk(2048);

    pk.genKey("/tmp/rsa.key");
    pk.genPubKey("/tmp/rsa.pub");

    RSAPrivateKey pk1("/tmp/rsa.key");
    RSAPublicKey pk2("/tmp/rsa.pub");

    BigInt x = BigInt(0x114514);

    BigInt ct = pk2.encrypt(x);
    BigInt dct = pk1.decrypt(ct);
    ASSERT_EQ(dct, x);

    BigInt s = pk1.decrypt(x);
    ASSERT_TRUE(pk2.verify(x, s));
}