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
    const std::vector<fts::DocScore> docs_scores;
    const std::size_t num_of_terms;
};

class Searcher
{
    fts::IndexAccessor& accessor;

   public:
    explicit Searcher(fts::IndexAccessor& access) : accessor(access)
    {
    }

    void set_accessor(fts::IndexAccessor& new_access)
    {
        accessor = new_access;
    }

    fts::SearchInfo score_calc(const std::string& query);

    void print_scores(const fts::SearchInfo& search_info);
};

}  // namespace fts