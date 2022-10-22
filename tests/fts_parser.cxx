#include <fts/parser.hpp>
#include <fts/indexer.hpp>
#include <PicoSHA2/picosha2.h>
#include <gtest/gtest.h>
#include <string>
#include <unordered_set>
#include <fstream>

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

TEST(write, check_text_in_index_docs)
{
    char text_from_doc[256];
    int doc_id = 103975;
    std::string path = "../../../index";
    std::string query = "The Matrix Reloaded Revolution";
    const std::string conf_filename = "../../../RunOptions.json";

    fts::ConfOptions conf_options = fts::parse_config(conf_filename);
    fts::IndexBuilder indexes;
    fts::TextIndexWriter index_writer(path);

    indexes.add_document(doc_id, query, conf_options);
    index_writer.write(indexes);

    std::ifstream doc_to_check(path + "/docs/" += std::to_string(doc_id));
    doc_to_check.getline(text_from_doc, 256);
    EXPECT_TRUE(text_from_doc == query);
}

TEST(write, check_entries_in_index)
{
    char text_from_file[256];
    int doc_id = 103975;
    std::string exp_entrie = "revolu 1 103975 1 2 ";

    std::string term_hash = "9e2f4d";
    std::string path = "../../../index";
    std::string query = "The Matrix Reloaded Revolution";
    const std::string conf_filename = "../../../RunOptions.json";

    fts::ConfOptions conf_options = fts::parse_config(conf_filename);
    fts::IndexBuilder indexes;
    fts::TextIndexWriter index_writer(path);

    indexes.add_document(doc_id, query, conf_options);
    index_writer.write(indexes);

    std::ifstream current_doc(path + "/entries/" += term_hash);
    current_doc.getline(text_from_file, 256);
    EXPECT_TRUE(text_from_file == exp_entrie);
}