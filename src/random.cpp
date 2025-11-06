#include <rsa/random.h>

#include <random>
#include <cassert>
#include <iostream>
#include <algorithm>

namespace BNRandom
{
    static bool isPrime(BigInt w);

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

    unsigned char
    getRandByte()
    {
        return g() & 0xFF;
    }

    BIT
    getRandWord()
    {
#if defined(BIT_USE_64)
        return (static_cast<BIT>(g()) << 32) | g();
#else
        return g();
#endif
    }

    BigInt
    getRandInt(int bits, bool keep)
    {
        int n = bits / BITL;
        int m = bits % BITL;
        std::vector<BIT> res(n);
        for (int i = 0; i < n; ++i)
            res[i] = getRandWord();
        if (m != 0)
        {
            BIT max = static_cast<BIT>(1) << m, mask = max - 1;
            BIT tmp = getRandWord() & mask;
            if (keep)
                tmp |= (max >> 1);
            res.push_back(tmp);
        }
        else
        {
            if (keep)
                res.back() |= static_cast<BIT>(1) << (BITL - 1);
        }
        return BigInt(std::move(res), 0);
    }

    BigInt
    getRandPrime(int bits, bool safe)
    {
        if (bits <= 16)
            throw std::runtime_error("prime bits should greater than 16...");

        BigInt tmp = getRandInt(bits);
        tmp |= safe ? 0x3 : 0x1;
        int delta = safe ? 4 : 2;

        int try_max_time = 10000000;
        for (int round = 0; round < try_max_time; ++round)
        {
            if (isPrime(tmp))
            {
                if (safe && !isPrime(tmp >> 1))
                    continue;
#if not defined(NDEBUG)
                std::cerr << "tried: " << round + 1 << std::endl;
#endif
                return tmp;
            }
            tmp += delta;
        }
        assert(0 && "get prime over the limit rounds...");
        return BigInt();
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

    // 质数判定，Miller-Rabin
    static bool
    isPrime(BigInt w)
    {
        int bits = w.bits();
        int tdiv = getTrialDivision(bits);
        for (int i = 1; i < tdiv; ++i)
            if (w % primes[i] == 0)
                return false;

        BigInt w1 = w - 1;
        int a = w1.ctz();
        assert(a >= 1);
        BigInt m = w1 >> a;

        int iter = getMinMRChecks(bits);
        while (iter)
        {
            BigInt b = getRandInt(bits, false);
            if (b > w1)
                b = b - w;
            if (b < 3)
                continue;

            BigInt z = b.modPow(m, w);
            if (z == 1 || z == w1)
                goto loop_cont;

            for (int j = 1; j < a; ++j)
            {
                z = z * z % w;
                if (z == w1)
                    goto loop_cont;
                if (z == 1)
                    return false;
            }

            return false;

        loop_cont:
            --iter;
        }
        return true;
    }
}
