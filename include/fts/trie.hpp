#pragma once
#include <string>
#include <unordered_map>
#include <memory>

namespace fts {

struct TrieNode
{
    bool is_leaf = false;
    std::unordered_map<char, std::unique_ptr<fts::TrieNode>> childs;
    uint32_t entry_offset;
};

class Trie
{
    fts::TrieNode root;

   public:
    void add(const std::string& word, uint32_t offset);

    // void serialize(const std::string& filename) const;
};

}  // namespace fts