#include <fts/fts.hpp>
#include <cxxopts.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    cxxopts::Options options("TextSearcher");

    // clang-format off
    options.add_options()
    ("config,config_file", "Config file name", cxxopts::value<std::string>())
    ("text,text_to_search", "Text to search", cxxopts::value<std::string>());
    // clang-format on

    const auto parse_cmd_line = options.parse(argc, argv);

    if ((parse_cmd_line.count("config") != 1) || (parse_cmd_line.count("text") != 1))
    {
        std::cout << options.help() << "\n";
        return -1;
    }

    const auto config_filename = parse_cmd_line["config"].as<std::string>();
    const auto text = parse_cmd_line["text"].as<std::string>();

    try
    {
        fts::run_parser(config_filename, text);
    }
    catch (fts::parse_exception& msg)
    {
        std::cout << msg.err() << '\n';
        return -1;
    }

    return 0;
}