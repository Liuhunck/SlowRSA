#include "big_integer.h"
#include <tuple>

namespace BNAlgo
{
    std::tuple<bool, BigInt, BigInt> inv_exgcd(const BigInt &a, const BigInt &b);

    BigInt inv(const BigInt &a, const BigInt &n, bool is_prime = false);

    BigInt hash(const BigInt &x);
}