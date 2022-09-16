#include <cxxopts.hpp>
#include <fts.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    cxxopts::Options options("sum");

    options.add_options()("first,first_number", "any number", cxxopts::value<float>())(
        "second,second_number", "any number", cxxopts::value<float>);

    const auto result = options.parse(argc, argv);

    if ((result.count("first") != 1) || (result.count("second") != 1))
    {
        std::cout << options.help() << "\n";
        return 0;
    }

    const auto first_num = result["first"].as<float>();
    const auto second_num = result["second"].as<float>();
    std::cout << sum(first_num, second_num) << "\n";
    return 0;
}