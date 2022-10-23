#include <fts/parser.hpp>
#include <fts/indexer.hpp>
#include <cxxopts.hpp>
#include <iostream>
#include <stdexcept>

int main(int argc, char** argv)
{
    cxxopts::Options options("TextSearcher");

    options.add_options()("config,config_file", "Config file name", cxxopts::value<std::string>());

    const auto parse_cmd_line = options.parse(argc, argv);

    std::string conf_filename = "RunOptions.json";

    if (parse_cmd_line.count("config") == 1)
    {
        conf_filename = parse_cmd_line["config"].as<std::string>();
    }

    try
    {
        fts::ConfOptions conf_options = fts::parse_config(conf_filename);

        fts::IndexBuilder indexes;
        indexes.add_document(103975, "The Matrix Matrix Matrix Reloaded Revolution", conf_options);  // delete
        indexes.add_document(238695, "The Matrix Revolution", conf_options);  // delete
        indexes.add_document(390473, "The Matrix", conf_options);  // delete

        fts::TextIndexWriter index_writer("index");
        index_writer.write(indexes.get_index());
    }
    catch (const std::runtime_error& msg)
    {
        std::cout << msg.what() << '\n';
        return -1;
    }

    return 0;
}