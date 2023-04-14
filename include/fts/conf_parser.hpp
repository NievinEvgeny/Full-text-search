#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <unordered_set>

namespace fts {

struct ConfOptions
{
    std::unordered_set<std::string> stop_words;
    int ngram_min_len;
    int ngram_max_len;
};

void to_json(nlohmann::json& j, const fts::ConfOptions& conf);

void from_json(const nlohmann::json& j, fts::ConfOptions& conf);

void print_config_to_json(std::ofstream& file, const fts::ConfOptions& config);

fts::ConfOptions parse_config(const std::string& conf_filename);

void config_serialize(const std::string& index_dir_path, const fts::ConfOptions& config);

}  // namespace fts