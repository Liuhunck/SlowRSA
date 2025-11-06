#include <rsa/algorithm.h>

namespace BNAlgo
{
    bool exgcd(BigInt a, BigInt b, BigInt *x, BigInt *y)
    {
        if (b == 0)
        {
            *x = BigInt(1);
            *y = BigInt();
            return a == 1;
        }

        auto [div, mod] = a.divAndMod(b);

        bool ret = exgcd(b, mod, x, y);
        if (ret == false)
            return false;

        BigInt tmp = *x;
        *x = *y;
        *y = tmp - div * *y;
        return true;
    }
}