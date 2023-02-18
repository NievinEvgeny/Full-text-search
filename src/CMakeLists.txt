set(target_name TextSearcher)

add_executable(${target_name})

include(CompileOptions)
set_compile_options(${target_name})

target_sources(
    ${target_name}
    PRIVATE
    main.cxx
)

target_link_libraries(
    ${target_name}
    PRIVATE
    fts
    cxxopts
)

add_subdirectory(fts)
add_subdirectory(fts-c)
add_subdirectory(testapp)