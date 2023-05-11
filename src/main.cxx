#include <fts/config.hpp>
#include <fts/text_index_accessor.hpp>
#include <fts/bin_index_accessor.hpp>
#include <fts/index_builder.hpp>
#include <fts/text_index_writer.hpp>
#include <fts/bin_index_writer.hpp>
#include <fts/searcher.hpp>
#include <fts/replxx_wrapper.hpp>
#include <cxxopts.hpp>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <vector>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory>

static void check_index_dirs(const cxxopts::ParseResult& parse_cmd_line)
{
    const std::string index_path = parse_cmd_line["index_path"].as<std::string>();

    const std::filesystem::path index{index_path};

    std::vector<std::filesystem::path> directories{index};

    if (!parse_cmd_line.count("bin"))
    {
        const std::filesystem::path index_docs{index_path + "/docs"};
        const std::filesystem::path index_entries{index_path + "/entries"};

        directories.push_back(index_docs);
        directories.push_back(index_entries);
    }

    for (const auto& dir : directories)
    {
        if (!std::filesystem::exists(dir))
        {
            if (parse_cmd_line.count("indexer"))
            {
                std::filesystem::create_directory(dir);
            }
            if (parse_cmd_line.count("searcher") && !parse_cmd_line.count("indexer"))
            {
                throw std::runtime_error{"Selected directory doesn't exist"};
            }
        }
    }
}

static void build_index(const cxxopts::ParseResult& parse_cmd_line)
{
    const std::string index_path = parse_cmd_line["index_path"].as<std::string>();
    const std::string conf_filename = parse_cmd_line["config_file"].as<std::string>();
    const fts::ConfOptions config = fts::parse_config(conf_filename);

    fts::IndexBuilder indexes(config);

    const std::string csv_filename = parse_cmd_line["csv"].as<std::string>();
    indexes.parse_csv(csv_filename);

    std::shared_ptr<fts::IndexWriter_I> writer = nullptr;

    if (parse_cmd_line.count("bin"))
    {
        writer = std::make_shared<fts::BinIndexWriter>(index_path, config);
    }
    else
    {
        writer = std::make_shared<fts::TextIndexWriter>(index_path, config);
    }

    writer->write(indexes.get_index());
}

static void search(const cxxopts::ParseResult& parse_cmd_line)
{
    const std::string index_path = parse_cmd_line["index_path"].as<std::string>();

    std::string query;

    if (!parse_cmd_line.count("query"))
    {
        query = replxx::wrapper::getline();
    }
    else
    {
        query = parse_cmd_line["query"].as<std::string>();
    }

    std::shared_ptr<fts::IndexAccessor_I> index_accessor = nullptr;
    char* index_data = nullptr;
    std::size_t index_file_size = 0;

    if (parse_cmd_line.count("bin"))
    {
        const std::string index_filename = index_path + "/index.txt";
        int fd = open(index_filename.c_str(), O_RDONLY | O_CLOEXEC);
        index_file_size = std::filesystem::file_size(index_filename);
        index_data = static_cast<char*>(mmap(nullptr, index_file_size, PROT_READ, MAP_PRIVATE, fd, 0));

        index_accessor = std::make_shared<fts::BinIndexAccessor>(index_path, index_data);

        close(fd);
    }
    else
    {
        index_accessor = std::make_shared<fts::TextIndexAccessor>(index_path);
    }

    fts::Searcher searcher(*index_accessor);

    const fts::SearchInfo result_of_search = searcher.score_calc(query);
    searcher.print_scores(result_of_search);

    if (parse_cmd_line.count("bin"))
    {
        munmap(index_data, index_file_size);
    }
}

int main(int argc, char** argv)
{
    cxxopts::Options options("TextSearcher");

    // clang-format off
    options.add_options()
        ("indexer", "Indexer call")
        ("searcher", "Searcher call")
        ("bin", "Binary format")
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

        check_index_dirs(parse_cmd_line);

        if (parse_cmd_line.count("indexer"))
        {
            build_index(parse_cmd_line);
        }

        if (parse_cmd_line.count("searcher"))
        {
            search(parse_cmd_line);
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
