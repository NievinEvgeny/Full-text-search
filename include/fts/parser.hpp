#pragma once
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
    int ngram_min_length = 0;
    int ngram_max_length = 0;
};

fts::ConfOptions parse_config(const std::string& conf_filename);

std::vector<std::string> string_tokenization(std::string& text);

void delete_stop_words(std::vector<std::string>& text_tokens, const std::unordered_set<std::string>& stop_words);

std::vector<Ngram> ngram_generation(std::vector<std::string>& text_tokens, int ngram_min_length, int ngram_max_length);

std::vector<Ngram> parse_query(const fts::ConfOptions& conf_options, const std::string& text);

}  // namespace fts