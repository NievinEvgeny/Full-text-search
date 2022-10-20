#include <fts/parser.hpp>
#include <unordered_map>
#include <string>
#include <vector>

namespace fts {

class IndexBuilder
{
    struct Index
    {
        std::unordered_map<int, std::string> docs;

        // clang-format off
        std::unordered_map
        <std::string, std::unordered_map
        <std::string, std::unordered_map
        <int, std::vector<int>
        >>> entries;
        // clang-format on

    } index;

   public:
    void add_document(int document_id, const std::string& text, fts::ConfOptions conf_options);
};

}  // namespace fts