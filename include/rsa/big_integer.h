#ifndef RSA_BIG_INTEGER_H
#define RSA_BIG_INTEGER_H

#include <vector>
#include <string>
#include "prime.h"

#define BIT_USE_64
#define BIT_USE_BARRETT

#if defined(BIT_USE_64)
#define BIT unsigned long long
#define BITT __uint128_t
#define BITL 64
#define BIT_CLZ __builtin_clzll
#define BIT_CTZ __builtin_ctzll
#else
#define BIT unsigned int
#define BITT unsigned long long
#define BITL 32
#define BIT_CLZ __builtin_clz
#define BIT_CTZ __builtin_ctz
#endif

#define BIT_MAX (static_cast<BITT>(1) << BITL)
#define BIT_MASK (BIT_MAX - 1)

class BigInt
{
protected:
    std::vector<BIT> digits;
    bool sign = false;

public:
    BigInt(const int c = 0);
    BigInt(const char *str, int len = 0);
    BigInt(const std::string &str);

    BigInt(const BigInt &other) : digits(other.digits), sign(other.sign) {}
    BigInt(BigInt &&other) : digits(std::move(other.digits)), sign(other.sign) {}
    void operator=(const BigInt &other) { digits = other.digits, sign = other.sign; }
    void operator=(BigInt &&other) { digits = std::move(other.digits), sign = other.sign; }
    BigInt(std::vector<BIT> &&digits, bool sign) : digits(digits), sign(sign) {}
    BigInt(const std::vector<BIT> &digits, bool sign) : digits(digits), sign(sign) {}

    int clz() const;
    int ctz() const;
    int bits() const;

    // 布尔运算
    explicit operator bool() const;
    bool operator!() const;
    bool operator!=(const BigInt &other) const;

    bool operator==(const int other) const;
    bool operator==(const BigInt &other) const;

    bool operator>(const int other) const;
    bool operator>(const BigInt &other) const;

    bool operator<(const int other) const;
    bool operator<(const BigInt &other) const;

    // 基本算术运算
    BigInt operator+(const BigInt &other) const;
    BigInt &operator+=(const int other);

    BigInt operator-(const int other) const;
    BigInt operator-(const BigInt &other) const;

    BigInt operator*(const BigInt &other) const;

    BigInt operator/(const BigInt &other) const;

    prime_t operator%(const prime_t other) const;
    BigInt operator%(const BigInt &other) const;

    std::pair<BigInt, BigInt> divAndMod(const BigInt &other) const;

    // 基本位运算
    BigInt operator|(const unsigned int other) const;
    void operator|=(const unsigned int other);

    BigInt operator>>(const int bits) const;
    BigInt &operator>>=(const int bits);

    BigInt operator<<(const int bits) const;

    // 模幂运算 (核心)
    BigInt modPow(const BigInt &exp, const BigInt &mod) const
    {
#if defined(BIT_USE_BARRETT)
        return modPowBarrett(exp, mod);
#else
        return modPowBasic(exp, mod);
#endif
    }
    BigInt modPowBasic(const BigInt &exp, const BigInt &mod) const;
    BigInt modPowBarrett(const BigInt &exp, const BigInt &mod) const;

    // 转换函数
    std::string toString() const;

    // Just for debug
    void debug() const;
    static void debug(const BIT, char end = '\n');
    static void debug(const std::vector<BIT> &);
};

#endif