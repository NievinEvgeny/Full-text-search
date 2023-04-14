#pragma once
#include <fts/config.hpp>
#include <vector>
#include <string>

namespace fts {

struct Ngram
{
    int index;
    std::string word;
};

std::vector<std::string> string_tokenize(const std::string& text);

std::vector<fts::Ngram> parse_query(const fts::ConfOptions& config, const std::string& text);

}  // namespace fts