#pragma once
#include <fts/i_index_accessor.hpp>
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
    fts::IndexAccessor_I& accessor;

   public:
    explicit Searcher(fts::IndexAccessor_I& access) : accessor(access)
    {
    }

    void set_accessor(fts::IndexAccessor_I& new_access)
    {
        accessor = new_access;
    }

    fts::SearchInfo score_calc(const std::string& query);

    void print_scores(const fts::SearchInfo& search_info);
};

}  // namespace fts