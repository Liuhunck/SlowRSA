#ifndef RSA_RANDOM_H
#define RSA_RANDOM_H

#include "big_integer.h"

namespace BNRandom
{
    void initRandom(int seed = 0);

    unsigned char getRandByte();
    BIT getRandWord();

    BigInt getRandInt(int bits, bool keep = true);
    BigInt getRandPrime(int bits);
}

#endif