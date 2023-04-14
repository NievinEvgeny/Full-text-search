#include <fts/index_writer.hpp>
#include <fts/index_builder.hpp>
#include <fts/conf_parser.hpp>
#include <fts/word_hash.hpp>
#include <string>
#include <fstream>

namespace fts {

static void config_serialize(const std::string& index_dir_path, const fts::ConfOptions& config)
{
    std::ofstream conf_copy_file(index_dir_path + "/Config.json");

    if (!conf_copy_file.is_open())
    {
        throw std::runtime_error{"Can't open file in serialize_config function"};
    }

    fts::print_config_to_json(conf_copy_file, config);

    conf_copy_file.close();
}

static void docs_serialize(const std::string& index_dir_path, const fts::Index& index)
{
    for (const auto& [doc_id, text] : index.docs)
    {
        std::ofstream current_doc(index_dir_path + "/docs/" += std::to_string(doc_id));

        if (!current_doc.is_open())
        {
            throw std::runtime_error{"Can't open file in TextIndexWriter::write function"};
        }

        current_doc << text;

        current_doc.close();
    }
}

static void entries_serialize(const std::string& index_dir_path, const fts::Index& index)
{
    for (const auto& [term_hash, terms] : index.entries)
    {
        std::ofstream current_entrie(index_dir_path + "/entries/" += term_hash);

        for (const auto& [term, docs] : terms)
        {
            if (!current_entrie.is_open())
            {
                throw std::runtime_error{"Can't open file in TextIndexWriter::write function"};
            }

            current_entrie << term << ' ' << docs.size() << ' ';

            for (const auto& [doc_id, term_positions] : docs)
            {
                current_entrie << doc_id << ' ' << term_positions.size() << ' ';

                for (const auto& term_position : term_positions)
                {
                    current_entrie << term_position << ' ';
                }
            }
            current_entrie << '\n';
        }
        current_entrie.close();
    }
}

void TextIndexWriter::write(const fts::Index& index) const
{
    fts::config_serialize(index_dir_path, config);
    fts::docs_serialize(index_dir_path, index);
    fts::entries_serialize(index_dir_path, index);
}

}  // namespace fts