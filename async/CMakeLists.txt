set(SRC thread_pool.cpp)
set(HEADER thread_pool.h mpscqueue.h spin_lock.h)

add_library(async ${SRC} ${HEADER})
set_target_properties(async PROPERTIES PUBLIC_HEADER "${HEADER}")
target_include_directories(async PUBLIC ${CMAKE_SOURCE_DIR})
