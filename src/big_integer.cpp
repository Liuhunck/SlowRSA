#include <rsa/big_integer.h>

#include <bitset>
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

unsigned int
BigInt::ctz() const
{
    int n = digits.size();
    unsigned int res = 0;
    for (int i = 0; i < n; ++i)
    {
        if (digits[i] == 0)
            res += BITL;
        else
        {
            res += BIT_CTZ(digits[i]);
            return res;
        }
    }
    return res;
}

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

bool BigInt::operator>(const int other) const
{
    bool sign1 = other < 0;
    if (sign != sign1)
        return sign1;
    BIT a = static_cast<BIT>(sign ? -other : other);
    bool ret = true;
    switch (digits.size())
    {
    case 0:
        if (a == 0)
            return false;
        ret = false;
        break;
    case 1:
        if (a == digits.front())
            return false;
        ret = digits.front() > a;
        break;
    default:
        break;
    }
    return ret != sign;
}

bool BigInt::operator>(const BigInt &other) const
{
    if (other.sign != sign)
        return other.sign;
    int cmp = unsign_compare(digits, other.digits);
    return (cmp != 0) && ((cmp > 0) != sign);
}

bool BigInt::operator<(const int other) const
{
    bool sign1 = other < 0;
    if (sign != sign1)
        return sign;
    BIT a = static_cast<BIT>(sign ? -other : other);
    bool ret = false;
    switch (digits.size())
    {
    case 0:
        if (a == 0)
            return false;
        ret = true;
        break;
    case 1:
        if (a == digits.front())
            return false;
        ret = digits.front() < a;
        break;
    default:
        break;
    }
    return ret != sign;
}

bool BigInt::operator<(const BigInt &other) const
{
    int flag = other.sign != sign;
    int cmp = unsign_compare(digits, other.digits);
    return (cmp != 0) && ((cmp < 0) != flag);
}

static std::vector<BIT>
unsign_add(const std::vector<BIT> &a1, const std::vector<BIT> &a2)
{
    int n1 = a1.size();
    int n2 = a2.size();
    assert(n1 >= n2);

    std::vector<BIT> res(n1, 0);
    BITT pre = 0;

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
    assert((pre >> BITL) == 0);

    return res;
}

static std::vector<BIT>
unsign_sub(const std::vector<BIT> &a1, const std::vector<BIT> &a2)
{
    int n1 = a1.size();
    int n2 = a2.size();
    assert(n1 >= n2);

    std::vector<BIT> res(n1, 0);
    BITT pre = 0;

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

    return res;
}

static void
unsign_sub_inplace(std::vector<BIT> &a1, const std::vector<BIT> &a2)
{
    int n1 = a1.size();
    int n2 = a2.size();
    assert(n1 >= n2);

    BITT pre = 0;

    for (int i = 0; i < n2; ++i)
    {
        pre += a2[i];
        if (pre > a1[i])
        {
            a1[i] = static_cast<BIT>(-pre + BIT_MAX + a1[i]);
            pre = 1;
        }
        else
        {
            a1[i] = static_cast<BIT>(-pre + a1[i]);
            pre = 0;
        }
    }
    for (int i = n2; i < n1; ++i)
    {
        if (pre > a1[i])
        {
            a1[i] = static_cast<BIT>(-pre + BIT_MAX + a1[i]);
            pre = 1;
        }
        else
        {
            a1[i] = static_cast<BIT>(-pre + a1[i]);
            pre = 0;
        }
    }

    assert(pre == 0);
    while (!a1.empty() && a1.back() == 0)
        a1.pop_back();
}

static inline std::vector<BIT>
unsign_add_or_sub(const std::vector<BIT> &a1, const std::vector<BIT> &a2, bool is_sub)
{
    return is_sub ? unsign_sub(a1, a2) : unsign_add(a1, a2);
}

