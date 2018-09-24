#include <getopt.h>

#include "include/personal.hpp"
namespace my
{
int verbose = 0;

int get_opt_template(int argc, char **argv)
{
    int next_option;
    const std::string short_options{"hi:v"};
    const struct option long_options[] = {
        {"help", 0, nullptr, 'h'},
        {"input", 1, nullptr, 'i'},
        {"verbose", 0, nullptr, 'v'},
        {nullptr, 0, nullptr, 0}};
    const std::string program_name(argv[0]);

    do
    {
        next_option = getopt_long(argc, argv, short_options.c_str(), long_options, nullptr);

        switch (next_option)
        {
        case 'h':
            print_usage(std::cout, 0, program_name);

        case 'i':
            std::cout << optarg << "\n";
            break;

        case 'v':
            verbose = 1;
            break;

        case '?':
            print_usage(std::cerr, 1, program_name);

        case -1:
            break;

        default:
            throw;
        }
    } while (next_option != -1);

    return 0;
}

void print_usage(
    std::ostream &out,
    int exit_code,
    const std::string &program_name)
{
    out << "Usage: " << program_name
        << " options [ ... ]\n"
        << " -h --help          Display this usage information.\n"
        << " -i --input         Input string to echo.\n"
        << " -v --verbose       Print verbose messages.\n";
    exit(exit_code);
}

//------------------------------------------------------------------------------

template <>
std::vector<int> split<int>(
    const std::string &buffer,
    const char delimiter)
{
    std::stringstream ss(buffer);
    std::string word;
    std::vector<int> output;

    while (std::getline(ss, word, delimiter))
    {
        output.push_back(std::stoi(word));
    }

    return output;
}

template <>
std::vector<float> split<float>(
    const std::string &buffer,
    const char delimiter)
{
    std::stringstream ss(buffer);
    std::string word;
    std::vector<float> output;

    while (std::getline(ss, word, delimiter))
    {
        output.push_back(std::stof(word));
    }

    return output;
}

//------------------------------------------------------------------------------
} // namespace my