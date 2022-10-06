#pragma once
#include <string>
#include <vector>

namespace fts {

struct ngram
{
    int index;
    std::string word;
};

struct conf_options
{
    const std::string text;
    const std::vector<std::string> stop_words;
    int ngram_min_length = 0;
    int ngram_max_length = 0;
};

void char_to_lower_case(std::string& text);

void remove_punctuation(std::string& text);

int parse_config(const std::string& config_filename, struct conf_options& conf_options);

std::vector<std::string> string_tokenization(std::string& text);

void delete_stop_words(std::vector<std::string>& text_tokens, const std::vector<std::string>& stop_words);

std::vector<ngram> ngram_generation(std::vector<std::string>& text_tokens, int ngram_min_length, int ngram_max_length);

void run_parser(const std::string& filename);

}  // namespace fts