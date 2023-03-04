#include <fts-c/index_accessor.h>
#include <fts-c/term_info.h>
#include <fts-c/searcher.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    // Index accessor
    const char* index_dir_name = "index";

    IndexAccessor* text_index_accessor = text_index_accessor_new(index_dir_name);

    size_t total_docs = index_accessor_total_docs(text_index_accessor);
    char* line_from_doc = index_accessor_load_document(text_index_accessor, 1);
    TermInfo* term_infos = index_accessor_get_term_infos(text_index_accessor, "clint");

    printf("total docs = %ld\n", total_docs);
    printf("harry = %s\n", line_from_doc);
    for (int i = 0; i < 2; i++)
    {
        printf("doc id = %d, term freq = %d\n", term_infos[i].doc_id, term_infos[i].term_frequency);
    }

    // Searcher
    const char* query = "harry potter";
    Searcher* searcher = searcher_new(text_index_accessor);
    SearchInfo* search_info = searcher_score_calc(searcher, query);

    searcher_print_scores(searcher, search_info);

    free(search_info->docs_scores);
    free(search_info);
    searcher_delete(searcher);
    free(term_infos);
    free(line_from_doc);
    index_accessor_delete(text_index_accessor);
}