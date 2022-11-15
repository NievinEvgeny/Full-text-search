#pragma once
#include <fts/parser.hpp>
#include <unordered_map>
#include <unordered_set>
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
    std::unordered_set<int> doc_ids;

    std::unordered_map<int, int> term_frequency;

    int doc_frequency;
};

class SearcherBuf
{
    std::unordered_map<std::string, fts::TermAttributes> terms;

    std::vector<DocScore> doc_scores;

    std::vector<int> all_doc_ids;

    void deserialize_index(const std::string& query, const std::string& index_path, const fts::ConfOptions& config);

    void store_doc_ids(const std::string& index_path);

    void score_calc();

    void score_sort();

   public:
    SearcherBuf(const std::string& query, const std::string& index_path, const fts::ConfOptions& config)
    {
        deserialize_index(query, index_path, config);
        store_doc_ids(index_path);
        score_calc();
        score_sort();
    }

    const std::vector<DocScore>& get_scores()
    {
        return this->doc_scores;
    }
};

}  // namespace fts