#include <fts/bin_index_writer.hpp>
#include <fts/binary_buffer.hpp>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>

namespace fts {

static void serialize_data(const fts::BinaryBuffer& data, const std::string& temp)
{
    const std::string& index_filename = temp;

    std::ofstream index_file;
    index_file.open(index_filename, std::ios::out | std::ios::binary);

    if (!index_file.is_open())
    {
        throw std::runtime_error{"Can't open file in BinIndexWriter::write function"};
    }

    index_file.write(data.get_data().data(), static_cast<long>(data.get_data().size()));
    index_file.close();
}

void BinIndexWriter::write(const fts::Index& index) const
{
    fts::config_serialize(index_dir_path, config);

    fts::BinaryBuffer docs_data;
    fts::BinaryBuffer entries_data;

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

    serialize_data(docs_data, "index/docs.txt");
    serialize_data(entries_data, "index/entries.txt");
}

}  // namespace fts