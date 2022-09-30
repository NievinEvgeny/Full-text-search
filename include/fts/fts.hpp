#pragma once
#include <iostream>
#include <vector>

namespace fts {

void char_to_lower_case(std::string& text);

void remove_puctuation(std::string& text);

int parse_config(
    const std::string& config_filename,
    std::string& text,
    std::vector<std::string>& stop_words,
    int ngram_min_length,
    int ngram_max_length);

std::vector<std::string> string_tokenization(std::string& text);

void delete_stop_words(std::vector<std::string>& text_tokens, std::vector<std::string>& stop_words);

void run_parser(const std::string& filename);

}  // namespace fts