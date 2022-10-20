#include <fts/indexer.hpp>
#include <fts/parser.hpp>
#include <PicoSHA2/picosha2.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace fts {

void IndexBuilder::add_document(int document_id, const std::string& text, fts::ConfOptions conf_options)
{
    index.docs[document_id] = text;

    const int hash_required_len = 6;
    std::vector<fts::Ngram> ngrams = fts::parse_query(conf_options, text);

    for (auto& ngram : ngrams)
    {
        std::string text_hash = picosha2::hash256_hex_string(ngram.word);
        text_hash.erase(hash_required_len);
        index.entries[text_hash][ngram.word][document_id].push_back(ngram.index);
    }
}

}  // namespace fts