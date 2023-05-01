#pragma once
#include <fts/index_builder.hpp>

namespace fts {

class IndexWriter_I
{
   public:
    virtual void write(const fts::Index& index) const = 0;
    virtual ~IndexWriter_I() = default;
};

}  // namespace fts