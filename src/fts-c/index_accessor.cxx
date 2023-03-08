#include <fts-c/index_accessor.h>
#include <fts-c/term_info.h>
#include <fts/index_accessor.hpp>
#include <fts/conf_parser.hpp>
#include <string>
#include <vector>
#include <cstdio>

size_t index_accessor_total_docs(const IndexAccessor* self)
{
    const auto* index_accessor = reinterpret_cast<const fts::IndexAccessor*>(self);
    return index_accessor->total_docs();
}

char* index_accessor_load_document(IndexAccessor* self, int document_id)
{
    auto* index_accessor = reinterpret_cast<fts::IndexAccessor*>(self);
    std::string line_from_doc = index_accessor->load_document(document_id);

    char* conv_line = new char[line_from_doc.size()];
    strncpy(conv_line, line_from_doc.c_str(), line_from_doc.size());

    return conv_line;
}

TermInfo* index_accessor_get_term_infos(IndexAccessor* self, const char* term)
{
    auto* index_accessor = reinterpret_cast<fts::IndexAccessor*>(self);
    std::vector<fts::TermInfo> term_infos = index_accessor->get_term_infos(term);

    auto* conv_term_infos = new TermInfo[term_infos.size()];

    for (size_t i = 0; i < term_infos.size(); i++)
    {
        conv_term_infos[i] = {term_infos.at(i).doc_id, term_infos.at(i).term_frequency};
    }

    return conv_term_infos;
}

void index_accessor_delete(IndexAccessor* self)
{
    const auto* index_accessor = reinterpret_cast<fts::IndexAccessor*>(self);
    delete index_accessor;
}

IndexAccessor* text_index_accessor_new(const char* index_dir_name)
{
    std::string index_dir = index_dir_name;
    return reinterpret_cast<IndexAccessor*>(static_cast<fts::IndexAccessor*>(
        new fts::TextIndexAccessor(index_dir, fts::parse_config(index_dir + "/Config.json"))));
}