BigInt
BigInt::operator+(const BigInt &other) const
{
    int cmp = unsign_compare(digits, other.digits);
    bool sub = sign != other.sign;
    if (cmp == 0 && sub)
        return BigInt(0);

    if (cmp >= 0)
        return BigInt(unsign_add_or_sub(digits, other.digits, sub), sign);
    else
        return BigInt(unsign_add_or_sub(other.digits, digits, sub), other.sign);
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
        return BigInt(unsign_add_or_sub(digits, other.digits, sub), sign);
    else
        return BigInt(unsign_add_or_sub(other.digits, digits, sub), sign1);
}

static std::vector<BIT>
unsign_mul_one(const std::vector<BIT> &a1, BIT a2)
{
    if (a2 == 0)
        return std::vector<BIT>();

    int n1 = a1.size();
    std::vector<BIT> res(n1, 0);

    BITT pre = 0;
    for (int i = 0; i < n1; ++i)
    {
        pre += static_cast<BITT>(1) * a1[i] * a2;
        res[i] = static_cast<BIT>(pre);
        pre >>= BITL;
    }

    if (pre)
        res.push_back(pre);
    assert((pre >> BITL) == 0);

    return res;
}

static void
unsign_mul_one_inplace(std::vector<BIT> &a1, BIT a2)
{
    if (a2 == 0)
        return a1.clear(), void();

    int n1 = a1.size();

    BITT pre = 0;
    for (int i = 0; i < n1; ++i)
    {
        pre += static_cast<BITT>(1) * a1[i] * a2;
        a1[i] = static_cast<BIT>(pre);
        pre >>= BITL;
    }

    if (pre)
        a1.push_back(pre);
    assert((pre >> BITL) == 0);
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

    return BigInt(std::move(res), sign != other.sign);
}

static std::pair<std::vector<BIT>, std::vector<BIT>>
unsign_div_and_mod(const std::vector<BIT> &a1, const std::vector<BIT> &a2)
{
    int n1 = a1.size();
    int n2 = a2.size();
    assert(n2 != 0 && n1 >= n2);
    int n = n1 - n2 + 1;

    std::vector<BIT> res(n, 0);
    std::vector<BIT> rem(n2, 0);
    for (int i = 0; i < n2; ++i)
        rem[i] = a1[n + i - 1];
    assert(rem.back() != 0);

    int nr;
    BITT x, y = a2.back(), l, r, mid;
    for (int i = n - 1; i >= 0; --i)
    {
        x = 0;
        nr = rem.size();
        if (nr > n2)
        {
            assert(nr == n2 + 1);
            x = (static_cast<BITT>(rem[n2]) << BITL) | static_cast<BITT>(rem[n2 - 1]);
        }
        else if (nr == n2)
            x = rem[n2 - 1];
        else
            goto div_end;

        r = std::min(x / y + 1, BIT_MAX), l = x / (y + 1);
        while (l + 1 < r)
        {
            mid = (l + r) >> 1;
            int cmp = unsign_compare(rem, unsign_mul_one(a2, static_cast<BIT>(mid)));
            if (cmp == 0)
            {
                res[i] = static_cast<BIT>(mid);
                rem.clear();
                goto div_end;
            }
            if (cmp > 0)
                l = mid;
            else
                r = mid;
        }
        if (l)
        {
            res[i] = static_cast<BIT>(l);
            unsign_sub_inplace(rem, unsign_mul_one(a2, res[i]));
            assert(unsign_compare(rem, a2) <= 0);
        }

        if (i == 0)
            continue;

    div_end:
        nr = rem.size();
        rem.push_back(0);
        for (int j = nr; j >= 1; --j)
            rem[j] = rem[j - 1];
        rem[0] = a1[i - 1];
        assert(rem.back() != 0);
    }

    while (!res.empty() && res.back() == 0)
        res.pop_back();

    return std::make_pair(std::move(res), std::move(rem));
}

