#include <rsa/rsa_core.h>
#include <iostream>

void RSAContext::_init()
{
    if (bits < 34)
        throw std::runtime_error("unsupport bits less than 34...");

    int bits1 = bits >> 1;
    int bits2 = bits - bits1;

    p = BNRandom::getRandPrime(bits1);
    do
    {
        q = BNRandom::getRandPrime(bits2);
    } while ((p - q).bits() < bits1 - 1);
    n = p * q;

    p1 = p - 1;
    q1 = q - 1;
    n1 = n - p - q + 1;

    do
    {
        e = BNRandom::getRandInt(bits, false);
    } while ((e.bits() < bits - 3));

}