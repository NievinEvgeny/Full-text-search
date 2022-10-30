#include <fts/parser.hpp>
#include <fts/indexer.hpp>
#include <cxxopts.hpp>
#include <iostream>
#include <stdexcept>

int main(int argc, char** argv)
{
    cxxopts::Options options("TextSearcher");

    // clang-format off
    options.add_options()
        ("i,indexer", "Indexer call")
        ("s,searcher", "Searcher call")
        ("index,index_path", "Path for index", cxxopts::value<std::string>()->default_value("index"))
        ("q,query", "Query to search", cxxopts::value<std::string>())
        ("c,config_file", "Config file name", cxxopts::value<std::string>()->default_value("RunOptions.json"))
        ("h,help", "Print usage");
    // clang-format on

    try
    {
        const auto parse_cmd_line = options.parse(argc, argv);

        if (parse_cmd_line.count("help"))
        {
            std::cout << options.help() << '\n';
            return 0;
        }

        std::string index_path = parse_cmd_line["index_path"].as<std::string>();

        if (parse_cmd_line.count("indexer"))
        {
            std::string conf_filename = parse_cmd_line["config_file"].as<std::string>();

            fts::ConfOptions conf_options = fts::parse_config(conf_filename);

            fts::IndexBuilder indexes;
            indexes.add_document(103975, "The Matrix Matrix Matrix Reloaded Revolution", conf_options);  // delete
            indexes.add_document(238695, "The Matrix Revolution", conf_options);  // delete
            indexes.add_document(390473, "The Matrix", conf_options);  // delete

            fts::TextIndexWriter index_writer(index_path);
            index_writer.write(indexes.get_index());
            // TODO (create folder if index_path isn't default)
            // TODO (add config copy to serialization)
        }

        if (parse_cmd_line.count("searcher"))
        {
            // TODO (implementation of searcher)
        }
    }

    catch (const cxxopts::exceptions::exception& msg)
    {
        std::cerr << msg.what() << '\n';
        return -1;
    }

    catch (const std::exception& msg)
    {
        std::cerr << msg.what() << '\n';
        return -1;
    }

    return 0;
}