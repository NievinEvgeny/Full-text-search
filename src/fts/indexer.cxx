#include <fts/indexer.hpp>
#include <fts/conf_parser.hpp>
#include <fts/query_parser.hpp>
#include <fts/word_hash.hpp>
#include <rapidcsv.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

namespace fts {

void IndexBuilder::add_document(int document_id, const std::string& text)
{
    const std::vector<fts::Ngram> ngrams = fts::parse_query(this->config, text);

    if (ngrams.empty())
    {
        return;
    }

    this->index.docs[document_id] = text;

    for (const auto& ngram : ngrams)
    {
        std::string word_hash = fts::get_word_hash(ngram.word);
        this->index.entries[word_hash][ngram.word][document_id].push_back(ngram.index);
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

    const size_t col_index_book_id = csv_doc.GetColumnIdx(book_id_col);
    std::vector<int> book_ids = csv_doc.GetColumn<int>(col_index_book_id);

    for (size_t i = 0; i < book_ids.size(); i++)
    {
        this->add_document(book_ids.at(i), csv_doc.GetCell<std::string>(book_title_col, i));
    }
}

void TextIndexWriter::write(const fts::Index& index)
{
    for (const auto& [doc_id, text] : index.docs)
    {
        std::ofstream current_doc(this->index_dir_path + "/docs/" += std::to_string(doc_id));

        if (!current_doc.is_open())
        {
            throw std::runtime_error{"Can't open file in TextIndexWriter::write function"};
        }

        current_doc << text;

        current_doc.close();
    }
    for (const auto& [term_hash, terms] : index.entries)
    {
        std::ofstream current_entrie(this->index_dir_path + "/entries/" += term_hash);

        for (const auto& [term, docs] : terms)
        {
            if (!current_entrie.is_open())
            {
                throw std::runtime_error{"Can't open file in TextIndexWriter::write function"};
            }

            current_entrie << term << ' ' << docs.size() << ' ';

            for (const auto& [doc_id, term_positions] : docs)
            {
                current_entrie << doc_id << ' ' << term_positions.size() << ' ';

                for (const auto& term_position : term_positions)
                {
                    current_entrie << term_position << ' ';
                }
            }
            current_entrie << '\n';
        }
        current_entrie.close();
    }
}

}  // namespace fts