set(target_name tests)

add_executable(
    ${target_name}
    fts_parser.cxx
    fts_indexer.cxx
)
target_link_libraries(
    ${target_name}
    PRIVATE
    GTest::gtest_main
    nlohmann_json::nlohmann_json
    fts
)

include(GoogleTest)
gtest_discover_tests(${target_name})