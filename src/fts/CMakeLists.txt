set(target_name fts)

add_library(${target_name} STATIC
    conf_parse.cxx
    query_parse.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/parser.hpp
    indexer.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/indexer.hpp
    searcher.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/searcher.hpp
)

include(CompileOptions)
set_compile_options(${target_name})

target_link_libraries(
    ${target_name}
    PRIVATE
    nlohmann_json::nlohmann_json
    rapidcsv
)

target_include_directories(
    ${target_name}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/external/
)