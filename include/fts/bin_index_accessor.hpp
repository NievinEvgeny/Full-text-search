#pragma once
#include <fts/config.hpp>
#include <fts/i_index_accessor.hpp>
#include <fts/binary_reader.hpp>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

namespace fts {

struct EntryOffset
{
    bool offset_found;
    uint32_t offset;
};

class DictionaryAccessor
{
    const char* data;

   public:
    explicit DictionaryAccessor(const char* dictionary_data) : data(dictionary_data)
    {
    }

    fts::EntryOffset retrieve(const std::string& word) const noexcept;
};

class EntriesAccessor
{
    const char* data;

   public:
    explicit EntriesAccessor(const char* entries_data) : data(entries_data)
    {
    }

    std::vector<fts::TermInfo> get_term_infos(fts::EntryOffset offset) const noexcept;
};

class DocumentsAccessor
{
    const char* data;

   public:
    explicit DocumentsAccessor(const char* document_data) : data(document_data)
    {
    }

    std::string load_document(std::size_t offset) const noexcept;

    std::size_t total_docs() const noexcept;
};

class BinIndexAccessor : public IndexAccessor_I
{
    const fts::ConfOptions config;

    const fts::DictionaryAccessor dictionary;
    const fts::EntriesAccessor entries;
    const fts::DocumentsAccessor docs;

    static const char* find_section_offset(const char* data, const char* section_name);

   public:
    BinIndexAccessor(const std::string& index_dir_name, const char* data)
        : config(fts::parse_config(index_dir_name + "/Config.json")),
          dictionary(find_section_offset(data, "dictionary")),
          entries(find_section_offset(data, "entries")),
          docs(find_section_offset(data, "docs"))
    {
    }

    const fts::ConfOptions& get_config() const override
    {
        return config;
    }

    std::size_t total_docs() const override
    {
        return docs.total_docs();
    }

    std::string load_document(uint32_t document_offset) const override
    {
        return docs.load_document(document_offset);
    }

    std::vector<fts::TermInfo> get_term_infos(const std::string& term) const override
    {
        return entries.get_term_infos(dictionary.retrieve(term));
    }
};

}  // namespace fts