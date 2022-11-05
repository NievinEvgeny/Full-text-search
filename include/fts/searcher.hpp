#pragma once
#include <unordered_map>
#include <string>
#include <vector>

namespace fts {

struct TermAttributes
{
    std::vector<int> doc_ids;

    std::unordered_map<int, int> term_frequency;

    int doc_frequency;
};

class SearcherBuf
{
    std::unordered_map<std::string, fts::TermAttributes> terms;

    std::unordered_map<int, double> doc_score;

    int num_of_docs;

   public:
    void deserialize_index(const std::string& query, const std::string& index_path);
};

}  // namespace fts