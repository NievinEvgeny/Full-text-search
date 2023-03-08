#pragma once

#include "search_info.h"
#include "index_accessor.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Searcher Searcher;

void searcher_set_accessor(Searcher* self, IndexAccessor* new_accessor);

SearchInfo* searcher_score_calc(Searcher* self, const char* query);

void searcher_print_scores(Searcher* self, SearchInfo* info);

void searcher_delete(Searcher* self);

Searcher* searcher_new(IndexAccessor* accessor);

#ifdef __cplusplus
}
#endif