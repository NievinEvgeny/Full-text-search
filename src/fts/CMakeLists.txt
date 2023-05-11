set(target_name fts)

add_library(${target_name} STATIC
    config.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/config.hpp
    query_parser.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/query_parser.hpp
    ${PROJECT_SOURCE_DIR}/include/fts/binary_reader.hpp
    text_index_accessor.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/i_index_accessor.hpp
    ${PROJECT_SOURCE_DIR}/include/fts/text_index_accessor.hpp
    bin_index_accessor.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/bin_index_accessor.hpp
    index_builder.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/index_builder.hpp
    text_index_writer.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/i_index_writer.hpp
    ${PROJECT_SOURCE_DIR}/include/fts/text_index_writer.hpp
    bin_index_writer.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/bin_index_writer.hpp
    trie.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/binary_buffer.hpp
    ${PROJECT_SOURCE_DIR}/include/fts/trie.hpp
    searcher.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/searcher.hpp
    replxx_wrapper.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/replxx_wrapper.hpp
    word_hash.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/word_hash.hpp
)

include(CompileOptions)
set_compile_options(${target_name})

target_link_libraries(
    ${target_name}
    PRIVATE
    rapidcsv
    replxx
    PUBLIC
    nlohmann_json::nlohmann_json
)

target_include_directories(
    ${target_name}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/external/
)