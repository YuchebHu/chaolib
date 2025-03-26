set(SRC test.cpp)

add_library(serializer ${SRC})

target_link_libraries(serializer PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(serializer PRIVATE ${NLOHMANN_JSON_INCLUDE})
