cmake_minimum_required(VERSION 3.10.0)
project(chaolib VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 启用实验性库支持
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  # add_compile_options(-fexperimental-library)
elseif(MSVC)
  add_compile_options(/std:c++latest)
endif()

add_subdirectory(extern)

add_subdirectory(async)
add_subdirectory(core)
add_subdirectory(serializer)
add_subdirectory(tests)

add_executable(chaolib main.cpp)
target_link_libraries(chaolib PRIVATE async)

set(FORMAT_DIRECTORIES
  async
  core
  serializer
  tests
)
set(PENDING_FORMAT_SOURCES)
foreach(DIR ${FORMAT_DIRECTORIES})
  file(GLOB_RECURSE DIR_FORMAT_FILES
    ${DIR}/*.cpp
    ${DIR}/*.h)
  list(APPEND PENDING_FORMAT_SOURCES ${DIR_FORMAT_FILES})
endforeach()

add_custom_target(
  format
  COMMAND clang-format -i ${PENDING_FORMAT_SOURCES}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Format all .h/.cpp files"
)
