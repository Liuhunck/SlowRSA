#include <rsa/rsa_core.h>
#include <iostream>
#include <fstream>
#include <cassert>

RSAPrivateKey::RSAPrivateKey(int bits)
{
    if (bits < 34)
        throw std::runtime_error("unsupport bits less than 34...");

    int bits1 = bits >> 1;
    int bits2 = bits - bits1;

    p = BNRandom::getRandPrime(bits1);
    do
    {
        q = BNRandom::getRandPrime(bits2);
    } while ((p - q).bits() < bits1 - 3);
    n = p * q;

    BigInt p1 = p - 1;
    BigInt q1 = q - 1;
    BigInt n1 = n - p - q + 1;

    do
    {
        e = BNRandom::getRandInt(n1.bits(), false);
        e |= 1;
        if (e > n1)
            e = e - n1;
        if (e.bits() < bits1 - 3)
            continue;
        d = BNAlgo::inv(e, n1);
#if not defined(NDEBUG)
        std::cerr << "try ed..." << std::endl;
#endif
        if (d < 0 || d.bits() < bits1 - 3)
            continue;
        assert((e * d) % n1 == 1);
        break;
    } while (true);

    ep = e % p1;
    eq = e % q1;
    dp = d % p1;
    dq = d % q1;

    nm = q * BNAlgo::inv(q, p, true);
}

static std::string line;

static BigInt
read_big_int(std::ifstream &f)
{
    if (!std::getline(f, line))
        throw std::runtime_error("can not read private key file correctly...");
    return BigInt(line);
}

RSAPrivateKey::RSAPrivateKey(const std::string &file)
{
    std::ifstream f(file);
    if (f.is_open() == false)
        throw std::runtime_error("can not open private key file...");
    n = read_big_int(f);
    p = read_big_int(f);
    q = read_big_int(f);
    e = read_big_int(f);
    d = read_big_int(f);
    ep = read_big_int(f);
    eq = read_big_int(f);
    dp = read_big_int(f);
    dq = read_big_int(f);
    nm = read_big_int(f);
    f.close();
}

void RSAPrivateKey::genKey(const std::string &file) const
{
    std::ofstream f(file);
    if (f.is_open() == false)
        throw std::runtime_error("can not open private key file...");

    f << n.toString() << std::endl;
    f << p.toString() << std::endl;
    f << q.toString() << std::endl;
    f << e.toString() << std::endl;
    f << d.toString() << std::endl;
    f << ep.toString() << std::endl;
    f << eq.toString() << std::endl;
    f << dp.toString() << std::endl;
    f << dq.toString() << std::endl;
    f << nm.toString() << std::endl;

    if (f.fail())
        throw std::runtime_error("can not write private key...");
    f.close();
}

void RSAPrivateKey::genPubKey(const std::string &file) const
{
    std::ofstream f(file);
    if (f.is_open() == false)
        throw std::runtime_error("can not open public key file...");

    f << n.toString() << std::endl;
    f << e.toString() << std::endl;

    if (f.fail())
        throw std::runtime_error("can not write public key...");
    f.close();
}

BigInt
RSAPrivateKey::encrypt(const BigInt &x) const
{
    throw std::runtime_error("should not use...");
    BigInt xp = (x % p).modPow(ep, p);
    BigInt xq = (x % q).modPow(eq, q);
    BigInt res = ((xp - xq) * nm + xq) % n;
    if (res < 0)
        return res + n;
    return res;
}

BigInt
RSAPrivateKey::decrypt(const BigInt &x, bool use_crt) const
{
    if (use_crt == false)
        return x.modPow(d, n);
    BigInt xp = (x % p).modPow(dp, p);
    BigInt xq = (x % q).modPow(dq, q);
    BigInt res = ((xp - xq) * nm + xq) % n;
    if (res < 0)
        return res + n;
    return res;
}

BigInt
RSAPrivateKey::sign(const BigInt &x) const
{
    return decrypt(BNAlgo::hash(x));
}

RSAPublicKey::RSAPublicKey(const std::string &file)
{
    std::ifstream f(file);
    if (f.is_open() == false)
        throw std::runtime_error("can not open public key file...");

    n = read_big_int(f);
    e = read_big_int(f);
    f.close();
}

BigInt
RSAPublicKey::encrypt(const BigInt &x) const
{
    return x.modPow(e, n);
}

bool RSAPublicKey::verify(const BigInt &x, const BigInt &sign) const
{
    return encrypt(sign) == BNAlgo::hash(x);
}
