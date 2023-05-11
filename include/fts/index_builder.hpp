#pragma once
#include <fts/config.hpp>
#include <unordered_map>
#include <string>
#include <vector>

namespace fts {

using Pos = int;
using DocId = uint32_t;
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
        return index;
    }

    void add_document(uint32_t document_id, const std::string& text);

    void parse_csv(const std::string& filename);
};

}  // namespace fts