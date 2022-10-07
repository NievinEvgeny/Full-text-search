#include <fts/fts.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <cctype>

namespace fts {

static void char_to_lower_case(std::string& text)
{
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return std::tolower(c); });
}

static void remove_punctuation(std::string& text)
{
    for (int i = static_cast<int>(text.size()) - 1; i >= 0; i--)
    {
        if (ispunct(text[i]))
        {
            text.erase(i, 1);
        }
    }
}

struct conf_options parse_config(const std::string& config_filename)
{
    std::ifstream conf_file(config_filename);
    nlohmann::json parsed_config = nlohmann::json::parse(conf_file);

    // clang-format off
    fts::conf_options conf_options {
        parsed_config.at("stop_words"),
        parsed_config.at("ngram_min_length"),
        parsed_config.at("ngram_max_length")
    };
    // clang-format on

    if (conf_options.ngram_min_length < 1)
    {
        throw parse_exception{"Ngram min length is below 1"};
    }

    if (conf_options.ngram_max_length < conf_options.ngram_min_length)
    {
        throw parse_exception{"Max length of ngram less than min length"};
    }

    return conf_options;
}

std::vector<std::string> string_tokenization(std::string& text)
{
    std::vector<std::string> text_tokens;
    std::string token;
    std::size_t text_length = (text.length());
    for (std::size_t i = 0; i <= text_length; i++)
    {
        if (std::isspace(text[i]) || (text[i] == '\0'))
        {
            if (token.empty())
            {
                continue;
            }
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

void delete_stop_words(std::vector<std::string>& text_tokens, const std::unordered_set<std::string>& stop_words)
{
    for (int i = 0; i < static_cast<int>(text_tokens.size()); i++)
    {
        if (stop_words.find(text_tokens[i]) != stop_words.end())
        {
            text_tokens.erase(text_tokens.begin() + i);
            i--;
        }
    }
}

std::vector<ngram> ngram_generation(std::vector<std::string>& text_tokens, int ngram_min_length, int ngram_max_length)
{
    std::vector<ngram> ngrams;
    int index = 0;

    for (auto& text_token : text_tokens)
    {
        if (static_cast<int>(text_token.size()) < ngram_min_length)
        {
            continue;
        }

        ngram_max_length = std::min(ngram_max_length, static_cast<int>(text_token.size()));

        for (int j = ngram_min_length; j <= ngram_max_length; j++)
        {
            ngram temp_ngram{index, text_token.substr(0, j)};
            ngrams.push_back(temp_ngram);
        }
        index++;
    }
    return ngrams;
}

void run_parser(const std::string& config_filename, const std::string& text)
{
    std::vector<std::string> text_tokens;
    std::vector<ngram> ngrams;
    fts::conf_options conf_options = parse_config(config_filename);
    std::string text_copy = text;

    remove_punctuation(text_copy);
    char_to_lower_case(text_copy);
    text_tokens = string_tokenization(text_copy);

    if (text_tokens.empty())
    {
        throw parse_exception{"No relevant words in search string"};
    }

    delete_stop_words(text_tokens, conf_options.stop_words);

    if (text_tokens.empty())
    {
        throw parse_exception{"No relevant words in search string"};
    }

    ngrams = ngram_generation(text_tokens, conf_options.ngram_min_length, conf_options.ngram_max_length);

    if (ngrams.empty())
    {
        throw parse_exception{"No words that can be used to generate ngrams"};
    }

    for (auto& ngram : ngrams)
    {
        std::cout << ngram.word << ' ';
        std::cout << ngram.index << ' ';
    }
    std::cout << '\n';
}

}  // namespace fts