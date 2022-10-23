#include <fts/parser.hpp>
#include <gtest/gtest.h>
#include <string>
#include <unordered_set>

TEST(string_tokenization, simple)
{
    std::vector<std::string> expect{"dr", "jekyll", "and", "mr", "hyde"};
    std::string text = "dr jekyll and mr hyde";
    std::vector<std::string> real = fts::string_tokenization(text);
    for (int i = 0; i < 5; i++)
    {
        EXPECT_TRUE(expect[i] == real[i]);
    }
}

TEST(string_tokenization, white_spaces)
{
    std::string text = "   ";
    std::vector<std::string> real = fts::string_tokenization(text);
    EXPECT_TRUE(real.empty());
}

TEST(string_tokenization, empty)
{
    std::string text = "";
    std::vector<std::string> real = fts::string_tokenization(text);
    EXPECT_TRUE(real.empty());
}

TEST(string_tokenization, white_spaces_everywhere)
{
    std::vector<std::string> expect{5, "a"};
    std::string text = "    a a a  a  a    ";
    std::vector<std::string> real = fts::string_tokenization(text);
    for (int i = 0; i < 5; i++)
    {
        EXPECT_TRUE(expect[i] == real[i]);
    }
}

TEST(delete_stop_words, simple)
{
    const std::unordered_set<std::string> stop_words{"dr", "and", "mr"};
    std::vector<std::string> expect{"jekyll", "hyde"};
    std::vector<std::string> real{"dr", "jekyll", "and", "mr", "hyde"};
    fts::delete_stop_words(real, stop_words);
    for (int i = 0; i < 2; i++)
    {
        EXPECT_TRUE(expect[i] == real[i]);
    }
}

TEST(delete_stop_words, no_relevant_words)
{
    const std::unordered_set<std::string> stop_words{"dr", "and", "mr", "a", "with"};
    std::vector<std::string> real{"dr", "and", "mr", "a", "with"};
    fts::delete_stop_words(real, stop_words);
    EXPECT_TRUE(real.empty());
}

TEST(delete_stop_words, numbers_in_stop_words)
{
    const std::unordered_set<std::string> stop_words{"dr", "and", "mr", "a12", "with"};
    std::vector<std::string> expect;
    std::vector<std::string> real{"dr", "and", "mr", "a12", "with"};
    fts::delete_stop_words(real, stop_words);
    EXPECT_TRUE(expect.size() == real.size());
}

TEST(ngram_generation, simple)
{
    int ngram_min_length = 3, ngram_max_length = 6;
    std::vector<std::string> text_tokens{"jekyll", "hyde"};

    std::vector<fts::Ngram> expect = {{0, "jek"}, {0, "jeky"}, {0, "jekyl"}, {0, "jekyll"}, {1, "hyd"}, {1, "hyde"}};
    std::vector<fts::Ngram> real;

    real = fts::ngram_generation(text_tokens, ngram_min_length, ngram_max_length);
    for (int i = 0; i < 6; i++)
    {
        EXPECT_TRUE(expect[i].index == real[i].index);
        EXPECT_TRUE(expect[i].word == real[i].word);
    }
}

TEST(ngram_generation, non_relevant_words)
{
    int ngram_min_length = 3, ngram_max_length = 6;
    std::vector<std::string> text_tokens;
    text_tokens.push_back("je");
    text_tokens.push_back("hy");

    std::vector<fts::Ngram> expect;
    std::vector<fts::Ngram> real;

    real = fts::ngram_generation(text_tokens, ngram_min_length, ngram_max_length);

    EXPECT_TRUE(expect.size() == real.size());
}

TEST(ngram_generation, big_and_small_words)
{
    int ngram_min_length = 3, ngram_max_length = 6;
    std::vector<std::string> text_tokens;
    text_tokens.push_back("peepoclown");
    text_tokens.push_back("hy");

    std::vector<fts::Ngram> expect = {{0, "pee"}, {0, "peep"}, {0, "peepo"}, {0, "peepoc"}};
    std::vector<fts::Ngram> real;

    real = fts::ngram_generation(text_tokens, ngram_min_length, ngram_max_length);
    for (int i = 0; i < 4; i++)
    {
        EXPECT_TRUE(expect[i].index == real[i].index);
        EXPECT_TRUE(expect[i].word == real[i].word);
    }
}