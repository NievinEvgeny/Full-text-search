#include <fts/bin_index_writer.hpp>
#include <fts/binary_buffer.hpp>
#include <fts/trie.hpp>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>

namespace fts {

using umap = std::unordered_map<uint32_t, uint32_t>;

static umap store_dir_index(const fts::Index& index, fts::BinaryBuffer& docs_data)
{
    umap id_to_offset;

    docs_data.write(static_cast<uint32_t>(index.docs.size()));

    for (const auto& [id, title] : index.docs)
    {
        id_to_offset[id] = docs_data.get_data().size();

        docs_data.write(title);
    }

    return id_to_offset;
}

static void store_inv_index(
    const fts::Index& index,
    umap& id_to_offset,
    fts::BinaryBuffer& entries_data,
    fts::BinaryBuffer& dictionary_data)
{
    fts::Trie dictionary;

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

    dictionary.serialize(dictionary_data);
}

static uint32_t calc_header_size(const std::vector<std::pair<const std::string, uint32_t>>& section_names)
{
    uint32_t header_size = 0;

    header_size += sizeof(uint8_t) + section_names.size() * sizeof(uint32_t);

    for (const auto& [name, offset] : section_names)
    {
        header_size += name.size() + 1;
    }

    return header_size;
}

static fts::BinaryBuffer store_header(uint32_t dictionary_size, uint32_t entries_size)
{
    std::vector<std::pair<const std::string, uint32_t>> sections{
        {"dictionary", 0}, {"entries", dictionary_size}, {"docs", dictionary_size + entries_size}};

    const uint32_t header_size = calc_header_size(sections);

    fts::BinaryBuffer header{header_size};

    header.write(static_cast<uint8_t>(sections.size()));

    for (auto& section : sections)
    {
        section.second += header_size;
        header.write(section.first);
        header.write(section.second);
    }

    return header;
}

void BinIndexWriter::write(const fts::Index& index) const
{
    constexpr short hash_len = 6;

    const std::string index_filename = index_dir_path + "/index.txt";

    fts::BinaryBuffer docs_data{sizeof(uint32_t) * index.docs.size()};
    fts::BinaryBuffer entries_data{hash_len * index.entries.size()};
    fts::BinaryBuffer dictionary_data;

    fts::config_serialize(index_dir_path, config);
    umap id_to_offset = fts::store_dir_index(index, docs_data);
    fts::store_inv_index(index, id_to_offset, entries_data, dictionary_data);

    fts::BinaryBuffer header = store_header(dictionary_data.get_data().size(), entries_data.get_data().size());

    std::ofstream index_file;
    index_file.open(index_filename, std::ios::out | std::ios::binary);

    if (!index_file.is_open())
    {
        throw std::runtime_error{"Can't open file in BinIndexWriter::write function"};
    }

    header.serialize(index_file);
    dictionary_data.serialize(index_file);
    entries_data.serialize(index_file);
    docs_data.serialize(index_file);

    index_file.close();
}

}  // namespace fts