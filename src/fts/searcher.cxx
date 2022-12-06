#include <fts/searcher.hpp>
#include <fts/query_parser.hpp>
#include <fts/word_hash.hpp>
#include <filesystem>
#include <string>
#include <cmath>
#include <vector>
#include <iostream>

namespace fts {

std::size_t TextIndexAccessor::find_num_of_docs(const std::string& index_dir_name)
{
    return std::count_if(
        std::filesystem::directory_iterator(index_dir_name + "/docs/"),
        std::filesystem::directory_iterator{},
        [&](const std::filesystem::directory_entry& path) { return path.is_regular_file(); });
}

std::string TextIndexAccessor::load_document(int document_id)
{
    const std::string string_doc_id = std::to_string(document_id);

    std::ifstream doc(index_path + "/docs/" += string_doc_id);

    if (!doc.is_open())
    {
        throw std::runtime_error{"Can't open file in TextIndexAccessor::load_document function"};
    }

    std::string line_from_doc;
    std::getline(doc, line_from_doc);
    doc.close();

    return line_from_doc;
}

std::vector<fts::TermInfo> TextIndexAccessor::get_term_infos(const std::string& term)
{
    const std::string word_hash = fts::get_word_hash(term);

    const std::size_t min_capacity_term_infos = 5;
    std::vector<fts::TermInfo> term_infos;
    term_infos.reserve(min_capacity_term_infos);

    std::ifstream entrie_doc(index_path + "/entries/" += word_hash);

    if (!entrie_doc.is_open())
    {
        return term_infos;
    }

    std::string line_from_entrie;

    while (std::getline(entrie_doc, line_from_entrie))
    {
        if (line_from_entrie.find(term) == std::string::npos)
        {
            continue;
        }

        const std::vector<std::string> line_tokens = string_tokenize(line_from_entrie);

        for (auto iter = line_tokens.begin() + 2; iter != line_tokens.end(); iter++)
        {
            int doc_id = std::stoi(*iter);
            iter++;

            int term_freq = std::stoi(*iter);
            iter += term_freq;

            term_infos.push_back({doc_id, term_freq});
        }

        break;
    }
    entrie_doc.close();
    return term_infos;
}

static void score_sort(std::vector<fts::DocScore>& doc_scores)
{
    std::sort(doc_scores.begin(), doc_scores.end(), [](const fts::DocScore& a, const fts::DocScore& b) {
        return (a.score == b.score) ? (a.doc_id < b.doc_id) : (a.score > b.score);
    });
}

std::vector<DocScore> Searcher::score_calc(const std::string& query)
{
    std::unordered_map<int, double> doc_scores_map;

    const std::vector<fts::Ngram> ngrams = parse_query(accessor.get_config(), query);

    for (const auto& ngram : ngrams)
    {
        std::vector<fts::TermInfo> term_infos = accessor.get_term_infos(ngram.word);

        for (const auto& term_info : term_infos)
        {
            doc_scores_map[term_info.doc_id] += std::sqrt(term_info.term_frequency)
                * std::log(static_cast<double>(accessor.total_docs()) / static_cast<double>(term_infos.size()));
        }
    }

    std::vector<DocScore> doc_scores;
    doc_scores.reserve(doc_scores_map.size());

    for (const auto& [doc_id, score] : doc_scores_map)
    {
        doc_scores.push_back({doc_id, score});
    }

    score_sort(doc_scores);

    return doc_scores;
}

void Searcher::print_scores(const std::vector<DocScore>& doc_scores)
{
    if (doc_scores.empty())
    {
        return;
    }

    const int num_of_ranges = 3;
    const std::array<int, num_of_ranges> ranges{30, 150, 350};

    for (const auto& range : ranges)
    {
        // const size_t terms_max_num = 20;
        //  const size_t terms_num = std::min(terms.size(), terms_max_num);
        const size_t terms_num = 10;  // TODO (get num of terms)

        double min_score
            = static_cast<double>(terms_num) * ((std::log(static_cast<double>(accessor.total_docs()) / range)));

        if (doc_scores.at(0).score > min_score)
        {
            for (size_t i = 0; doc_scores.at(i).score > min_score; i++)
            {
                std::cout << doc_scores.at(i).doc_id << ' ' << doc_scores.at(i).score << '\n';
            }
            return;
        }
    }

    std::cout << "Nothing was found for your query" << '\n';
}

}  // namespace fts