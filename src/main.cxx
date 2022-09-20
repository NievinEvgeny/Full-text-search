#include <fts/fts.hpp>
#include <cxxopts.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    cxxopts::Options options("sum");

    // clang-format off
    options.add_options()
    ("first,first_number", "any number", cxxopts::value<float>())
    ("second,second_number", "any number", cxxopts::value<float>());
    // clang-format on

    const auto result = options.parse(argc, argv);

    if ((result.count("first") != 1) || (result.count("second") != 1))
    {
        std::cout << options.help() << "\n";
        return 0;
    }

    const auto first_num = result["first"].as<float>();
    const auto second_num = result["second"].as<float>();
    std::cout << fts::sum(first_num, second_num) << "\n";
    return 0;
}