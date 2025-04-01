add_library(serializer INTERFACE)
target_link_libraries(serializer INTERFACE nlohmann_json::nlohmann_json)
target_include_directories(serializer INTERFACE ${CMAKE_SOURCE_DIR}/)
target_include_directories(serializer INTERFACE ${NLOHMANN_JSON_INCLUDE}/)

