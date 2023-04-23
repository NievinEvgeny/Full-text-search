#include <fts/trie.hpp>
#include <string>
#include <memory>

namespace fts {

void Trie::add(const std::string& word, uint32_t offset)
{
    fts::TrieNode curr_node = std::move(root);

    for (const char& letter : word)
    {
        if (curr_node.childs.find(letter) == curr_node.childs.end())
        {
            curr_node.childs.emplace(std::make_pair(letter, std::make_unique<fts::TrieNode>()));
        }
        curr_node = std::move(*(curr_node.childs.at(letter)));
    }
    curr_node.is_leaf = true;
    curr_node.entry_offset = offset;
}

}  // namespace fts