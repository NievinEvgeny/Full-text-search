#include <fts/fts.hpp>
#include <gtest/gtest.h>
#include <string>

TEST(char_to_lower_case, simple)
{
    std::string expect = "dr. jekyll and mr. hyde";
    std::string real = "Dr. Jekyll and Mr. Hyde";
    fts::char_to_lower_case(real);
    EXPECT_TRUE(expect == real);
}

TEST(char_to_lower_case, no_capital_letters)
{
    std::string expect = "dr. jekyll and mr. hyde";
    std::string real = "dr. jekyll and mr. hyde";
    fts::char_to_lower_case(real);
    EXPECT_TRUE(expect == real);
}

TEST(char_to_lower_case, empty_string)
{
    std::string expect = "";
    std::string real = "";
    fts::char_to_lower_case(real);
    EXPECT_TRUE(expect == real);
}

TEST(remove_punctuation, simple)
{
    std::string expect = "Dr Jekyll and Mr Hyde";
    std::string real = "Dr. Jekyll and Mr. Hyde";
    fts::remove_punctuation(real);
    EXPECT_TRUE(expect == real);
}

TEST(remove_punctuation, no_punct)
{
    std::string expect = "Dr Jekyll and Mr Hyde";
    std::string real = "Dr Jekyll and Mr Hyde";
    fts::remove_punctuation(real);
    EXPECT_TRUE(expect == real);
}

TEST(remove_punctuation, empty_string)
{
    std::string expect = "";
    std::string real = "";
    fts::remove_punctuation(real);
    EXPECT_TRUE(expect == real);
}

TEST(remove_punctuation, do_not_ruin_text)
{
    std::string expect = "ABCDE";
    std::string real = ".A,B.C,D.E,.,.,.,.,,";
    fts::remove_punctuation(real);
    EXPECT_TRUE(expect == real);
}

TEST(remove_punctuation, only_punct_in_text)
{
    std::string expect = "";
    std::string real = ".,.,.,.,.,,";
    fts::remove_punctuation(real);
    EXPECT_TRUE(expect == real);
}

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
    std::vector<std::string> expect;
    std::string text = "   ";
    std::vector<std::string> real = fts::string_tokenization(text);
    EXPECT_TRUE(expect.size() == real.size());
}

TEST(string_tokenization, empty)
{
    std::vector<std::string> expect;
    std::string text = "";
    std::vector<std::string> real = fts::string_tokenization(text);
    EXPECT_TRUE(expect.size() == real.size());
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
    std::vector<std::string> stop_words{"dr", "and", "mr"};
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
    std::vector<std::string> stop_words{"dr", "and", "mr", "a", "with"};
    std::vector<std::string> expect;
    std::vector<std::string> real{"dr", "and", "mr", "a", "with"};
    fts::delete_stop_words(real, stop_words);
    EXPECT_TRUE(expect.size() == real.size());
}

TEST(delete_stop_words, numbers_in_stop_words)
{
    std::vector<std::string> stop_words{"dr", "and", "mr", "a12", "with"};
    std::vector<std::string> expect;
    std::vector<std::string> real{"dr", "and", "mr", "a12", "with"};
    fts::delete_stop_words(real, stop_words);
    EXPECT_TRUE(expect.size() == real.size());
}

TEST(ngram_generation, simple)
{
    int ngram_min_length = 3, ngram_max_length = 6;
    std::vector<std::string> text_tokens{"jekyll", "hyde"};

    std::vector<fts::ngram> expect;
    fts::ngram temp = {0, "jek"};
    expect.push_back(temp);
    temp = {0, "jeky"};
    expect.push_back(temp);
    temp = {0, "jekyl"};
    expect.push_back(temp);
    temp = {0, "jekyll"};
    expect.push_back(temp);
    temp = {1, "hyd"};
    expect.push_back(temp);
    temp = {1, "hyde"};
    expect.push_back(temp);
    std::vector<fts::ngram> real;

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

    std::vector<fts::ngram> expect;
    std::vector<fts::ngram> real;

    real = fts::ngram_generation(text_tokens, ngram_min_length, ngram_max_length);

    EXPECT_TRUE(expect.size() == real.size());
}

TEST(ngram_generation, big_and_small_words)
{
    int ngram_min_length = 3, ngram_max_length = 6;
    std::vector<std::string> text_tokens;
    text_tokens.push_back("peepoclown");
    text_tokens.push_back("hy");

    std::vector<fts::ngram> expect;
    fts::ngram temp = {0, "pee"};
    expect.push_back(temp);
    temp = {0, "peep"};
    expect.push_back(temp);
    temp = {0, "peepo"};
    expect.push_back(temp);
    temp = {0, "peepoc"};
    expect.push_back(temp);

    std::vector<fts::ngram> real;

    real = fts::ngram_generation(text_tokens, ngram_min_length, ngram_max_length);
    for (int i = 0; i < 4; i++)
    {
        std::cout << expect[i].word << '\n';
        std::cout << real[i].word << '\n';
        EXPECT_TRUE(expect[i].index == real[i].index);
        EXPECT_TRUE(expect[i].word == real[i].word);
    }
}