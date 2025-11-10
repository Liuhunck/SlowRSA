#ifndef RSA_UTILS_H
#define RSA_UTILS_H

#include <rsa/rsa_core.h>
#include <string>

class BNUtils : public BigInt
{
private:
    int sz;
    int length;
    int lenlength;
    void init();
    void clear();

public:
    BNUtils(int sz, int length, int lenlength) : sz(sz), length(length), lenlength(lenlength) { init(); };
    BNUtils &operator=(BigInt &&x)
    {
        BigInt::operator=(std::move(x));
        return *this;
    }

    void encode(const char *src, int len);
    std::string decode();
};

#define GET_INPUT_STREAM(in, input_path)                                         \
    std::ifstream _input_file;                                                   \
    if (!(input_path).empty())                                                   \
    {                                                                            \
        _input_file.open(input_path);                                            \
        if (!_input_file)                                                        \
            throw std::runtime_error("Cannot open input file: " + (input_path)); \
    }                                                                            \
    std::istream &in = _input_file.is_open() ? _input_file : std::cin

#define CLOSE_INPUT_STREAM() \
    if (_input_file)         \
    _input_file.close()

#define GET_OUTPUT_STREAM(out, output_path)                                        \
    std::ofstream _output_file;                                                    \
    if (!(output_path).empty())                                                    \
    {                                                                              \
        _output_file.open(output_path);                                            \
        if (!_input_file)                                                          \
            throw std::runtime_error("Cannot open output file: " + (output_path)); \
    }                                                                              \
    std::ostream &out = _output_file.is_open() ? _output_file : std::cout

#define CLOSE_OUTPUT_STREAM() \
    if (_output_file)         \
    _output_file.close()

void split_and_encrypt(const std::string &input_path, const std::string &output_path, const RSAPublicKey &pk);
void decrypt_and_join(const std::string &input_path, const std::string &output_path, const RSAPrivateKey &pk);

#endif