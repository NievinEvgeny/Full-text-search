#include <fts/word_hash.hpp>
#include <PicoSHA2/picosha2.h>
#include <string>

namespace fts {

std::string get_word_hash(const std::string& word)
{
    const int hash_required_len = 6;
    std::string word_hash = picosha2::hash256_hex_string(word);
    return word_hash.erase(hash_required_len);
}

}  // namespace fts