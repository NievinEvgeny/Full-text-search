#include <fts/parser.hpp>
#include <fts/indexer.hpp>
#include <fts/searcher.hpp>
#include <nlohmann/json.hpp>
#include <cxxopts.hpp>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <vector>

namespace {

void check_index_directories(const std::string& index_path, size_t indexer, size_t searcher)
{
    const std::filesystem::path index(index_path);
    const std::filesystem::path index_docs(index_path + "/docs");
    const std::filesystem::path index_entries(index_path + "/entries");

    const std::vector<std::filesystem::path> directories = {index, index_docs, index_entries};

    for (const auto& dir : directories)
    {
        if (!std::filesystem::exists(dir))
        {
            if (indexer)
            {
                std::filesystem::create_directory(dir);
            }
            if (searcher && (indexer == 0))
            {
                throw std::runtime_error{"Selected directory doesn't exist"};
            }
        }
    }
}

}  // namespace

int main(int argc, char** argv)
{
    cxxopts::Options options("TextSearcher");

    // clang-format off
    options.add_options()
        ("indexer", "Indexer call")
        ("searcher", "Searcher call")
        ("i,index_path", "Path for index", cxxopts::value<std::string>()->default_value("index"))
        ("q,query", "Query to search", cxxopts::value<std::string>())
        ("c,config_file", "Config file name (only for indexer)", cxxopts::value<std::string>()->default_value("RunOptions.json"))
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

        const std::string index_path = parse_cmd_line["index_path"].as<std::string>();

        check_index_directories(index_path, parse_cmd_line.count("indexer"), parse_cmd_line.count("searcher"));

        if (parse_cmd_line.count("indexer"))
        {
            const std::string conf_filename = parse_cmd_line["config_file"].as<std::string>();
            const nlohmann::json parsed_conf = fts::parse_config(conf_filename);
            const fts::ConfOptions conf_options = fts::parse_json_struct(parsed_conf);
            fts::copy_config(parsed_conf, index_path);

            fts::IndexBuilder indexes;
            indexes.add_document(103975, "The Matrix Matrix Matrix Reloaded Revolution", conf_options);  // delete
            indexes.add_document(238695, "The Matrix Revolution", conf_options);  // delete
            indexes.add_document(390473, "The Matrix", conf_options);  // delete

            fts::TextIndexWriter index_writer(index_path);
            index_writer.write(indexes.get_index());
        }

        if (parse_cmd_line.count("searcher"))
        {
            const std::string query = parse_cmd_line["query"].as<std::string>();

            fts::SearcherBuf searcher_buf;
            searcher_buf.deserialize_index(query, index_path);
            searcher_buf.count_docs(index_path);
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