BigInt
BigInt::operator/(const BigInt &other) const
{
    int n1 = digits.size();
    int n2 = other.digits.size();
    if (n2 == 0)
        throw std::runtime_error("div zero occured...");

    if (n1 == 0)
        return BigInt();

    bool flag = sign != other.sign;

    int cmp = unsign_compare(digits, other.digits);
    if (cmp < 0)
        return BigInt();
    if (cmp == 0)
        return BigInt(flag ? -1 : 1);

    auto [res, _] = unsign_div_and_mod(digits, other.digits);
    return BigInt(std::move(res), flag);
}

BigInt
BigInt::operator%(const BigInt &other) const
{
    int n1 = digits.size();
    int n2 = other.digits.size();
    if (n2 == 0)
        throw std::runtime_error("div zero occured...");

    if (n1 == 0)
        return BigInt();

    bool flag = sign != other.sign;

    int cmp = unsign_compare(digits, other.digits);
    if (cmp < 0)
        return BigInt(digits, flag);
    if (cmp == 0)
        return BigInt();

    auto [_, rem] = unsign_div_and_mod(digits, other.digits);
    return BigInt(std::move(rem), flag);
}

std::pair<BigInt, BigInt>
BigInt::divAndMod(const BigInt &other) const
{
    int n1 = digits.size();
    int n2 = other.digits.size();
    if (n2 == 0)
        throw std::runtime_error("div zero occured...");

    if (n1 == 0)
        return std::make_pair(BigInt(), BigInt());

    bool flag = sign != other.sign;

    int cmp = unsign_compare(digits, other.digits);
    if (cmp < 0)
        return std::make_pair(BigInt(), BigInt(digits, flag));
    if (cmp == 0)
        return std::make_pair(BigInt(flag ? -1 : 1), BigInt());

    auto [res, rem] = unsign_div_and_mod(digits, other.digits);
    return std::make_pair(BigInt(std::move(res), flag), BigInt(std::move(rem), flag));
}

BigInt
BigInt::operator|(const unsigned int other) const
{
    std::vector<BIT> res = digits;
    if (res.empty())
        res.push_back(other);
    else
        res.front() |= other;
    return BigInt(std::move(res), sign);
}

void BigInt::operator|=(const unsigned int other)
{
    if (digits.empty())
        digits.push_back(other);
    else
        digits.front() |= other;
}

BigInt
BigInt::operator>>(const unsigned int bits) const
{
    int x = bits / BITL;
    int y = bits % BITL;
    int n = digits.size();
    const BIT mask = (static_cast<BIT>(1) << y) - 1;

    std::vector<BIT> res(n - x, 0);

    for (int i = 0; i < n - x - 1; ++i)
        res[i] = (digits[i + x] >> y) | (digits[i + x + 1] & mask);
    res.back() = digits.back() >> y;
    if (res.back())
        res.pop_back();
    return BigInt(std::move(res), sign);
}

BigInt BigInt::operator<<(const unsigned int bits) const
{
    throw std::runtime_error("not implement function...");
    return BigInt();
}

BigInt
BigInt::modPow(const BigInt &exp, const BigInt &mod) const
{
    BigInt x = *this;
    BigInt res = BigInt(1);
    BIT tmp;
    int n = exp.digits.size();
    for (int i = 0; i < n - 1; ++i)
    {
        BIT tmp = exp.digits[i];
        for (int j = 0; j < BITL; ++j)
        {
            if (tmp & 1)
                res = res * x % mod;
            x = x * x % mod;
        }
    }
    tmp = exp.digits.back();
    while (true)
    {
        if (tmp & 1)
            res = res * x % mod;
        tmp >>= 1;
        if (tmp == 0)
            break;
        x = x * x % mod;
    }
    return res;
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

void BigInt::debug(const std::vector<BIT> &vec)
{
    fprintf(stderr, "%d %zu\n", 0, vec.size());
    for (auto x : vec)
        fprintf(stderr, "%#llX ", x);
    fprintf(stderr, "\n");
}