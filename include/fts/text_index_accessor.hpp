#pragma once
#include <fts/config.hpp>
#include <fts/i_index_accessor.hpp>
#include <string>
#include <vector>

namespace fts {

class TextIndexAccessor : public IndexAccessor_I
{
    const fts::ConfOptions config;

    const std::string index_path;

    const std::size_t num_of_docs;

    static std::size_t find_num_of_docs(const std::string& index_dir_name);

   public:
    explicit TextIndexAccessor(std::string index_dir_name)
        : config(fts::parse_config(index_dir_name + "/Config.json")),
          index_path(std::move(index_dir_name)),
          num_of_docs(fts::TextIndexAccessor::find_num_of_docs(index_path))
    {
    }

    const fts::ConfOptions& get_config() const override
    {
        return config;
    }

    std::size_t total_docs() const override
    {
        return num_of_docs;
    }

    std::string load_document(int document_id) const override;

    std::vector<fts::TermInfo> get_term_infos(const std::string& term) const override;
};

}  // namespace fts