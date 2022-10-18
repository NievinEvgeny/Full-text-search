#include <fts/parser.hpp>
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

    const auto conf_filename = parse_cmd_line["config"].as<std::string>();
    const auto text = parse_cmd_line["text"].as<std::string>();

    try
    {
        fts::ConfOptions conf_options = fts::parse_config(conf_filename);
        std::vector<fts::Ngram> ngrams = fts::parse_query(conf_options, text);
        for (auto& ngram : ngrams)
        {
            std::cout << ngram.word << '\n';
        }
    }
    catch (fts::ParseException& msg)
    {
        std::cout << msg.what() << '\n';
        return -1;
    }

    return 0;
}