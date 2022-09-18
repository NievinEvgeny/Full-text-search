set(target_name tests)

enable_testing()

add_executable(
    ${target_name}
    fts_sum.cxx
)
target_link_libraries(
    ${target_name}
    PRIVATE
    GTest::gtest_main
    fts
)

include(GoogleTest)
gtest_discover_tests(${target_name})