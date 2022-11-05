#pragma once
#include <fts/parser.hpp>
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

   public:
    const fts::Index& get_index()
    {
        return index;
    }

    void add_document(int document_id, const std::string& text, const fts::ConfOptions& conf_options);
};

class TextIndexWriter
{
    std::string index_dir_path;

   public:
    explicit TextIndexWriter(std::string path) : index_dir_path(std::move(path))
    {
    }

    void write(const fts::Index& index);
};

}  // namespace fts