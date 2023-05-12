#include <fts/bin_index_accessor.hpp>
#include <fts/binary_reader.hpp>
#include <string>
#include <vector>

#include <iostream>

namespace fts {

fts::EntryOffset DictionaryAccessor::retrieve(const std::string& word) const noexcept
{
    bool word_found = false;
    uint32_t entry_offset = 0;

    std::string curr_word;
    uint32_t curr_offset = 0;

    for (const auto& letter : word)
    {
        fts::BinaryReader curr_data{data + curr_offset};

        uint32_t child_count = 0;
        curr_data.read(&child_count, sizeof(child_count));

        bool letter_found = false;
        uint32_t letter_index = 0;

        for (letter_index = 0; letter_index < child_count; letter_index++)
        {
            char curr_child = 0;
            curr_data.read(&curr_child, sizeof(curr_child));

            if (curr_child == letter)
            {
                curr_word += curr_child;
                letter_found = true;
                break;
            }
        }

        if (!letter_found)
        {
            break;
        }

        curr_data.ptr_shift(child_count - (letter_index + 1) + letter_index * sizeof(uint32_t));
        curr_data.read(&curr_offset, sizeof(curr_offset));

        if (curr_word == word)
        {
            word_found = true;
            break;
        }
    }

    if (word_found)
    {
        fts::BinaryReader curr_data{data + curr_offset};

        uint32_t child_count = 0;
        curr_data.read(&child_count, sizeof(child_count));

        curr_data.ptr_shift(child_count + child_count * sizeof(child_count));

        uint8_t is_leaf = 0;
        curr_data.read(&is_leaf, sizeof(is_leaf));

        if (is_leaf)
        {
            curr_data.read(&entry_offset, sizeof(entry_offset));
        }
        else
        {
            word_found = false;
        }
    }

    return fts::EntryOffset{word_found, entry_offset};
}

std::vector<fts::TermInfo> EntriesAccessor::get_term_infos(fts::EntryOffset entry_offset) const noexcept
{
    std::vector<fts::TermInfo> term_infos;

    if (!entry_offset.offset_found)
    {
        return term_infos;
    }

    fts::BinaryReader curr_data{data + entry_offset.offset};

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