#include <fts/searcher.hpp>
#include <fts/conf_parser.hpp>
#include <fts/query_parser.hpp>
#include <fts/indexer.hpp>
#include <gtest/gtest.h>
#include <filesystem>
#include <cmath>
#include <string>
#include <vector>

TEST(IndexAccessor, check_scores)
{
    const std::string conf_filename = "../../../RunOptions.json";
    const fts::ConfOptions config = fts::parse_config(conf_filename);

    fts::IndexBuilder indexes{config};

    indexes.add_document(1, "The Matrix");
    indexes.add_document(2, "The Clown");
    indexes.add_document(3, "The PoroSad");

    const std::string temp_dir = std::filesystem::temp_directory_path().string();
    const std::filesystem::path index_docs(temp_dir + "/docs");
    const std::filesystem::path index_entries(temp_dir + "/entries");

    const std::vector<std::filesystem::path> directories = {index_docs, index_entries};

    for (const auto& dir : directories)
    {
        if (!std::filesystem::exists(dir))
        {
            std::filesystem::create_directory(dir);
        }
    }

    fts::TextIndexWriter index_writer(temp_dir);
    index_writer.write(indexes.get_index());

    const std::string query = "Matrix";
    const std::vector<fts::Ngram> ngrams = fts::parse_query(config, query);

    fts::IndexAccessor index_accessor(temp_dir, ngrams);
    const std::vector<fts::DocScore> doc_scores = index_accessor.get_scores();

    std::filesystem::remove_all(index_docs);
    std::filesystem::remove_all(index_entries);

    ASSERT_NEAR(doc_scores.at(0).score, 4.39444, 0.0001);
    ASSERT_DOUBLE_EQ(doc_scores.at(1).score, 0);
    ASSERT_DOUBLE_EQ(doc_scores.at(2).score, 0);
}

TEST(IndexAccessor, score_sort_stability)
{
    const std::string conf_filename = "../../../RunOptions.json";
    const fts::ConfOptions config = fts::parse_config(conf_filename);

    fts::IndexBuilder indexes{config};

    indexes.add_document(1, "The Matrix");
    indexes.add_document(2, "The Matrix");

    const std::string temp_dir = std::filesystem::temp_directory_path().string();
    const std::filesystem::path index_docs(temp_dir + "/docs");
    const std::filesystem::path index_entries(temp_dir + "/entries");

    const std::vector<std::filesystem::path> directories = {index_docs, index_entries};

    for (const auto& dir : directories)
    {
        if (!std::filesystem::exists(dir))
        {
            std::filesystem::create_directory(dir);
        }
    }

    fts::TextIndexWriter index_writer(temp_dir);
    index_writer.write(indexes.get_index());

    const std::string query = "Matrix";
    const std::vector<fts::Ngram> ngrams = fts::parse_query(config, query);

    fts::IndexAccessor index_accessor(temp_dir, ngrams);
    const std::vector<fts::DocScore> doc_scores = index_accessor.get_scores();

    std::filesystem::remove_all(index_docs);
    std::filesystem::remove_all(index_entries);

    EXPECT_TRUE(doc_scores.at(0).score == doc_scores.at(1).score);
    EXPECT_TRUE(doc_scores.at(0).doc_id < doc_scores.at(1).doc_id);
}