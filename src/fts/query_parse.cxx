#include <fts/parser.hpp>
#include <PicoSHA2/picosha2.h>
#include <vector>
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

std::string get_word_hash(const std::string& word)
{
    const int hash_required_len = 6;
    std::string word_hash = picosha2::hash256_hex_string(word);
    return word_hash.erase(hash_required_len);
}

std::vector<std::string> string_tokenization(const std::string& text)
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

std::vector<Ngram> ngram_generation(const std::vector<std::string>& text_tokens, int ngram_min_len, int ngram_max_len)
{
    std::vector<Ngram> ngrams;
    int index = 0;

    for (const auto& text_token : text_tokens)
    {
        if (static_cast<int>(text_token.size()) < ngram_min_len)
        {
            continue;
        }

        int temp_max_len = std::min(ngram_max_len, static_cast<int>(text_token.size()));

        for (int j = ngram_min_len; j <= temp_max_len; j++)
        {
            Ngram temp_ngram{index, text_token.substr(0, j)};
            ngrams.push_back(temp_ngram);
        }
        index++;
    }

    return ngrams;
}

std::vector<Ngram> parse_query(const fts::ConfOptions& config, const std::string& text)
{
    std::vector<std::string> text_tokens;
    std::vector<Ngram> ngrams;
    std::string text_copy = text;

    remove_punctuation(text_copy);
    char_to_lower_case(text_copy);
    text_tokens = string_tokenization(text_copy);
    delete_stop_words(text_tokens, config.stop_words);
    ngrams = ngram_generation(text_tokens, config.ngram_min_len, config.ngram_max_len);

    return ngrams;
}

}  // namespace fts