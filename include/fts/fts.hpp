#pragma once
#include <iostream>
#include <vector>

namespace fts {

int parse_config(
    const std::string& config_filename,
    std::string& text,
    std::vector<std::string>& stop_words,
    int ngram_min_length,
    int ngram_max_length);

void run_parser(const std::string& filename);

}  // namespace fts