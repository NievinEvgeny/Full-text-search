#include <fts/searcher.hpp>
#include <fts/conf_parser.hpp>
#include <fts/query_parser.hpp>
#include <fts/index_builder.hpp>
#include <fts/index_writer.hpp>
#include <gtest/gtest.h>
#include <filesystem>
#include <cmath>
#include <string>
#include <vector>

TEST(IndexAccessor, score_check_and_stability)
{
    const std::string conf_filename = "../../../RunOptions.json";
    const fts::ConfOptions config = fts::parse_config(conf_filename);

    fts::IndexBuilder indexes(config);

    indexes.add_document(1, "The Matrix");
    indexes.add_document(2, "The Matrix");
    indexes.add_document(3, "The Clown");

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

    fts::TextIndexWriter index_writer(temp_dir, config);
    index_writer.write(indexes.get_index());

    const std::string query = "Matrix";

    fts::TextIndexAccessor index_accessor(temp_dir, config);
    fts::Searcher searcher(index_accessor);
    const fts::SearchInfo search_result = searcher.score_calc(query);

    std::filesystem::remove_all(index_docs);
    std::filesystem::remove_all(index_entries);

    ASSERT_NEAR(search_result.docs_scores.at(0).score, 1.6218, 0.0001);
    EXPECT_TRUE(search_result.docs_scores.at(0).score == search_result.docs_scores.at(1).score);
    EXPECT_TRUE(search_result.docs_scores.at(0).doc_id < search_result.docs_scores.at(1).doc_id);
}