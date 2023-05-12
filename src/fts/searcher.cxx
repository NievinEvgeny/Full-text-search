#include <fts/searcher.hpp>
#include <fts/query_parser.hpp>
#include <string>
#include <cmath>
#include <vector>
#include <iostream>

namespace fts {

static void score_sort(std::vector<fts::DocScore>& doc_scores)
{
    std::sort(doc_scores.begin(), doc_scores.end(), [](const fts::DocScore& a, const fts::DocScore& b) {
        return (a.score == b.score) ? (a.doc_id < b.doc_id) : (a.score > b.score);
    });
}

fts::SearchInfo Searcher::score_calc(const std::string& query)
{
    std::unordered_map<uint32_t, double> doc_scores_map;

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

    std::vector<fts::DocScore> doc_scores;
    doc_scores.reserve(doc_scores_map.size());

    for (const auto& [doc_id, score] : doc_scores_map)
    {
        doc_scores.emplace_back(fts::DocScore{doc_id, score});
    }

    score_sort(doc_scores);

    return fts::SearchInfo{doc_scores, ngrams.size()};
}

void Searcher::print_scores(const fts::SearchInfo& search_info)
{
    if (search_info.docs_scores.empty())
    {
        std::cout << "Nothing was found for your query" << '\n';
        return;
    }

    const std::vector<int> ranges{30, 150, 350};

    for (const auto& range : ranges)
    {
        constexpr std::size_t terms_max_num = 20;
        const std::size_t terms_num = std::min(search_info.num_of_terms, terms_max_num);

        const double min_score
            = static_cast<double>(terms_num) * std::log(static_cast<double>(accessor.total_docs()) / range);

        if (search_info.docs_scores.at(0).score < min_score)
        {
            continue;
        }

        for (const auto& doc : search_info.docs_scores)
        {
            if (doc.score > min_score)
            {
                std::cout << doc.score << ' ' << accessor.load_document(doc.doc_id) << '\n';
                continue;
            }
            return;
        }
    }
}

}  // namespace fts