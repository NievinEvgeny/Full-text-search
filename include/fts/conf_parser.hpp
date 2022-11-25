#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_set>

namespace fts {

struct ConfOptions
{
    const std::unordered_set<std::string> stop_words;
    int ngram_min_len = 0;
    int ngram_max_len = 0;
};

nlohmann::json parse_config(const std::string& conf_filename);

fts::ConfOptions parse_json_struct(const nlohmann::json& parsed_conf);

void copy_config(const nlohmann::json& parsed_conf, const std::string& index_path);

}  // namespace fts