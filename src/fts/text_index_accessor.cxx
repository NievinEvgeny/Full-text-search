#include <fts/text_index_accessor.hpp>
#include <fts/word_hash.hpp>
#include <fts/query_parser.hpp>
#include <filesystem>
#include <string>
#include <vector>
#include <iostream>

namespace fts {

std::size_t TextIndexAccessor::find_num_of_docs(const std::string& index_dir_name)
{
    return std::count_if(
        std::filesystem::directory_iterator(index_dir_name + "/docs/"),
        std::filesystem::directory_iterator{},
        [&](const std::filesystem::directory_entry& path) { return path.is_regular_file(); });
}

std::string TextIndexAccessor::load_document(int document_id) const
{
    const std::string string_doc_id = std::to_string(document_id);

    std::ifstream doc(index_path + "/docs/" += string_doc_id);

    if (!doc.is_open())
    {
        throw std::runtime_error{"Can't open file in TextIndexAccessor::load_document function"};
    }

    std::string line_from_doc;
    std::getline(doc, line_from_doc);
    doc.close();

    return line_from_doc;
}

std::vector<fts::TermInfo> TextIndexAccessor::get_term_infos(const std::string& term) const
{
    const std::string word_hash = fts::get_word_hash(term);

    std::vector<fts::TermInfo> term_infos;

    std::ifstream entrie_doc(index_path + "/entries/" += word_hash);

    if (!entrie_doc.is_open())
    {
        return term_infos;
    }

    std::string line_from_entrie;

    while (std::getline(entrie_doc, line_from_entrie))
    {
        if (line_from_entrie.find(term) == std::string::npos)
        {
            continue;
        }

        const std::vector<std::string> line_tokens = string_tokenize(line_from_entrie);

        for (auto iter = line_tokens.begin() + 2; iter != line_tokens.end(); iter++)
        {
            int doc_id = std::stoi(*iter);
            iter++;

            int term_freq = std::stoi(*iter);
            iter += term_freq;

            term_infos.emplace_back(fts::TermInfo{doc_id, term_freq});
        }

        break;
    }

    entrie_doc.close();
    return term_infos;
}

}  // namespace fts