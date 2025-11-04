#include <rsa/random.h>

#include <random>
#include <cassert>
#include <algorithm>

namespace BNRandom
{
#define NUMPRIMES 2048

    std::mt19937 g;

    void initRandom(int seed)
    {
        if (seed != 0)
            return g.seed(seed);

        std::random_device rd;
        uint32_t numbers[624];
        std::generate(numbers, std::end(numbers), std::ref(rd));
        std::seed_seq sseq(numbers, std::end(numbers));
        g.seed(sseq);
    }

    inline unsigned char
    getRandByte()
    {
        return g() & 0xFF;
    }

    BigInt
    getRandInt(int bits)
    {
        int n = bits / BITL;
        int m = bits % BITL;
        std::vector<BIT> res(n);
        for (int i = 0; i < n; ++i)
        {
            BIT tmp = getRandByte();
            for (int j = 1; j < (BITL / 8); ++j)
                tmp = (tmp << 8) | getRandByte();
            res[i] = tmp;
        }
        if (m != 0)
        {
            int x = m / 8, y = m % 8, mask = 1 << y;
            BIT tmp = getRandByte() & (mask - 1) | (mask / 2);
            for (int j = 0; j < x; ++j)
                tmp = (tmp << 8) | getRandByte();
        }
        return BigInt(std::move(res), 0);
    }

    BigInt
    getRandPrime(int bits)
    {
        assert(bits > 16);
        while (true)
        {
            BigInt tmp = getRandInt(bits);
            tmp |= 0x3;

            if (isPrime(tmp, bits) && isPrime(tmp >> 1, bits - 1))
                return tmp;
        }
    }

    // Miller-Rabin 测试最小的轮数，参考 openssl 的实现标准
    static inline int
    getMinMRChecks(int bits)
    {
        if (bits > 2048)
            return 128;
        return 64;
    }

    // 试除的质数个数
    static inline int
    getTrialDivision(int bits)
    {
        if (bits <= 512)
            return 64;
        else if (bits <= 1024)
            return 128;
        else if (bits <= 2048)
            return 384;
        else if (bits <= 4096)
            return 1024;
        return NUMPRIMES;
    }

    static bool
    isPrime(BigInt w, int bits)
    {
        int tdiv = getTrialDivision(bits);
        for (int i = 1; i < tdiv; ++i)
            if (w % i == 0)
                return false;

        BigInt w1 = w - 1;
        unsigned int a = w1.ctz();
        BigInt m = w1 >> a;

        int iter = getMinMRChecks(bits);

        while (iter--)
        {
            BigInt b = getRandInt(bits);
            if (b > w1)
                b = b - w;
            if (b < 3)
            {
                iter++;
                continue;
            }
            BigInt z = b.modPow(m, w);
            if (z == 1 || z == w1)
                continue;

            for (int j = 1; j < a; ++j)
            {
                z = z * z % w;
                if (z == w1)
                    goto end;
                if (z == 1)
                    return false;
            }
            z = z * z % w;
            if (z == 1)
                return false;
        end:
        }
        return true;
    }
}
