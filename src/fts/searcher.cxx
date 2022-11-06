#include <fts/searcher.hpp>
#include <fts/parser.hpp>
#include <PicoSHA2/picosha2.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <cmath>

namespace fts {

void SearcherBuf::deserialize_index(const std::string& query, const std::string& index_path)
{
    const int hash_required_len = 6;

    const nlohmann::json parsed_conf = fts::parse_config(index_path + "/Config.json");
    const fts::ConfOptions conf_options = fts::parse_json_struct(parsed_conf);

    const std::vector<fts::Ngram> ngrams = fts::parse_query(conf_options, query);

    for (const auto& ngram : ngrams)
    {
        std::string text_hash = picosha2::hash256_hex_string(ngram.word);
        text_hash.erase(hash_required_len);

        const std::filesystem::path entrie_doc_path(index_path + "/entries/" += text_hash);
        std::ifstream entrie_doc(index_path + "/entries/" += text_hash);

        if (!std::filesystem::exists(entrie_doc_path))
        {
            continue;
        }
        if (!entrie_doc.is_open())
        {
            throw std::runtime_error{"Can't open file in SearcherBuf::deserialize_index function"};
        }

        std::string line_from_entrie;

        while (std::getline(entrie_doc, line_from_entrie))
        {
            if (line_from_entrie.find(ngram.word) == std::string::npos)
            {
                continue;
            }

            const std::vector<std::string> line_tokens = string_tokenization(line_from_entrie);

            terms[ngram.word].doc_frequency = std::stoi(*(line_tokens.begin() + 1));

            for (auto iter = line_tokens.begin() + 2; iter != line_tokens.end(); iter++)
            {
                int doc_id = std::stoi(*iter);
                iter++;

                int term_freq = std::stoi(*iter);
                iter += term_freq;

                terms[ngram.word].doc_ids.insert(doc_id);
                terms[ngram.word].term_frequency[doc_id] = term_freq;
            }

            break;
        }

        entrie_doc.close();
    }
}

void SearcherBuf::store_doc_ids(const std::string& index_path)
{
    for (const auto& doc : std::filesystem::directory_iterator(index_path + "/docs/"))
    {
        if (doc.is_regular_file())
        {
            all_doc_ids.push_back(std::stoi(doc.path().stem().string()));
        }
    }
}

void SearcherBuf::score_calc()
{
    for (const auto& doc_id : all_doc_ids)
    {
        fts::DocScore temp_doc_score = {doc_id, 0};

        for (const auto& [term, attributes] : terms)
        {
            if (attributes.doc_ids.find(doc_id) != attributes.doc_ids.end())
            {
                temp_doc_score.score += attributes.term_frequency.at(doc_id)
                    * log(static_cast<double>(all_doc_ids.size()) / static_cast<double>(attributes.doc_frequency));
            }
        }

        doc_scores.push_back(temp_doc_score);
    }
}

void SearcherBuf::score_sort()
{
    std::sort(doc_scores.begin(), doc_scores.end(), [](const fts::DocScore& a, const fts::DocScore& b) {
        if (a.score == b.score)
        {
            return a.doc_id < b.doc_id;
        }
        return a.score > b.score;
    });
}

const std::vector<DocScore>& SearcherBuf::get_scores(const std::string& query, const std::string& index_path)
{
    deserialize_index(query, index_path);
    store_doc_ids(index_path);
    score_calc();
    score_sort();
    return doc_scores;
}

}  // namespace fts