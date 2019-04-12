cmake_minimum_required(VERSION 2.8)
project(llfsm_test)

add_compile_options(-std=c++11)

set(LLFSM_LIB_DIR ${PROJECT_SOURCE_DIR}/..)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(${LLFSM_LIB_DIR} ${LLFSM_LIB_DIR}/build/llfsm)

include_directories(${LLFSM_LIB_DIR})

link_directories(${LLFSM_LIB_DIR}/lib)

aux_source_directory(. DIR_SRCS)
aux_source_directory(shell DIR_SRCS)

add_executable(${PROJECT_NAME} ${DIR_SRCS})

target_link_libraries(${PROJECT_NAME} llfsm)
if (UNIX)
target_link_libraries(${PROJECT_NAME} pthread)
endif ()
