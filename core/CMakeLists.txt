set(SRC scoped_timer.cpp)
set(HEADER noncopyable.h scoped_timer.h)


add_library(core ${SRC} ${HEADER})
set_target_properties(core PROPERTIES PUBLIC_HEADER "${HEADER}")
target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR})
