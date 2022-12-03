#pragma once
#include <fts/conf_parser.hpp>
#include <unordered_map>
#include <string>
#include <vector>

namespace fts {

using Pos = int;
using DocId = int;
using TermPositions = std::vector<Pos>;
using IdToPositions = std::unordered_map<DocId, TermPositions>;
using Term = std::string;
using TermToDocsToTermPositions = std::unordered_map<Term, IdToPositions>;
using TermHash = std::string;
using TermHashToTermToDocsToTermPositions = std::unordered_map<TermHash, TermToDocsToTermPositions>;

using Text = std::string;
using DocIdToText = std::unordered_map<DocId, Text>;

struct Index
{
    DocIdToText docs;

    TermHashToTermToDocsToTermPositions entries;
};

class IndexBuilder
{
    fts::Index index;

    const fts::ConfOptions config;

   public:
    explicit IndexBuilder(fts::ConfOptions conf) : config(std::move(conf))
    {
    }

    const fts::Index& get_index()
    {
        return this->index;
    }

    void add_document(int document_id, const std::string& text);

    void parse_csv(const std::string& filename);
};

class TextIndexWriter
{
    const std::string index_dir_path;

    const fts::ConfOptions config;

   public:
    TextIndexWriter(std::string path, fts::ConfOptions conf) : index_dir_path(std::move(path)), config(std::move(conf))
    {
    }

    void write(const fts::Index& index);
};

}  // namespace fts