#include <fts/trie.hpp>
#include <fts/binary_buffer.hpp>
#include <string>
#include <memory>
#include <stack>
#include <queue>
#include <fstream>

namespace fts {

void Trie::add(const std::string& word, uint32_t offset)
{
    fts::TrieNode* curr_node = root.get();

    for (const char& letter : word)
    {
        if (curr_node->childs.find(letter) == curr_node->childs.end())
        {
            curr_node->childs.emplace(std::make_pair(letter, std::make_unique<fts::TrieNode>()));
        }
        curr_node = curr_node->childs.at(letter).get();
    }
    curr_node->is_leaf = true;
    curr_node->entry_offset = offset;
}

static uint32_t calc_offset(const fts::TrieNode& node)
{
    return node.node_pos_offset + sizeof(uint32_t) + node.childs.size() + sizeof(uint32_t) * node.childs.size()
        + sizeof(bool) + static_cast<uint32_t>(node.is_leaf * sizeof(uint32_t));
}

static std::queue<fts::TrieNode*> store_nodes_in_order(fts::TrieNode* root)
{
    std::stack<std::stack<fts::TrieNode*>> childs_stack;
    std::queue<fts::TrieNode*> nodes;

    nodes.emplace(root);

    while (!childs_stack.empty() || !nodes.back()->childs.empty())
    {
        if (!nodes.back()->childs.empty())
        {
            childs_stack.emplace();
            for (const auto& [letter, child] : nodes.back()->childs)
            {
                childs_stack.top().push(child.get());
            }
        }

        childs_stack.top().top()->node_pos_offset = fts::calc_offset(*nodes.back());
        nodes.emplace(childs_stack.top().top());
        childs_stack.top().pop();

        if (childs_stack.top().empty())
        {
            childs_stack.pop();
        }
    }

    return nodes;
}

static void write_to_bin_buf(std::queue<fts::TrieNode*> nodes, fts::BinaryBuffer& b_data)
{
    while (!nodes.empty())
    {
        b_data.write(static_cast<uint32_t>(nodes.front()->childs.size()));

        for (const auto& [letter, child] : nodes.front()->childs)
        {
            b_data.write(letter);
        }

        for (const auto& [letter, child] : nodes.front()->childs)
        {
            b_data.write(child->node_pos_offset);
        }

        b_data.write(nodes.front()->is_leaf);

        if (nodes.front()->is_leaf)
        {
            b_data.write(nodes.front()->entry_offset);
        }

        nodes.pop();
    }
}

void Trie::serialize(fts::BinaryBuffer& b_data) const
{
    std::queue<fts::TrieNode*> nodes;

    nodes = fts::store_nodes_in_order(root.get());
    fts::write_to_bin_buf(nodes, b_data);
}

}  // namespace fts