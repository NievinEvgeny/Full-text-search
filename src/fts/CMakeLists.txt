set(target_name fts)

add_library(${target_name} STATIC
    conf_parser.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/conf_parser.hpp
    query_parser.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/query_parser.hpp
    indexer.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/indexer.hpp
    searcher.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/searcher.hpp
    replxx-wrapper.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/replxx-wrapper.hpp
)

include(CompileOptions)
set_compile_options(${target_name})

target_link_libraries(
    ${target_name}
    PRIVATE
    nlohmann_json::nlohmann_json
    rapidcsv
    replxx
)

target_include_directories(
    ${target_name}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/external/
)