#include <fts/conf_parser.hpp>
#include <fts/indexer.hpp>
#include <gtest/gtest.h>
#include <fts/word_hash.hpp>
#include <string>

TEST(add_document, num_of_added_docs)
{
    const std::string conf_filename = "../../../RunOptions.json";
    const fts::ConfOptions config = fts::parse_config(conf_filename);

    fts::IndexBuilder indexes{config};

    indexes.add_document(103975, "The Matrix Matrix Matrix Reloaded Revolution");
    indexes.add_document(238695, "The Matrix Revolution");
    indexes.add_document(390473, "The Matrix");

    EXPECT_TRUE(indexes.get_index().docs.size() == 3);
}

TEST(add_document, text_in_index_docs)
{
    const std::string conf_filename = "../../../RunOptions.json";
    const fts::ConfOptions config = fts::parse_config(conf_filename);

    fts::IndexBuilder indexes{config};

    indexes.add_document(103975, "The Matrix Matrix Matrix Reloaded Revolution");
    indexes.add_document(238695, "The Matrix Revolution");
    indexes.add_document(390473, "The Matrix");

    EXPECT_TRUE(indexes.get_index().docs.at(238695) == "The Matrix Revolution");
}

TEST(add_document, non_exist_doc_id)
{
    const std::string conf_filename = "../../../RunOptions.json";
    const fts::ConfOptions config = fts::parse_config(conf_filename);

    fts::IndexBuilder indexes{config};

    indexes.add_document(103975, "The Matrix Matrix Matrix Reloaded Revolution");
    indexes.add_document(238695, "The Matrix Revolution");
    indexes.add_document(390473, "The Matrix");

    EXPECT_THROW(indexes.get_index().docs.at(1), std::out_of_range);
}

TEST(add_document, check_rewrited_doc)
{
    const std::string conf_filename = "../../../RunOptions.json";
    const fts::ConfOptions config = fts::parse_config(conf_filename);

    fts::IndexBuilder indexes{config};

    indexes.add_document(103975, "The Matrix Matrix Matrix Reloaded Revolution");
    indexes.add_document(103975, "The pepega");

    const std::string term = "pepega";
    std::string text_hash = fts::get_word_hash(term);

    EXPECT_TRUE(indexes.get_index().docs.at(103975) == "The pepega");
}

TEST(add_document, check_num_of_entries)
{
    const std::string conf_filename = "../../../RunOptions.json";
    const nlohmann::json parsed_conf = fts::parse_config(conf_filename);
    const fts::ConfOptions config = fts::parse_json_struct(parsed_conf);

    fts::IndexBuilder indexes{config};

    indexes.add_document(103975, "The Matrix Matrix Matrix Reloaded Revolution");
    indexes.add_document(238695, "The Matrix Revolution");
    indexes.add_document(390473, "The Matrix");

    EXPECT_TRUE(indexes.get_index().entries.size() == 12);
}

TEST(add_document, check_num_of_docs_with_term)
{
    const std::string conf_filename = "../../../RunOptions.json";
    const fts::ConfOptions config = fts::parse_config(conf_filename);

    fts::IndexBuilder indexes{config};

    indexes.add_document(103975, "The Matrix Matrix Matrix Reloaded Revolution");
    indexes.add_document(238695, "The Matrix Revolution");
    indexes.add_document(390473, "The Matrix");

    const std::string term = "mat";
    std::string text_hash = fts::get_word_hash(term);

    EXPECT_TRUE(indexes.get_index().entries.at(text_hash).at(term).size() == 3);
}

TEST(add_document, num_of_docs_with_term_is_zero)
{
    const std::string conf_filename = "../../../RunOptions.json";
    const fts::ConfOptions config = fts::parse_config(conf_filename);

    fts::IndexBuilder indexes{config};

    indexes.add_document(103975, "The Matrix Matrix Matrix Reloaded Revolution");
    indexes.add_document(238695, "The Matrix Revolution");
    indexes.add_document(390473, "The Matrix");

    const std::string term = "clown";
    std::string text_hash = fts::get_word_hash(term);

    EXPECT_THROW(indexes.get_index().entries.at(text_hash).at(term).size(), std::out_of_range);
}

TEST(add_document, check_term_positions_in_doc)
{
    const std::string conf_filename = "../../../RunOptions.json";
    const fts::ConfOptions config = fts::parse_config(conf_filename);

    fts::IndexBuilder indexes{config};

    indexes.add_document(103975, "The Matrix Matrix Reloaded Revolution Matrix");
    indexes.add_document(238695, "The Matrix Revolution");
    indexes.add_document(390473, "The Matrix");

    const std::string term = "matrix";
    std::string text_hash = fts::get_word_hash(term);

    std::vector<int> exp{0, 1, 4};

    for (int i = 0; i < 3; i++)
    {
        EXPECT_TRUE(indexes.get_index().entries.at(text_hash).at(term).at(103975).at(i) == exp.at(i));
    }
}
