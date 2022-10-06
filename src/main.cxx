#include <fts/fts.hpp>
#include <cxxopts.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    cxxopts::Options options("TextSearcher");

    options.add_options()("config,config_file", "Config file name", cxxopts::value<std::string>());

    const auto parse_cmd_line = options.parse(argc, argv);

    if (parse_cmd_line.count("config") != 1)
    {
        std::cout << options.help() << "\n";
        return -1;
    }

    const auto config_filename = parse_cmd_line["config"].as<std::string>();

    try
    {
        fts::run_parser(config_filename);
    }
    catch (fts::parse_exception& msg)
    {
        std::cout << msg.err() << '\n';
        return -1;
    }

    return 0;
}