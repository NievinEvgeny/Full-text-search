#pragma once
#include <fts/index_accessor.hpp>
#include <string>
#include <vector>

namespace fts {

struct DocScore
{
    int doc_id;

    double score;
};

struct SearchInfo
{
    std::vector<fts::DocScore> docs_scores;

    std::size_t num_of_terms;
};

class Searcher
{
    fts::IndexAccessor& accessor;

   public:
    explicit Searcher(fts::IndexAccessor& access) : accessor(access)
    {
    }

    fts::SearchInfo score_calc(const std::string& query);

    void print_scores(const fts::SearchInfo& search_info);
};

}  // namespace fts