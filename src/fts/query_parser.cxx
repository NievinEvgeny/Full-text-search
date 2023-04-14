#include <fts/config.hpp>
#include <fts/query_parser.hpp>
#include <vector>
#include <algorithm>
#include <string>
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

static void delete_stop_words(std::vector<std::string>& text_tokens, const std::unordered_set<std::string>& stop_words)
{
    text_tokens.erase(
        std::remove_if(
            text_tokens.begin(),
            text_tokens.end(),
            [&](const std::string& token) { return (stop_words.find(token) != stop_words.end()); }),
        text_tokens.end());
}

static std::vector<fts::Ngram> ngram_generate(const std::vector<std::string>& text_tokens, int min_len, int max_len)
{
    std::vector<fts::Ngram> ngrams;
    int index = 0;

    for (const auto& text_token : text_tokens)
    {
        if (static_cast<int>(text_token.size()) < min_len)
        {
            continue;
        }

        int temp_max_len = std::min(max_len, static_cast<int>(text_token.size()));

        for (int j = min_len; j <= temp_max_len; j++)
        {
            fts::Ngram temp_ngram{index, text_token.substr(0, j)};
            ngrams.push_back(temp_ngram);
        }
        index++;
    }

    return ngrams;
}

std::vector<std::string> string_tokenize(const std::string& text)
{
    std::vector<std::string> text_tokens;
    std::string token;
    std::size_t text_len = (text.length());

    for (std::size_t i = 0; i <= text_len; i++)
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

std::vector<fts::Ngram> parse_query(const fts::ConfOptions& config, const std::string& text)
{
    std::vector<std::string> text_tokens;
    std::vector<fts::Ngram> ngrams;
    std::string text_copy = text;

    fts::remove_punctuation(text_copy);
    fts::char_to_lower_case(text_copy);
    text_tokens = fts::string_tokenize(text_copy);
    fts::delete_stop_words(text_tokens, config.stop_words);
    ngrams = fts::ngram_generate(text_tokens, config.ngram_min_len, config.ngram_max_len);

    return ngrams;
}

}  // namespace fts