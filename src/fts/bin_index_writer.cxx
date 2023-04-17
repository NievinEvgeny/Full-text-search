#include <fts/bin_index_writer.hpp>
#include <fts/binary_buffer.hpp>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>

namespace fts {

void BinIndexWriter::write(const fts::Index& index) const
{
    const std::string index_filename = "index/index.txt";

    fts::config_serialize(index_dir_path, config);

    fts::BinaryBuffer docs_data;

    std::unordered_map<uint32_t, uint32_t> id_to_offset;

    uint32_t titles_count = index.docs.size();
    docs_data.write(titles_count);

    for (const auto& [id, title] : index.docs)
    {
        id_to_offset[id] = docs_data.get_data().size();

        uint8_t title_size = title.size() + 1;

        docs_data.write(title_size);
        docs_data.write(title);
    }

    std::ofstream index_file;
    index_file.open(index_filename, std::ios::out | std::ios::binary);

    if (!index_file.is_open())
    {
        throw std::runtime_error{"Can't open file in BinIndexWriter::write function"};
    }

    index_file.write(docs_data.get_data().data(), static_cast<long>(docs_data.get_data().size()));
    index_file.close();
}

}  // namespace fts