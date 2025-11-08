#include <rsa/algorithm.h>
#include <iostream>

namespace BNAlgo
{
    std::tuple<bool, BigInt, BigInt> inv_exgcd(const BigInt &a, const BigInt &b)
    {
        if (b == 0)
            return std::make_tuple(a == 1, BigInt(1), BigInt());

        auto [div, mod] = a.divAndMod(b);
        auto [ret, x, y] = inv_exgcd(b, mod);
        if (ret == false)
            return std::make_tuple(false, std::move(x), std::move(y));

        BigInt xy = x - div * y;
        return std::make_tuple(true, std::move(y), std::move(xy));
    }

    BigInt inv(const BigInt &a, const BigInt &n, bool is_prime)
    {
        if (is_prime)
            return a.modPow(n - 2, n);
        auto [ret, _, x] = inv_exgcd(n, a);
        if (ret == false)
            return BigInt(-1);
        x = x % n;
        if (x < 0)
            return x + n;
        return x;
    }

    BigInt hash(const BigInt &x)
    {
        return x;
    }
}