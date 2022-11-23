#include <fts/searcher.hpp>
#include <fts/parser.hpp>
#include <string>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <array>
#include <iostream>

namespace fts {

void IndexAccessor::deserialize_index(const std::string& index_path, const std::vector<fts::Ngram>& ngrams)
{
    for (const auto& ngram : ngrams)
    {
        const std::string word_hash = fts::get_word_hash(ngram.word);

        const std::filesystem::path entrie_doc_path(index_path + "/entries/" += word_hash);
        std::ifstream entrie_doc(index_path + "/entries/" += word_hash);

        if (!std::filesystem::exists(entrie_doc_path))
        {
            continue;
        }
        if (!entrie_doc.is_open())
        {
            throw std::runtime_error{"Can't open file in IndexAccessor::deserialize_index function"};
        }

        std::string line_from_entrie;

        while (std::getline(entrie_doc, line_from_entrie))
        {
            if (line_from_entrie.find(ngram.word) == std::string::npos)
            {
                continue;
            }

            const std::vector<std::string> line_tokens = string_tokenization(line_from_entrie);

            this->terms[ngram.word].doc_frequency = std::stoi(*(line_tokens.begin() + 1));

            for (auto iter = line_tokens.begin() + 2; iter != line_tokens.end(); iter++)
            {
                int doc_id = std::stoi(*iter);
                iter++;

                int term_freq = std::stoi(*iter);
                iter += term_freq;

                this->terms[ngram.word].doc_ids.insert(doc_id);
                this->terms[ngram.word].term_frequency[doc_id] = term_freq;
            }

            break;
        }

        entrie_doc.close();
    }
}

void IndexAccessor::store_doc_ids(const std::string& index_path)
{
    for (const auto& doc : std::filesystem::directory_iterator(index_path + "/docs/"))
    {
        if (doc.is_regular_file())
        {
            this->all_doc_ids.push_back(std::stoi(doc.path().stem().string()));
        }
    }
}

void IndexAccessor::score_calc()
{
    for (const auto& doc_id : this->all_doc_ids)
    {
        fts::DocScore temp_doc_score = {doc_id, 0};

        for (const auto& [term, attributes] : this->terms)
        {
            if (attributes.doc_ids.find(doc_id) != attributes.doc_ids.end())
            {
                temp_doc_score.score += attributes.term_frequency.at(doc_id)
                    * log(static_cast<double>(this->all_doc_ids.size())
                          / static_cast<double>(attributes.doc_frequency));
            }
        }

        this->doc_scores.push_back(temp_doc_score);
    }
}

void IndexAccessor::score_sort()
{
    std::sort(this->doc_scores.begin(), this->doc_scores.end(), [](const fts::DocScore& a, const fts::DocScore& b) {
        return (a.score == b.score) ? (a.doc_id < b.doc_id) : (a.score > b.score);
    });
}

bool IndexAccessor::print_scores_in_range(const int& range)
{
    const size_t terms_max_num = 20;
    const size_t terms_num = std::min(this->terms.size(), terms_max_num);

    double min_score = static_cast<double>(terms_num) * ((log(static_cast<double>(this->all_doc_ids.size()) / range)));

    if (this->doc_scores.at(0).score > min_score)
    {
        for (size_t i = 0; this->doc_scores.at(i).score > min_score; i++)
        {
            std::cout << this->doc_scores.at(i).doc_id << ' ' << this->doc_scores.at(i).score << '\n';
        }
        return true;
    }
    return false;
}

void IndexAccessor::print_scores()
{
    if (this->doc_scores.empty())
    {
        return;
    }

    const int num_of_ranges = 3;
    const std::array<int, num_of_ranges> ranges = {50, 200, 500};
    bool range_found = false;

    for (const auto& range : ranges)
    {
        if ((range_found = print_scores_in_range(range)) == 1)
        {
            break;
        }
    }

    if (!range_found)
    {
        std::cout << "Nothing was found for your query" << '\n';
    }
}

}  // namespace fts