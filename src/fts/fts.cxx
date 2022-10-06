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

void remove_punctuation(std::string& text)
{
    for (int i = static_cast<int>(text.size()) - 1; i >= 0; i--)
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
    int& ngram_min_length,
    int& ngram_max_length)
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
    int text_length = static_cast<int>(text.length());
    for (int i = 0; i <= text_length; i++)
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

void delete_stop_words(std::vector<std::string>& text_tokens, std::vector<std::string>& stop_words)
{
    for (int i = 0; i < static_cast<int>(text_tokens.size()); i++)
    {
        for (auto& stop_word : stop_words)
        {
            if (text_tokens[i] == stop_word)
            {
                text_tokens.erase(text_tokens.begin() + i);
                i--;
                break;
            }
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

void run_parser(const std::string& config_filename)
{
    std::string text;
    std::vector<std::string> stop_words;
    int ngram_min_length = 0;
    int ngram_max_length = 0;
    std::vector<std::string> text_tokens;
    std::vector<ngram> ngrams;

    if (parse_config(config_filename, text, stop_words, ngram_min_length, ngram_max_length) == -1)
    {
        return;
    }
    remove_punctuation(text);
    char_to_lower_case(text);
    text_tokens = string_tokenization(text);

    if (text_tokens.empty())
    {
        throw "No relevant words in search string";
        return;
    }

    delete_stop_words(text_tokens, stop_words);

    if (text_tokens.empty())
    {
        throw "No relevant words in search string";
        return;
    }

    ngrams = ngram_generation(text_tokens, ngram_min_length, ngram_max_length);

    if (ngrams.empty())
    {
        throw "No words that can be used to generate ngrams";
        return;
    }

    for (auto& ngram : ngrams)
    {
        std::cout << ngram.word << ' ';
        std::cout << ngram.index << ' ';
    }
    std::cout << '\n';
}

}  // namespace fts