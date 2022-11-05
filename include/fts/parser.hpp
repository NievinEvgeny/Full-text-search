#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <unordered_set>

namespace fts {

struct Ngram
{
    int index;
    std::string word;
};

struct ConfOptions
{
    const std::unordered_set<std::string> stop_words;
    int ngram_min_len = 0;
    int ngram_max_len = 0;
};

nlohmann::json parse_config(const std::string& conf_filename);

fts::ConfOptions parse_json_struct(const nlohmann::json& parsed_conf);

void copy_config(const nlohmann::json& parsed_conf, const std::string& index_path);

std::vector<std::string> string_tokenization(const std::string& text);

void delete_stop_words(std::vector<std::string>& text_tokens, const std::unordered_set<std::string>& stop_words);

std::vector<Ngram> ngram_generation(const std::vector<std::string>& text_tokens, int ngram_min_len, int ngram_max_len);

std::vector<Ngram> parse_query(const fts::ConfOptions& conf_options, const std::string& text);

}  // namespace fts