set(target_name fts-c)

add_library(${target_name} SHARED
    index_accessor.cxx
    ${PROJECT_SOURCE_DIR}/include/fts-c/index_accessor.h
    ${PROJECT_SOURCE_DIR}/include/fts-c/term_info.h
)

include(CompileOptions)
set_compile_options(${target_name})

target_link_libraries(${target_name} PUBLIC fts)

target_include_directories(
    ${target_name}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include/
)