set(target_name fts)

add_library(${target_name} STATIC
    fts.cxx
    ${PROJECT_SOURCE_DIR}/include/fts/fts.hpp)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
    ${target_name}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include/
)