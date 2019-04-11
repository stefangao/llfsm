cmake_minimum_required(VERSION 2.8)

project(llfsm)

add_compile_options(-std=c++11)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/inc)

file(GLOB_RECURSE DIR_HEADERS  *.h)
source_group("Include" FILES ${DIR_HEADERS}) 

aux_source_directory(core DIR_LIB_SRCS)
aux_source_directory(platform DIR_LIB_SRCS)
aux_source_directory(platform/shell DIR_LIB_SRCS)

add_library (${PROJECT_NAME} ${DIR_LIB_SRCS} ${DIR_HEADERS})
