#ifndef RSA_CORE_H
#define RSA_CORE_H

#include "big_integer.h"
#include "random.h"

class RSAPrivateKey
{
    BigInt *p;
    BigInt *q;
    BigInt *d;
};

class RSAPublicKey
{
    BigInt *n;
    BigInt *e;

public:
    RSAPublicKey(BigInt *n, BigInt *e) : n(n), e(e) {}
};

class RSAContext
{
private:
    int bits;
    BigInt n;
    BigInt p;
    BigInt q;
    BigInt n1;
    BigInt p1;
    BigInt q1;
    BigInt e;
    BigInt d;

public:
    RSAContext(int bits) : bits(bits) { _init(); }

    RSAPublicKey pubKey()
    {
        return RSAPublicKey(&n, &e);
    }

private:
    void _init();
};

#endif