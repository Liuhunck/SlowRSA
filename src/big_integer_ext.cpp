#include <rsa/big_integer.h>
#include <cassert>
#include <cmath>
#include <iostream>

static BigInt
modBarrett(const BigInt &x, const BigInt &mod, const BigInt inv, int k)
{
    BigInt res = x - ((x * inv) >> (2 * k * BITL)) * mod;
    if (res > mod)
        return res - mod;
    return res;
}

BigInt
BigInt::modPowBarrett(const BigInt &exp, const BigInt &mod) const
{
    int k = std::max(digits.size(), mod.digits.size()) + 1;
    std::vector<BIT> mu(2 * k + 1, 0);
    mu.back() = 1;
    BigInt inv = BigInt(std::move(mu), false) / mod;

    BigInt x = *this;
    BigInt res = BigInt(1);
    BIT tmp;
    BigInt tmpb;
    int n = exp.digits.size();
    for (int i = 0; i < n - 1; ++i)
    {
        BIT tmp = exp.digits[i];
        for (int j = 0; j < BITL; ++j)
        {
            if (tmp & 1)
                res = modBarrett(res * x, mod, inv, k);
            x = modBarrett(x * x, mod, inv, k);
            tmp >>= 1;
        }
    }
    tmp = exp.digits.back();
    while (true)
    {
        if (tmp & 1)
            res = modBarrett(res * x, mod, inv, k);
        tmp >>= 1;
        if (tmp == 0)
            break;
        x = modBarrett(x * x, mod, inv, k);
    }
    return res;
}