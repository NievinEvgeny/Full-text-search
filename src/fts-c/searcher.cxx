#include <fts-c/search_info.h>
#include <fts-c/doc_score.h>
#include <fts-c/index_accessor.h>
#include <fts-c/searcher.h>
#include <fts/searcher.hpp>
#include <fts/index_accessor.hpp>
#include <string>
#include <vector>

void searcher_set_accessor(Searcher* self, IndexAccessor* new_accessor)
{
    auto* index_accessor = reinterpret_cast<fts::IndexAccessor*>(new_accessor);
    auto* searcher = reinterpret_cast<fts::Searcher*>(self);
    searcher->set_accessor(*index_accessor);
}

SearchInfo* searcher_score_calc(Searcher* self, const char* query)
{
    const std::string str_query = query;

    auto* searcher = reinterpret_cast<fts::Searcher*>(self);
    const fts::SearchInfo search_res = searcher->score_calc(str_query);

    auto* doc_scores = new DocScore[search_res.docs_scores.size()];

    for (size_t i = 0; i < search_res.docs_scores.size(); i++)
    {
        doc_scores[i] = {search_res.docs_scores.at(i).doc_id, search_res.docs_scores.at(i).score};
    }

    auto* conv_search_res = new SearchInfo{doc_scores, search_res.num_of_terms, search_res.docs_scores.size()};

    return conv_search_res;
}

void searcher_print_scores(Searcher* self, SearchInfo* info)
{
    std::vector<fts::DocScore> docs_scores;
    docs_scores.reserve(info->num_of_docs);

    for (size_t i = 0; i < info->num_of_docs; i++)
    {
        docs_scores.push_back({info->docs_scores[i].doc_id, info->docs_scores[i].score});
    }

    fts::SearchInfo search_info = {docs_scores, info->num_of_terms};

    auto* searcher = reinterpret_cast<fts::Searcher*>(self);
    searcher->print_scores(search_info);
}

void searcher_delete(Searcher* self)
{
    const auto* searcher = reinterpret_cast<fts::Searcher*>(self);
    delete searcher;
}

Searcher* searcher_new(IndexAccessor* accessor)
{
    auto* index_accessor = reinterpret_cast<fts::IndexAccessor*>(accessor);
    return reinterpret_cast<Searcher*>(static_cast<fts::Searcher*>(new fts::Searcher(*index_accessor)));
}