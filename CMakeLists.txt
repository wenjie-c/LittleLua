CMAKE_MINIMUM_REQUIRED(VERSION 3.13)
set(LANGUAGE CXX)
project(LittleLua VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_SOURCE_DIR src)
include_directories(include)
link_directories(lib)
add_subdirectory(${PROJECT_SOURCE_DIR})
add_subdirectory(tests)
set_target_properties(LittleLua PROPERTIES LINKER_LANGUAGE CXX 
LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
target_link_libraries(LittleLua lua)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-Os") # -DCMAKE_BUILD_TYPE=Release
