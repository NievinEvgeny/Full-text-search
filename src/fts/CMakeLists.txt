set(target_name fts)

add_library(${target_name} STATIC
    conf_parser.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/conf_parser.hpp
    query_parser.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/query_parser.hpp
    indexer.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/index_accessor.hpp
    index_accessor.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/indexer.hpp
    searcher.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/searcher.hpp
    replxx-wrapper.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/replxx-wrapper.hpp
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