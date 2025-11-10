#include <rsa/utils.h>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>

void BNUtils::init()
{
    assert(sz * BITL >= (length + lenlength) * 8);
    digits.resize(sz);
}

void BNUtils::clear()
{
    while (!digits.empty() && digits.back() == 0)
        digits.pop_back();
}

void BNUtils::encode(const char *src, int len)
{
    assert(len <= length);
    init();
    char *dd = (char *)digits.data();
    memcpy(dd, src, len);
    for (int j = 0; j <= lenlength; ++j, len >>= 8)
        *(dd + length + j) = len & 0xFF;
    clear();
}

std::string
BNUtils::decode()
{
    init();
    // if (digits.size() * BITL < (length + lenlength) * 8UL)
    //     throw std::runtime_error("Can not decode...");
    char *dd = (char *)digits.data();
    int len = 0;
    for (int j = lenlength; j >= 0; --j)
        len = len << 8 | *(dd + length + j);
    assert(len <= length);
    return std::string(dd, len);
}

static inline int
get_code_len(int bits)
{
    --bits;
    int n = bits / 8;
    int m = bits % 8;
    if ((1 << m) >= n)
        return n;
    if ((1 << (m + 8)) >= n - 1)
        return n - 1;
    if ((1 << (m + 16)) >= n - 2)
        return n - 2;
    throw std::invalid_argument("Can not get code len...");
}

static inline int
get_code_len_len(int bits)
{
    --bits;
    int n = bits / 8;
    int m = bits % 8;
    if ((1 << m) >= n)
        return 0;
    if ((1 << (m + 8)) >= n - 1)
        return 1;
    if ((1 << (m + 16)) >= n - 2)
        return 2;
    throw std::invalid_argument("Can not get code len len...");
}

static std::string
read_all_from_stream(std::istream &input)
{
    std::string data;
    std::string line;

    while (std::getline(input, line))
        data += line + "\n";

    return data;
}

// 二进制读取版本
// static std::string
// read_binary_from_stream(std::istream &input)
// {
//     // 移动到流末尾获取大小
//     input.seekg(0, std::ios::end);
//     std::streamsize size = input.tellg();
//     input.seekg(0, std::ios::beg);

//     std::string data(size, '\0');
//     input.read(&data[0], size);

//     return data;
// }

// static void
// write_binary_to_stream(std::ostream &output, const std::string &data)
// {
//     output.write(data.data(), data.size());
//     output.flush();
// }

void split_and_encrypt(const std::string &input_path, const std::string &output_path, const RSAPublicKey &pk)
{
    GET_INPUT_STREAM(in, input_path);
    GET_OUTPUT_STREAM(out, output_path);

    std::string content = read_all_from_stream(in);

    int bits = pk.bits();
    int len = get_code_len(bits);
    int len_len = get_code_len_len(bits);

    int n = content.length() / len;
    int m = content.length() % len;

    int sz = (bits + BITL - 1) / BITL;
    BNUtils bt(sz, len, len_len);

    for (int i = 0; i < n; ++i)
    {
        bt.encode(content.data() + len * i, len);
        assert(bt.bits() < pk.bits());
        out << pk.encrypt(bt).toString() << std::endl;
    }
    if (m != 0)
    {
        bt.encode(content.data() + len * n, m);
        assert(bt.bits() < pk.bits());
        out << pk.encrypt(bt).toString() << std::endl;
    }

    CLOSE_INPUT_STREAM();
    CLOSE_OUTPUT_STREAM();
}

void decrypt_and_join(const std::string &input_path, const std::string &output_path, const RSAPrivateKey &pk)
{
    GET_INPUT_STREAM(in, input_path);
    GET_OUTPUT_STREAM(out, output_path);

    int bits = pk.bits();
    int len = get_code_len(bits);
    int len_len = get_code_len_len(bits);

    int sz = (bits + BITL - 1) / BITL;
    BNUtils bt(sz, len, len_len);

    std::string line;
    while (std::getline(in, line))
    {
        bt = pk.decrypt(BigInt(line));
        out << bt.decode();
    }

    CLOSE_INPUT_STREAM();
    CLOSE_OUTPUT_STREAM();
}