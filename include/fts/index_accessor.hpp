#pragma once
#include <fts/conf_parser.hpp>
#include <string>
#include <vector>

namespace fts {

struct TermInfo
{
    int doc_id;

    int term_frequency;
};

class IndexAccessor
{
   public:
    virtual const fts::ConfOptions& get_config() const = 0;
    virtual std::size_t total_docs() const = 0;
    virtual std::string load_document(int document_id) = 0;
    virtual std::vector<fts::TermInfo> get_term_infos(const std::string& term) = 0;
    virtual ~IndexAccessor() = default;
};

class TextIndexAccessor : public IndexAccessor
{
    const std::string index_path;

    const fts::ConfOptions config;

    const std::size_t num_of_docs;

    static std::size_t find_num_of_docs(const std::string& index_dir_name);

   public:
    TextIndexAccessor(std::string index_dir_name, fts::ConfOptions conf)
        : index_path(std::move(index_dir_name)),
          config(std::move(conf)),
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

    std::string load_document(int document_id) override;

    std::vector<fts::TermInfo> get_term_infos(const std::string& term) override;
};

}  // namespace fts