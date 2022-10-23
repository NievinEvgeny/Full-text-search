#include <fts/parser.hpp>
#include <fts/indexer.hpp>
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

    const auto conf_filename = parse_cmd_line["config"].as<std::string>();

    try
    {
        fts::ConfOptions conf_options = fts::parse_config(conf_filename);

        fts::IndexBuilder indexes;
        indexes.add_document(103975, "The Matrix Matrix Matrix Reloaded Revolution", conf_options);  // delete
        indexes.add_document(238695, "The Matrix Revolution", conf_options);  // delete
        indexes.add_document(390473, "The Matrix", conf_options);  // delete

        fts::TextIndexWriter index_writer("index");
        index_writer.write(indexes);
    }
    catch (const fts::ParseException& msg)
    {
        std::cout << msg.what() << '\n';
        return -1;
    }
    catch (const fts::WriteIndexException& msg)
    {
        std::cout << msg.what() << '\n';
        return -1;
    }

    return 0;
}