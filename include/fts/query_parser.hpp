#pragma once
#include <fts/conf_parser.hpp>
#include <vector>
#include <string>
#include <unordered_set>

namespace fts {

struct Ngram
{
    int index;
    std::string word;
};

std::vector<std::string> string_tokenize(const std::string& text);

void delete_stop_words(std::vector<std::string>& text_tokens, const std::unordered_set<std::string>& stop_words);

std::vector<Ngram> ngram_generate(const std::vector<std::string>& text_tokens, int ngram_min_len, int ngram_max_len);

std::vector<Ngram> parse_query(const fts::ConfOptions& config, const std::string& text);

}  // namespace fts