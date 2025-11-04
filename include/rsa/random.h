#ifndef RSA_RANDOM_H
#define RSA_RANDOM_H

#include "big_integer.h"

namespace BNRandom
{
    void initRandom(int seed = 0);
    inline unsigned char getRandByte();
    BigInt getRandInt(int bits);
    BigInt getRandPrime(int bits);
}

#endif