#ifndef RSA_BIG_INTEGER_H
#define RSA_BIG_INTEGER_H

#include <vector>
#include <string>

#define BIT_USE_64

#ifdef BIT_USE_64
#define BIT unsigned long long
#define BITT __uint128_t
#define BITL 64
#else
#define BIT unsigned int
#define BITT unsigned long long
#define BITL 32
#endif

#define BIT_MAX (static_cast<BITT>(1) << BITL)
#define BIT_MASK (BIT_MAX - 1)

class BigInt
{
private:
    std::vector<BIT> digits;
    bool sign = false;

public:
    BigInt(const int c = 0);
    BigInt(const char *str, int len = 0);
    BigInt(const std::string &str);

    BigInt(std::vector<BIT> &&digits, bool sign) : digits(digits), sign(sign) {}
    BigInt(const std::vector<BIT> &digits, bool sign) : digits(digits), sign(sign) {}

    // 布尔运算
    explicit operator bool() const;

    bool operator!() const;
    bool operator==(const int other) const;
    bool operator==(const BigInt &other) const;
    bool operator!=(const BigInt &other) const;

    // 基本算术运算
    BigInt operator+(const BigInt &other) const;
    BigInt operator-(const BigInt &other) const;
    BigInt operator*(const BigInt &other) const;
    BigInt operator/(const BigInt &other) const;
    BigInt operator%(const BigInt &other) const;
    std::pair<BigInt, BigInt> divAndMod(const BigInt &other) const;

    // 基本位运算
    BigInt operator>>(const unsigned int bits) const;
    BigInt operator<<(const unsigned int bits) const;

    // 模幂运算 (核心)
    BigInt modPow(const BigInt &exp, const BigInt &mod) const;

    // 数论函数
    BigInt gcd(const BigInt &other) const;
    BigInt modInverse(const BigInt &mod) const;

    // 随机数生成
    static BigInt random(int bitLength);
    static BigInt randomPrime(int bitLength);

    // 转换函数
    std::string toString() const;

    // Just for debug
    void debug() const;
    static void debug(const std::vector<BIT> &);
};

#endif