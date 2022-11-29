#include <fts/conf_parser.hpp>
#include <fts/query_parser.hpp>
#include <gtest/gtest.h>
#include <string>
#include <unordered_set>

TEST(string_tokenize, simple)
{
    std::vector<std::string> expect{"dr", "jekyll", "and", "mr", "hyde"};
    std::string text = "dr jekyll and mr hyde";
    std::vector<std::string> real = fts::string_tokenize(text);
    for (int i = 0; i < 5; i++)
    {
        EXPECT_TRUE(expect[i] == real[i]);
    }
}

TEST(string_tokenize, white_spaces)
{
    std::string text = "   ";
    std::vector<std::string> real = fts::string_tokenize(text);
    EXPECT_TRUE(real.empty());
}

TEST(string_tokenize, empty)
{
    std::string text = "";
    std::vector<std::string> real = fts::string_tokenize(text);
    EXPECT_TRUE(real.empty());
}

TEST(string_tokenize, white_spaces_everywhere)
{
    std::vector<std::string> expect{5, "a"};
    std::string text = "    a a a  a  a    ";
    std::vector<std::string> real = fts::string_tokenize(text);
    for (int i = 0; i < 5; i++)
    {
        EXPECT_TRUE(expect[i] == real[i]);
    }
}

TEST(parse_query, simple)
{
    const std::string conf_filename = "../../../RunOptions.json";
    const nlohmann::json parsed_conf = fts::parse_config(conf_filename);
    const fts::ConfOptions conf_options = fts::parse_json_struct(parsed_conf);

    std::vector<std::string> expect{"jek", "jeky", "jekyl", "jekyll", "hyd", "hyde"};
    std::string text = "dr. jekyll and hyde";
    std::vector<fts::Ngram> real = fts::parse_query(conf_options, text);
    for (int i = 0; i < 6; i++)
    {
        EXPECT_TRUE(expect.at(i) == real.at(i).word);
    }
}

TEST(parse_query, only_stop_words)
{
    const std::string conf_filename = "../../../RunOptions.json";
    const nlohmann::json parsed_conf = fts::parse_config(conf_filename);
    const fts::ConfOptions conf_options = fts::parse_json_struct(parsed_conf);

    std::string text = "dr. with  , and, the.";
    std::vector<fts::Ngram> real = fts::parse_query(conf_options, text);

    EXPECT_TRUE(real.empty());
}

TEST(parse_query, words_len_below_min)
{
    const std::string conf_filename = "../../../RunOptions.json";
    const nlohmann::json parsed_conf = fts::parse_config(conf_filename);
    const fts::ConfOptions conf_options = fts::parse_json_struct(parsed_conf);

    std::string text = "dr je a1 hy";
    std::vector<fts::Ngram> real = fts::parse_query(conf_options, text);

    EXPECT_TRUE(real.empty());
}

TEST(parse_query, numbers_in_words)
{
    const std::string conf_filename = "../../../RunOptions.json";
    const nlohmann::json parsed_conf = fts::parse_config(conf_filename);
    const fts::ConfOptions conf_options = fts::parse_json_struct(parsed_conf);

    std::vector<std::string> expect{"a12", "hyd", "hyde"};
    std::string text = "dr. a12 and hyde";
    std::vector<fts::Ngram> real = fts::parse_query(conf_options, text);
    for (int i = 0; i < 3; i++)
    {
        EXPECT_TRUE(expect.at(i) == real.at(i).word);
    }
}

TEST(parse_query, long_words)
{
    const std::string conf_filename = "../../../RunOptions.json";
    const nlohmann::json parsed_conf = fts::parse_config(conf_filename);
    const fts::ConfOptions conf_options = fts::parse_json_struct(parsed_conf);

    std::vector<std::string> expect{"pep", "pepe", "pepeg", "pepegu", "che", "cheb", "chebu", "chebur"};
    std::string text = "pepegus and cheburashka";
    std::vector<fts::Ngram> real = fts::parse_query(conf_options, text);
    for (int i = 0; i < 8; i++)
    {
        EXPECT_TRUE(expect.at(i) == real.at(i).word);
    }
}

TEST(parse_query, check_term_pos)
{
    const std::string conf_filename = "../../../RunOptions.json";
    const nlohmann::json parsed_conf = fts::parse_config(conf_filename);
    const fts::ConfOptions conf_options = fts::parse_json_struct(parsed_conf);

    std::string text = "dr. jekyll and hyde";
    std::vector<fts::Ngram> expect = {{0, "jek"}, {0, "jeky"}, {0, "jekyl"}, {0, "jekyll"}, {1, "hyd"}, {1, "hyde"}};
    std::vector<fts::Ngram> real = fts::parse_query(conf_options, text);

    for (int i = 0; i < 6; i++)
    {
        EXPECT_TRUE(expect.at(i).index == real.at(i).index);
        EXPECT_TRUE(expect.at(i).word == real.at(i).word);
    }
}

TEST(parse_query, big_and_small_words)
{
    const std::string conf_filename = "../../../RunOptions.json";
    const nlohmann::json parsed_conf = fts::parse_config(conf_filename);
    const fts::ConfOptions conf_options = fts::parse_json_struct(parsed_conf);

    std::string text = "peepoclown hy";
    std::vector<fts::Ngram> expect = {{0, "pee"}, {0, "peep"}, {0, "peepo"}, {0, "peepoc"}};
    std::vector<fts::Ngram> real = fts::parse_query(conf_options, text);

    for (int i = 0; i < 4; i++)
    {
        EXPECT_TRUE(expect.at(i).index == real.at(i).index);
        EXPECT_TRUE(expect.at(i).word == real.at(i).word);
    }
}
