#include <fts/bin_index_accessor.hpp>
#include <fts/binary_reader.hpp>
#include <string>
#include <vector>

#include <iostream>

namespace fts {

uint32_t DictionaryAccessor::retrieve(const std::string& word) const noexcept
{
    // TODO
    return 0;
}

std::vector<fts::TermInfo> EntriesAccessor::get_term_infos(std::size_t entry_offset) const noexcept
{
    fts::BinaryReader curr_data{data + entry_offset};

    std::vector<fts::TermInfo> term_infos;

    uint32_t docs_num = 0;
    curr_data.read(&docs_num, sizeof(docs_num));

    for (uint32_t i = 0; i < docs_num; i++)
    {
        uint32_t doc_id = 0;
        curr_data.read(&doc_id, sizeof(doc_id));

        uint32_t term_freq = 0;
        curr_data.read(&term_freq, sizeof(term_freq));
        curr_data.ptr_shift(term_freq * sizeof(uint32_t));

        term_infos.emplace_back(fts::TermInfo{doc_id, term_freq});
    }

    return term_infos;
}

std::size_t DocumentsAccessor::total_docs() const noexcept
{
    fts::BinaryReader curr_data{data};

    uint32_t docs_num = 0;
    curr_data.read(&docs_num, sizeof(docs_num));
    return docs_num;
}

std::string DocumentsAccessor::load_document(std::size_t book_offset) const noexcept
{
    fts::BinaryReader curr_data{data + book_offset};

    uint8_t size_book_name = 0;
    curr_data.read(&size_book_name, sizeof(size_book_name));

    std::vector<char> book_name;
    book_name.reserve(size_book_name - 1);
    curr_data.read(book_name.data(), size_book_name - 1);

    return std::string{book_name.data(), static_cast<std::size_t>(size_book_name - 1)};
}

const char* BinIndexAccessor::find_section_offset(const char* data, const char* section_name)
{
    fts::BinaryReader curr_data{data};

    uint8_t section_count = 0;
    curr_data.read(&section_count, sizeof(section_count));

    for (uint8_t i = 0; i < section_count; i++)
    {
        uint8_t section_length = 0;
        curr_data.read(&section_length, sizeof(section_length));

        std::vector<char> curr_name;
        curr_name.reserve(section_length - 1);
        curr_data.read(curr_name.data(), section_length - 1);

        if (!std::strncmp(curr_name.data(), section_name, section_length - 1))
        {
            uint32_t section_offset = 0;
            curr_data.read(&section_offset, sizeof(section_offset));

            return data + section_offset;
        }

        curr_data.ptr_shift(sizeof(uint32_t));
    }

    return nullptr;
}

}  // namespace fts