#include <fts/index_builder.hpp>
#include <fts/query_parser.hpp>
#include <fts/word_hash.hpp>
#include <rapidcsv.h>
#include <string>
#include <vector>

namespace fts {

void IndexBuilder::add_document(uint32_t document_id, const std::string& text)
{
    const std::vector<fts::Ngram> ngrams = fts::parse_query(config, text);

    if (ngrams.empty())
    {
        return;
    }

    index.docs[document_id] = text;

    for (const auto& ngram : ngrams)
    {
        std::string word_hash = fts::get_word_hash(ngram.word);
        index.entries[word_hash][ngram.word][document_id].emplace_back(ngram.index);
    }
}

void IndexBuilder::parse_csv(const std::string& filename)
{
    const std::filesystem::path entrie_doc_path(filename);

    if (!std::filesystem::exists(entrie_doc_path))
    {
        throw std::runtime_error{'"' + filename + '"' + " not found"};
    }

    rapidcsv::Document csv_doc(filename, rapidcsv::LabelParams(0, 0), rapidcsv::SeparatorParams(','));

    const std::string book_id_col = "bookID";
    const std::string book_title_col = "title";

    const std::size_t col_index_book_id = csv_doc.GetColumnIdx(book_id_col);
    std::vector<int> book_ids = csv_doc.GetColumn<int>(col_index_book_id);

    for (std::size_t i = 0; i < book_ids.size(); i++)
    {
        add_document(book_ids.at(i), csv_doc.GetCell<std::string>(book_title_col, i));
    }
}

}  // namespace fts