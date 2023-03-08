#pragma once

#include "doc_score.h"
#include <stdio.h>

typedef struct
{
    DocScore* docs_scores;
    size_t num_of_terms;
    size_t num_of_docs;
} SearchInfo;