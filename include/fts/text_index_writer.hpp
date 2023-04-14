#pragma once
#include <fts/conf_parser.hpp>
#include <fts/index_builder.hpp>
#include <fts/interface_index_writer.hpp>
#include <string>

namespace fts {

class TextIndexWriter : public IndexWriter_I
{
    const std::string index_dir_path;

    const fts::ConfOptions config;

   public:
    TextIndexWriter(std::string path, fts::ConfOptions conf) : index_dir_path(std::move(path)), config(std::move(conf))
    {
    }

    void write(const fts::Index& index) const override;
};

}  // namespace fts