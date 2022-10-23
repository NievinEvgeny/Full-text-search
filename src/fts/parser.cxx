#include <fts/parser.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>
#include <cctype>

namespace fts {

static unsigned char punct_to_space(unsigned char letter)
{
    return std::ispunct(letter) ? ' ' : letter;
}

static void char_to_lower_case(std::string& text)
{
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return std::tolower(c); });
}

static void remove_punctuation(std::string& text)
{
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return fts::punct_to_space(c); });
}

fts::ConfOptions parse_config(const std::string& conf_filename)
{
    std::ifstream conf_file(conf_filename);
    nlohmann::json parsed_conf = nlohmann::json::parse(conf_file);

    fts::ConfOptions conf_options{
        parsed_conf.at("stop_words"),
        parsed_conf.at("ngram_min_length"),
        parsed_conf.at("ngram_max_length"),
    };

    if (conf_options.ngram_min_length < 1)
    {
        throw ParseException{"Ngram min length is below 1"};
    }

    if (conf_options.ngram_max_length < conf_options.ngram_min_length)
    {
        throw ParseException{"Max length of ngram less than min length"};
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
    for (std::size_t i = 0; i < text_tokens.size(); i++)
    {
        if (stop_words.find(text_tokens[i]) != stop_words.end())
        {
            text_tokens.erase(text_tokens.begin() + static_cast<int>(i));
            i--;
        }
    }
}

std::vector<Ngram> ngram_generation(std::vector<std::string>& text_tokens, int ngram_min_length, int ngram_max_length)
{
    std::vector<Ngram> ngrams;
    int index = 0;

    for (auto& text_token : text_tokens)
    {
        if (static_cast<int>(text_token.size()) < ngram_min_length)
        {
            continue;
        }

        for (int j = ngram_min_length; j <= std::min(ngram_max_length, static_cast<int>(text_token.size())); j++)
        {
            Ngram temp_ngram{index, text_token.substr(0, j)};
            ngrams.push_back(temp_ngram);
        }
        index++;
    }
    return ngrams;
}

std::vector<Ngram> parse_query(const fts::ConfOptions& conf_options, const std::string& text)
{
    std::vector<std::string> text_tokens;
    std::vector<Ngram> ngrams;
    std::string text_copy = text;

    remove_punctuation(text_copy);
    char_to_lower_case(text_copy);
    text_tokens = string_tokenization(text_copy);

    if (text_tokens.empty())
    {
        throw ParseException{"No relevant words in search string"};
    }

    delete_stop_words(text_tokens, conf_options.stop_words);

    if (text_tokens.empty())
    {
        throw ParseException{"No relevant words in search string"};
    }

    ngrams = ngram_generation(text_tokens, conf_options.ngram_min_length, conf_options.ngram_max_length);

    if (ngrams.empty())
    {
        throw ParseException{"No words that can be used to generate ngrams"};
    }

    return ngrams;
}

}  // namespace fts