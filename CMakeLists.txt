# Cherie Language
cmake_minimum_required (VERSION 3.8)
project (Cherie)

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++17" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++17")
    endif()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE CHERIE_SRC "cpp.hint" "include/*.h" "src/*.cpp")
add_library (Cherie STATIC ${CHERIE_SRC})
target_include_directories(Cherie PUBLIC "include/")

file(GLOB_RECURSE CHERIE_TEST_SRC "test/*.cpp")
add_executable(Cherie_Test ${CHERIE_TEST_SRC})
target_link_libraries(Cherie_Test Cherie)