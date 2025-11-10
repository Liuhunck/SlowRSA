#include <rsa/utils.h>
#include <rsa/rsa_core.h>
#include <rsa/argsparser.h>

#include <iostream>
#include <fstream>

int main(const int argc, const char *argv[])
{
    ArgsParser parser;

    try
    {
        parser.parse(argc, argv);
        std::string mode = parser.get_mode();

        if (mode == "gen")
        {
            std::string key_path = parser.get_string("key", "./rsa.key");
            std::string pubkey_path = parser.get_string("pubkey", "./rsa.pub");
            int bits = parser.get_int("bits", 512);

            std::cout << "Generating keys with " << bits << " bits\n";
            std::cout << "Privaet key: " << key_path << std::endl;
            std::cout << "Public key: " << pubkey_path << std::endl;

            RSAPrivateKey pk = RSAPrivateKey(bits);
            pk.genKey(key_path);
            pk.genPubKey(pubkey_path);
            std::cout << "Generated..." << std::endl;
        }
        else if (mode == "enc")
        {
            std::string pubkey_path = parser.get_string("pubkey");
            std::string input_path = parser.get_string("in");
            std::string output_path = parser.get_string("out");

            if (pubkey_path.empty())
                throw std::invalid_argument("Missing pubkey argument for encryption");
            std::cout << "Using public key: " << pubkey_path << std::endl;

            RSAPublicKey pk(pubkey_path);
            split_and_encrypt(input_path, output_path, pk);
        }
        else if (mode == "dec")
        {
            std::string key_path = parser.get_string("key");
            std::string input_path = parser.get_string("in");
            std::string output_path = parser.get_string("out");

            if (key_path.empty())
                throw std::invalid_argument("Missing key argument for decryption");
            std::cout << "Using private key: " << key_path << std::endl;

            RSAPrivateKey pk(key_path);
            decrypt_and_join(input_path, output_path, pk);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << std::endl;
        parser.print_help(argv[0]);
        return 1;
    }
}
