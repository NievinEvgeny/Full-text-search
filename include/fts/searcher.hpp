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

class IndexAccessor
{
    std::unordered_map<std::string, fts::TermAttributes> terms;

    std::vector<DocScore> doc_scores;

    std::vector<int> all_doc_ids;

    void deserialize_index(const std::string& index_path, const std::vector<fts::Ngram>& ngrams);

    void store_doc_ids(const std::string& index_path);

    void score_calc();

    void score_sort();

   public:
    IndexAccessor(const std::string& index_path, const std::vector<fts::Ngram>& ngrams)
    {
        deserialize_index(index_path, ngrams);
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