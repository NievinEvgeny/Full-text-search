#pragma once
#include <string>
#include <vector>
#include <stdexcept>

namespace fts {

class parse_exception : public std::runtime_error
{
   private:
    std::string error;

   public:
    explicit parse_exception(const std::string& msg) : std::runtime_error{msg}, error{msg}
    {
    }
    std::string err()
    {
        return error;
    }
};

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