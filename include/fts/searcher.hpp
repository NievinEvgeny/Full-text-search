#pragma once
#include <unordered_map>
#include <string>
#include <vector>

namespace fts {

struct DocScore
{
    int doc_id;
    double score;
};

struct TermAttributes
{
    std::vector<int> doc_ids;

    std::unordered_map<int, int> term_frequency;

    int doc_frequency;
};

class SearcherBuf
{
    std::unordered_map<std::string, fts::TermAttributes> terms;

    std::vector<DocScore> doc_score;

    std::vector<int> all_doc_ids;

   public:
    void deserialize_index(const std::string& query, const std::string& index_path);

    void store_doc_ids(const std::string& index_path);
};

}  // namespace fts