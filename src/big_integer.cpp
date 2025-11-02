#include <rsa/big_integer.h>

#include <cstring>
#include <cassert>
#include <iostream>

BigInt::BigInt(const int c)
{
    if (c < 0)
    {
        sign = true;
        digits.push_back(-c);
    }
    else if (c > 0)
    {
        digits.push_back(c);
    }
}

static char
char2hex(char ch)
{
    if ('0' <= ch && ch <= '9')
        return ch ^ '0';
    if ('A' <= ch && ch <= 'F')
        return ch - 'A' + 10;
    if ('a' <= ch && ch <= 'f')
        return ch - 'a' + 10;
    assert(ch == ' ' || ch == '_');
    return -1;
}

static char
hex2char(int ch)
{
    assert(ch < 16);
    if (ch < 10)
        return '0' + ch;
    return 'A' + ch - 10;
}

BigInt::BigInt(const char *str, int len)
{
    if (len == 0)
    {
        len = strlen(str);
    }
    if (len < 2)
        std::__throw_invalid_argument("len < 2");
    if (str[0] == '-')
    {
        sign = true;
        ++str;
        --len;
    }
    if (str[0] != '0' || str[1] != 'x')
        std::__throw_invalid_argument("fromString only support hex");
    int cnt = 0;
    BIT tmp = 0;
    for (int i = len - 1; i > 1; --i)
    {
        char hex = char2hex(str[i]);
        if (hex == -1)
            continue;
        tmp |= static_cast<BIT>(hex) << cnt;

        cnt += 4;
        if (cnt == BITL)
        {
            digits.push_back(tmp);
            cnt = 0;
            tmp = 0;
        }
    }
    if (tmp != 0)
        digits.push_back(tmp);
}

BigInt::BigInt(const std::string &str) : BigInt(str.c_str(), str.length()) {}

BigInt::operator bool() const
{
    return digits.size() != 0;
}

bool BigInt::operator!() const
{
    return digits.size() == 0;
}

bool BigInt::operator==(const int other) const
{
    if (other == 0)
        return digits.size() == 0;
    bool flag = other > 0;
    if (flag != sign)
        return false;
    return digits[0] == static_cast<BIT>(flag ? -other : other);
}

bool BigInt::operator==(const BigInt &other) const
{
    int n1 = digits.size();
    int n2 = other.digits.size();

    if (n1 != n2)
        return false;

    if (n1 == 0)
        return true;

    if (sign != other.sign)
        return false;

    for (int i = 0; i < n1; ++i)
        if (digits[i] != other.digits[i])
            return false;
    return true;
}

bool BigInt::operator!=(const BigInt &other) const
{
    return !(*this == other);
}

static int
unsign_compare(const std::vector<BIT> &a1, const std::vector<BIT> &a2)
{
    int n1 = a1.size();
    int n2 = a2.size();
    if (n1 > n2)
        return 1;
    if (n1 < n2)
        return -1;
    for (int i = n1 - 1; i >= 0; --i)
    {
        if (a1[i] > a2[i])
            return 1;
        if (a1[i] < a2[i])
            return -1;
    }
    return 0;
}

static std::vector<BIT>
eval_add_or_sub(const std::vector<BIT> &a1, const std::vector<BIT> &a2, bool is_sub)
{
    int n1 = a1.size();
    int n2 = a2.size();
    assert(n1 >= n2);

    std::vector<BIT> res(n1, 0);
    BITT pre = 0;

    if (is_sub)
    {
        for (int i = 0; i < n2; ++i)
        {
            pre += a2[i];
            if (pre > a1[i])
            {
                res[i] = static_cast<BIT>(-pre + BIT_MAX + a1[i]);
                pre = 1;
            }
            else
            {
                res[i] = static_cast<BIT>(-pre + a1[i]);
                pre = 0;
            }
        }
        for (int i = n2; i < n1; ++i)
        {
            if (pre > a1[i])
            {
                res[i] = static_cast<BIT>(-pre + BIT_MAX + a1[i]);
                pre = 1;
            }
            else
            {
                res[i] = static_cast<BIT>(-pre + a1[i]);
                pre = 0;
            }
        }
        assert(pre == 0);
        while (!res.empty() && res.back() == 0)
            res.pop_back();
    }
    else
    {
        for (int i = 0; i < n2; ++i)
        {
            pre = pre + a1[i] + a2[i];
            res[i] = static_cast<BIT>(pre);
            pre >>= BITL;
        }

        for (int i = n2; i < n1; ++i)
        {
            pre = pre + a1[i];
            res[i] = static_cast<BIT>(pre);
            pre >>= BITL;
        }

        if (pre)
            res.push_back(pre);
    }

    return res;
}

BigInt
BigInt::operator+(const BigInt &other) const
{
    int cmp = unsign_compare(digits, other.digits);
    bool sub = sign != other.sign;
    if (cmp == 0 && sub)
        return BigInt(0);
    if (cmp >= 0)
        return BigInt(eval_add_or_sub(digits, other.digits, sub), sign);
    else
        return BigInt(eval_add_or_sub(other.digits, digits, sub), other.sign);
}

BigInt
BigInt::operator-(const BigInt &other) const
{
    bool sign1 = !other.sign;
    int cmp = unsign_compare(digits, other.digits);
    bool sub = sign != sign1;
    if (cmp == 0 && sub)
        return BigInt(0);
    if (cmp >= 0)
        return BigInt(eval_add_or_sub(digits, other.digits, sub), sign);
    else
        return BigInt(eval_add_or_sub(other.digits, digits, sub), sign1);
}

BigInt
BigInt::operator*(const BigInt &other) const
{
    if (!*this || !other)
        return BigInt(0);

    int n1 = digits.size();
    int n2 = other.digits.size();
    int n = n1 + n2 - 1;

    std::vector<BITT> tmp(n + 1, 0);
    for (int i = 0; i < n1; ++i)
        for (int j = 0; j < n2; ++j)
        {
            auto now = static_cast<BITT>(1) * digits[i] * other.digits[j];
            tmp[i + j + 1] += (now >> BITL);
            tmp[i + j] += static_cast<BIT>(now);
        }

    BITT pre = 0;
    std::vector<BIT> res(n + 1, 0);
    for (int i = 0; i <= n; ++i)
    {
        pre += tmp[i];
        res[i] = static_cast<BIT>(pre);
        pre >>= BITL;
    }
    assert(pre == 0);
    while (res.back() == 0)
        res.pop_back();

    return BigInt(res, sign != other.sign);
}

std::string
BigInt::toString() const
{
    int n = digits.size();
    if (n == 0)
        return "0x0";

    std::string res = sign ? "-0x" : "0x";
    for (int j = 4; j <= BITL; j += 4)
    {
        BIT now = digits.back() >> (BITL - j);
        if (now == 0)
            continue;
        res += hex2char(now & 0xF);
    }

    for (int i = n - 2; i >= 0; --i)
        for (int j = 4; j <= BITL; j += 4)
            res += hex2char((digits[i] >> (BITL - j)) & 0xF);

    return res;
}

void BigInt::debug() const
{
    fprintf(stderr, "%d %zu\n", sign, digits.size());
    for (auto x : digits)
        fprintf(stderr, "%#llX ", x);
    fprintf(stderr, "\n");
}