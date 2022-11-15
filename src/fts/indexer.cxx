#include <fts/indexer.hpp>
#include <fts/parser.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

namespace fts {

void IndexBuilder::add_document(int document_id, const std::string& text, const fts::ConfOptions& conf_options)
{
    index.docs[document_id] = text;

    const std::vector<fts::Ngram> ngrams = fts::parse_query(conf_options, text);

    for (const auto& ngram : ngrams)
    {
        std::string word_hash = fts::get_word_hash(ngram.word);
        index.entries[word_hash][ngram.word][document_id].push_back(ngram.index);
    }
}

void TextIndexWriter::write(const fts::Index& index)
{
    for (const auto& [doc_id, text] : index.docs)
    {
        std::ofstream current_doc(index_dir_path + "/docs/" += std::to_string(doc_id));

        if (!current_doc.is_open())
        {
            throw std::runtime_error{"Can't open file in TextIndexWriter::write function"};
        }

        current_doc << text;

        current_doc.close();
    }
    for (const auto& [term_hash, terms] : index.entries)
    {
        std::ofstream current_entrie(index_dir_path + "/entries/" += term_hash);

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