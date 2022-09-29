#include <fts/fts.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>

namespace fts {

void parse_config(
    const std::string& config_filename,
    std::string& text,
    std::vector<std::string>& stop_words,
    int ngram_min_length,
    int ngram_max_length)
{
    std::ifstream conf_file(config_filename);
    nlohmann::json parsed_config = nlohmann::json::parse(conf_file);
    text = parsed_config.at("text");
    ngram_min_length = parsed_config.at("ngram_min_length");
    ngram_max_length = parsed_config.at("ngram_max_length");
    stop_words = parsed_config.at("stop_words");
}

void run_parser(const std::string& config_filename)
{
    std::string text;
    std::vector<std::string> stop_words;
    int ngram_min_length;
    int ngram_max_length;

    parse_config(config_filename, text, stop_words, ngram_min_length, ngram_max_length);
}

}  // namespace fts