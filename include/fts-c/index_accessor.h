#pragma once

#include "term_info.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct IndexAccessor IndexAccessor;

size_t index_accessor_total_docs(const IndexAccessor* self);

char* index_accessor_load_document(IndexAccessor* self, int document_id);

TermInfo* index_accessor_get_term_infos(IndexAccessor* self, const char* term);

void index_accessor_delete(IndexAccessor* self);

IndexAccessor* text_index_accessor_new(const char* index_dir_name);

#ifdef __cplusplus
}
#endif