#include <fts/parser.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>
#include <string>

namespace fts {

nlohmann::json parse_config(const std::string& conf_filename)
{
    std::ifstream conf_file(conf_filename);

    if (!conf_file.is_open())
    {
        throw std::runtime_error{"Can't open file in parse_config function"};
    }

    nlohmann::json parsed_conf = nlohmann::json::parse(conf_file);

    conf_file.close();

    return parsed_conf;
}

fts::ConfOptions parse_json_struct(const nlohmann::json& parsed_conf)
{
    fts::ConfOptions config{
        parsed_conf.at("stop_words"),
        parsed_conf.at("ngram_min_len"),
        parsed_conf.at("ngram_max_len"),
    };

    if (config.ngram_min_len < 1)
    {
        throw std::runtime_error{"Ngram min length is below 1"};
    }

    if (config.ngram_max_len < config.ngram_min_len)
    {
        throw std::runtime_error{"Max length of ngram less than min len"};
    }

    return config;
}

void copy_config(const nlohmann::json& parsed_conf, const std::string& index_path)
{
    std::ofstream conf_copy_file(index_path + "/Config.json");

    if (!conf_copy_file.is_open())
    {
        throw std::runtime_error{"Can't open file in copy_config function"};
    }

    conf_copy_file << std::setw(4) << parsed_conf << '\n';
    conf_copy_file.close();
}

}  // namespace fts