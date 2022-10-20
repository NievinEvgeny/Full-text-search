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

class IndexBuilder
{
    struct Index
    {
        DocIdToText docs;

        TermHashToTermToDocsToTermPositions entries;

    } index;

   public:
    void add_document(int document_id, const std::string& text, fts::ConfOptions conf_options);
};

}  // namespace fts