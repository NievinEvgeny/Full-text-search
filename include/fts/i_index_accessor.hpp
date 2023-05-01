#pragma once
#include <fts/config.hpp>
#include <string>
#include <vector>

namespace fts {

struct TermInfo
{
    int doc_id;
    int term_frequency;
};

class IndexAccessor_I
{
   public:
    virtual const fts::ConfOptions& get_config() const = 0;
    virtual std::size_t total_docs() const = 0;
    virtual std::string load_document(int document_id) const = 0;
    virtual std::vector<fts::TermInfo> get_term_infos(const std::string& term) const = 0;
    virtual ~IndexAccessor_I() = default;
};

}  // namespace fts