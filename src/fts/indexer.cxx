#include <fts/indexer.hpp>
#include <fts/parser.hpp>
#include <PicoSHA2/picosha2.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

namespace fts {

void IndexBuilder::add_document(int document_id, const std::string& text, const fts::ConfOptions& conf_options)
{
    index.docs[document_id] = text;

    const int hash_required_len = 6;
    const std::vector<fts::Ngram> ngrams = fts::parse_query(conf_options, text);

    for (const auto& ngram : ngrams)
    {
        std::string text_hash = picosha2::hash256_hex_string(ngram.word);
        text_hash.erase(hash_required_len);
        index.entries[text_hash][ngram.word][document_id].push_back(ngram.index);
    }
}

void TextIndexWriter::write(IndexBuilder& indexes)
{
    for (auto& doc : indexes.index.docs)
    {
        std::ofstream current_doc(index_dir_path + "/docs/" += std::to_string(doc.first));

        if (!current_doc.is_open())
        {
            throw std::runtime_error{"Can't open file in TextIndexWriter::write function"};
        }

        current_doc << doc.second;

        current_doc.close();
    }
    for (auto& [term_hash, terms] : indexes.index.entries)
    {
        for (auto& [term, docs] : terms)
        {
            std::ofstream current_entrie(index_dir_path + "/entries/" += term_hash);

            if (!current_entrie.is_open())
            {
                throw std::runtime_error{"Can't open file in TextIndexWriter::write function"};
            }

            current_entrie << term << ' ' << docs.size() << ' ';

            for (auto& [doc_id, term_positions] : docs)
            {
                current_entrie << doc_id << ' ' << term_positions.size() << ' ';

                for (auto& term_position : term_positions)
                {
                    current_entrie << term_position << ' ';
                }
            }

            current_entrie.close();
        }
    }
}

}  // namespace fts