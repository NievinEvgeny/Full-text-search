#include <fts/parser.hpp>
#include <fts/indexer.hpp>
#include <fts/searcher.hpp>
#include <nlohmann/json.hpp>
#include <cxxopts.hpp>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <vector>

static void check_index_directories(const std::string& index_path, size_t indexer, size_t searcher)
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

int main(int argc, char** argv)
{
    cxxopts::Options options("TextSearcher");

    // clang-format off
    options.add_options()
        ("indexer", "Indexer call")
        ("searcher", "Searcher call")
        ("i,index_path", "Path for index", cxxopts::value<std::string>()->default_value("index"))
        ("c,csv", "Path for *.csv file", cxxopts::value<std::string>()->default_value("books.csv"))
        ("q,query", "Query to search", cxxopts::value<std::string>())
        ("config,config_file", "Config file name (only for indexer)", cxxopts::value<std::string>()->default_value("RunOptions.json"))
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
            const fts::ConfOptions config = fts::parse_json_struct(parsed_conf);
            fts::copy_config(parsed_conf, index_path);

            fts::IndexBuilder indexes;

            const std::string csv_filename = parse_cmd_line["csv"].as<std::string>();
            indexes.parse_csv(csv_filename, config);

            fts::TextIndexWriter index_writer(index_path);
            index_writer.write(indexes.get_index());
        }

        if (parse_cmd_line.count("searcher"))
        {
            const nlohmann::json parsed_conf = fts::parse_config(index_path + "/Config.json");
            const fts::ConfOptions config = fts::parse_json_struct(parsed_conf);

            const std::string query = parse_cmd_line["query"].as<std::string>();
            const std::vector<fts::Ngram> ngrams = fts::parse_query(config, query);

            fts::IndexAccessor index_accessor(index_path, ngrams);

            const std::vector<fts::DocScore> doc_scores = index_accessor.get_scores();
            index_accessor.print_scores();
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
