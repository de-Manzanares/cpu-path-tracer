cmake_minimum_required(VERSION 3.31)

project(rtiow VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CompilerOptions -Wall -Wextra -Wpedantic
        #-Werror
)
set(DEBUGCompilerOptions -g ${CompilerOptions})
set(COVCompilerOptions --coverage ${CompilerOptions})
set(COVLinkerOptions --coverage)
set(SANCompilerOptions -fsanitize=address,undefined ${DEBUGCompilerOptions})
set(SANLinkerOptions -fsanitize=address,undefined)
set(LIBCXXCompileOptions -stdlib=libc++)
set(LIBCXXLinkerOptions -stdlib=libc++ -lc++abi)

add_library(
        common-options
        INTERFACE
)
target_compile_options(
        common-options
        INTERFACE
        $<$<CONFIG:Debug>:$<$<CXX_COMPILER_ID:GNU,Clang>:${DEBUGCompilerOptions}>>
        $<$<CONFIG:Coverage>:$<$<CXX_COMPILER_ID:GNU,Clang>:${COVCompilerOptions}>>
        $<$<CONFIG:Sanitizer>:$<$<CXX_COMPILER_ID:GNU,Clang>:${SANCompilerOptions}>>
)
target_link_options(
        common-options
        INTERFACE
        $<$<CONFIG:Coverage>:$<$<CXX_COMPILER_ID:GNU,Clang>:${COVLinkerOptions}>>
        $<$<CONFIG:Sanitizer>:$<$<CXX_COMPILER_ID:GNU,Clang>:${SANLinkerOptions}>>
)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(common-options INTERFACE ${LIBCXXCompileOptions})
    target_link_options(common-options INTERFACE ${LIBCXXLinkerOptions})
endif ()

add_executable(rtiow main.cpp)
target_link_libraries(rtiow PRIVATE common-options)
