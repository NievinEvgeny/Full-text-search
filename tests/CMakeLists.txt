set(target_name tests)

add_executable(
    ${target_name}
    fts_parser.cxx
)
target_link_libraries(
    ${target_name}
    PRIVATE
    GTest::gtest_main
    fts
)

include(GoogleTest)
gtest_discover_tests(${target_name})