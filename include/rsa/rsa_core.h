#ifndef RSA_CORE_H
#define RSA_CORE_H

#include "big_integer.h"
#include "algorithm.h"
#include "random.h"
#include <memory>

class RSAPrivateKey
{
private:
    BigInt n;
    BigInt p;
    BigInt q;
    BigInt e;
    BigInt d;
    BigInt ep;
    BigInt eq;
    BigInt dp;
    BigInt dq;
    BigInt nm;

public:
    RSAPrivateKey() = delete;
    RSAPrivateKey(int bits);
    RSAPrivateKey(const std::string &file);

    int bits() const { return n.bits(); }

    void genKey(const std::string &file) const;
    void genPubKey(const std::string &file) const;

    BigInt encrypt(const BigInt &x) const;
    BigInt decrypt(const BigInt &x, bool use_crt = true) const;
    BigInt sign(const BigInt &x) const;
};

class RSAPublicKey
{
private:
    BigInt n;
    BigInt e;

public:
    RSAPublicKey() = delete;
    RSAPublicKey(const BigInt &n, const BigInt &e) : n(n), e(e) {}
    RSAPublicKey(BigInt &&n, BigInt &&e) : n(n), e(e) {}
    RSAPublicKey(const std::string &file);

    int bits() const { return n.bits(); }

    BigInt encrypt(const BigInt &x) const;
    bool verify(const BigInt &x, const BigInt &sign) const;
};

#endif