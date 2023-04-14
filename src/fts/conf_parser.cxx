#include <fts/conf_parser.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>
#include <string>

namespace fts {

void to_json(nlohmann::json& j, const fts::ConfOptions& conf)
{
    j = nlohmann::json{
        {"stop_words", conf.stop_words}, {"ngram_min_len", conf.ngram_min_len}, {"ngram_max_len", conf.ngram_max_len}};
}

void from_json(const nlohmann::json& j, fts::ConfOptions& conf)
{
    j.at("stop_words").get_to(conf.stop_words);
    j.at("ngram_min_len").get_to(conf.ngram_min_len);
    j.at("ngram_max_len").get_to(conf.ngram_max_len);
}

void print_config_to_json(std::ofstream& file, const fts::ConfOptions& config)
{
    const nlohmann::json j = config;
    file << std::setw(4) << j << '\n';
}

fts::ConfOptions parse_config(const std::string& conf_filename)
{
    std::ifstream conf_file(conf_filename);

    if (!conf_file.is_open())
    {
        throw std::runtime_error{"Can't open file in parse_config function"};
    }

    nlohmann::json parsed_conf = nlohmann::json::parse(conf_file);

    conf_file.close();

    fts::ConfOptions config = parsed_conf.get<fts::ConfOptions>();

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

void config_serialize(const std::string& index_dir_path, const fts::ConfOptions& config)
{
    std::ofstream conf_copy_file(index_dir_path + "/Config.json");

    if (!conf_copy_file.is_open())
    {
        throw std::runtime_error{"Can't open file in serialize_config function"};
    }

    fts::print_config_to_json(conf_copy_file, config);

    conf_copy_file.close();
}

}  // namespace fts