#ifndef ARGSPARSER_H
#define ARGSPARSER_H

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

class ArgsParser
{
private:
    std::map<std::string, std::string> arguments;
    std::vector<std::string> positional_args;
    std::vector<std::string> valid_modes;
    std::string mode;

    void validate_mode(const std::string &mode);

public:
    ArgsParser();

    void add_mode(const std::string &mode);
    void parse(const int argc, const char *argv[]);

    std::string get_mode() const;
    std::string get_string(const std::string &key, const std::string &default_value = "") const;
    int get_int(const std::string &key, int default_value = 0) const;
    bool has_key(const std::string &key) const;

    void print_help(const std::string &program_name) const;
};

#endif // ARGSPARSER_H