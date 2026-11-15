if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

add_executable(pbm src/main.cpp)
target_include_directories(pbm PRIVATE src)
target_compile_definitions(pbm PRIVATE SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_definitions(pbm PRIVATE RES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res")

add_subdirectory(deps/raylib SYSTEM)
target_link_libraries(pbm PRIVATE raylib)

set(RLCPP_PATCH ${CMAKE_CURRENT_SOURCE_DIR}/patches/raylib_cpp-nodiscard.patch)
execute_process(
    COMMAND git apply --reverse --check ${RLCPP_PATCH}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/deps/raylib_cpp
    RESULT_VARIABLE RLCPP_ALREADY_PATCHED
    OUTPUT_QUIET ERROR_QUIET)
if(NOT RLCPP_ALREADY_PATCHED EQUAL 0)
    execute_process(
        COMMAND git apply ${RLCPP_PATCH}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/deps/raylib_cpp
        COMMAND_ERROR_IS_FATAL ANY)
endif()

add_subdirectory(deps/raylib_cpp)
target_link_libraries(pbm PRIVATE raylib_cpp)

target_include_directories(pbm SYSTEM PRIVATE deps/eigen)
