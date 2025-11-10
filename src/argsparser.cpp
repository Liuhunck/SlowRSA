#include <rsa/argsparser.h>
#include <algorithm>
#include <cstdlib>

ArgsParser::ArgsParser()
{
    valid_modes = {"gen", "dec", "enc"};
}

void ArgsParser::add_mode(const std::string &mode)
{
    valid_modes.push_back(mode);
}

void ArgsParser::validate_mode(const std::string &mode)
{
    if (std::find(valid_modes.begin(), valid_modes.end(), mode) == valid_modes.end())
    {
        throw std::invalid_argument("Invalid mode: " + mode);
    }
}

void ArgsParser::parse(const int argc, const char *argv[])
{
    if (argc < 2)
    {
        throw std::invalid_argument("No mode specified");
    }

    // 第一个参数是模式
    mode = argv[1];
    validate_mode(mode);

    // 解析剩余参数
    for (int i = 2; i < argc; i++)
    {
        std::string arg = argv[i];

        if (arg.substr(0, 2) == "--")
        {
            // 长参数 --key value
            std::string key = arg.substr(2);

            if (i + 1 >= argc)
            {
                throw std::invalid_argument("Missing value for argument: " + key);
            }

            std::string value = argv[++i];
            arguments[key] = value;
        }
        else if (arg.substr(0, 1) == "-")
        {
            // 短参数 -k value
            std::string key = arg.substr(1);

            if (i + 1 >= argc)
            {
                throw std::invalid_argument("Missing value for argument: " + key);
            }

            std::string value = argv[++i];
            arguments[key] = value;
        }
        else
        {
            // 位置参数
            positional_args.push_back(arg);
        }
    }
}

std::string ArgsParser::get_mode() const
{
    return mode;
}

std::string ArgsParser::get_string(const std::string &key, const std::string &default_value) const
{
    auto it = arguments.find(key);
    if (it != arguments.end())
    {
        return it->second;
    }
    return default_value;
}

int ArgsParser::get_int(const std::string &key, int default_value) const
{
    auto it = arguments.find(key);
    if (it != arguments.end())
    {
        try
        {
            return std::stoi(it->second);
        }
        catch (const std::exception &e)
        {
            throw std::invalid_argument("Invalid integer value for argument: " + key);
        }
    }
    return default_value;
}

bool ArgsParser::has_key(const std::string &key) const
{
    return arguments.find(key) != arguments.end();
}

void ArgsParser::print_help(const std::string &program_name) const
{
    std::cout << "Usage: " << program_name << " <gen|dec|enc> [OPTIONS]\n";
    std::cout << "Modes:\n";
    std::cout << "  gen    Generate keys\n";
    std::cout << "  dec    Decrypt data\n";
    std::cout << "  enc    Encrypt data\n";
    std::cout << "\nOptions:\n";
    std::cout << "  --key PATH      Private key file path\n";
    std::cout << "  --pubkey PATH   Public key file path\n";
    std::cout << "  --in PATH       Input file path\n";
    std::cout << "  --out PATH      Output file path\n";
    std::cout << "  --bits NUM      Key bits (default: 512)\n";
    std::cout << "\nExamples:\n";
    std::cout << "  " << program_name << " gen --pubkey public.key --bits 1024\n";
    std::cout << "  " << program_name << " enc --pubkey public.key --in data.txt --out encrypted.dat\n";
    std::cout << "  " << program_name << " dec --key private.key --in encrypted.dat --out decrypted.txt\n";
}