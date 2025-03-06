set(SRC threadpool.cpp)
set(HEADER threadpool.h)

add_library(async ${SRC} ${HEADER})
set_target_properties(async PROPERTIES PUBLIC_HEADER "${HEADER}")
target_include_directories(async PUBLIC ${CMAKE_CURRENT_LIST_DIR}/..)
