#include <fts/fts.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>

namespace fts {

void char_to_lower_case(std::string& text)
{
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return std::tolower(c); });
}

void remove_puctuation(std::string& text)
{
    for (int i = text.size() - 1; i >= 0; i--)
    {
        if (ispunct(text[i]))
        {
            text.erase(i, 1);
        }
    }
}

int parse_config(
    const std::string& config_filename,
    std::string& text,
    std::vector<std::string>& stop_words,
    int ngram_min_length,
    int ngram_max_length)
{
    std::ifstream conf_file(config_filename);
    nlohmann::json parsed_config = nlohmann::json::parse(conf_file);

    ngram_min_length = parsed_config.at("ngram_min_length");
    if (ngram_min_length < 1)
    {
        throw "Ngram min length is below 1";
        return -1;
    }

    ngram_max_length = parsed_config.at("ngram_max_length");
    if (ngram_max_length < ngram_min_length)
    {
        throw "Max length of ngram less than min length";
        return -1;
    }

    text = parsed_config.at("text");
    if (text.empty())
    {
        throw "Search string is empty";
        return -1;
    }

    stop_words = parsed_config.at("stop_words");
    return 0;
}

std::vector<std::string> string_tokenization(std::string& text)
{
    std::vector<std::string> text_tokens;
    std::string token;
    int text_length = text.length();
    for (int i = 0; i <= text_length; i++)
    {
        if (isspace(text[i]) || (text[i] == '\0'))
        {
            text_tokens.push_back(token);
            token.erase();
        }
        else
        {
            token += text[i];
        }
    }
    return text_tokens;
}

void run_parser(const std::string& config_filename)
{
    std::string text;
    std::vector<std::string> stop_words;
    int ngram_min_length;
    int ngram_max_length;
    std::vector<std::string> text_tokens;

    if (parse_config(config_filename, text, stop_words, ngram_min_length, ngram_max_length) == -1)
    {
        return;
    }
    remove_puctuation(text);
    char_to_lower_case(text);
    text_tokens = string_tokenization(text);
}

}  // namespace fts