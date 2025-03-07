cmake_minimum_required(VERSION 3.10.0)
project(chelib VERSION 0.1.0 LANGUAGES C CXX)

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

add_subdirectory(async)
add_subdirectory(core)
add_subdirectory(tests)

add_executable(chelib main.cpp)
target_link_libraries(chelib PRIVATE async)


