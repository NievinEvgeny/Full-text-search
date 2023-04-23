#include <fts/bin_index_writer.hpp>
#include <fts/binary_buffer.hpp>
#include <fts/trie.hpp>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>

namespace fts {

void BinIndexWriter::write(const fts::Index& index) const
{
    constexpr short hash_len = 6;

    fts::config_serialize(index_dir_path, config);

    fts::BinaryBuffer docs_data{sizeof(uint32_t) * index.docs.size()};
    fts::BinaryBuffer entries_data{hash_len * index.entries.size()};
    fts::Trie dictionary;

    std::unordered_map<uint32_t, uint32_t> id_to_offset;

    docs_data.write(static_cast<uint32_t>(index.docs.size()));

    for (const auto& [id, title] : index.docs)
    {
        id_to_offset[id] = docs_data.get_data().size();

        docs_data.write(static_cast<uint8_t>(title.size() + 1));
        docs_data.write(title);
    }

    for (const auto& [term_hash, terms] : index.entries)
    {
        for (const auto& [term, docs] : terms)
        {
            entries_data.write(static_cast<uint32_t>(docs.size()));

            dictionary.add(term, static_cast<uint32_t>(entries_data.get_data().size()));

            for (const auto& [doc_id, term_positions] : docs)
            {
                entries_data.write(id_to_offset.at(doc_id));
                entries_data.write(static_cast<uint32_t>(term_positions.size()));

                for (const auto& term_position : term_positions)
                {
                    entries_data.write(static_cast<uint32_t>(term_position));
                }
            }
        }
    }

    docs_data.serialize("index/docs.txt");
    entries_data.serialize("index/entries.txt");
}

}  // namespace fts