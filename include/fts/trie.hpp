#pragma once
#include <fts/binary_buffer.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <fstream>

namespace fts {

struct TrieNode
{
    bool is_leaf = false;
    std::unordered_map<char, std::unique_ptr<fts::TrieNode>> childs;
    uint32_t entry_offset;
    uint32_t node_pos_offset;
};

class Trie
{
    std::unique_ptr<fts::TrieNode> root = std::make_unique<fts::TrieNode>();

   public:
    void add(const std::string& word, uint32_t offset);

    void serialize(fts::BinaryBuffer& b_data) const;
};

}  // namespace fts