add_library(luisa-compute-ext-stb-interface INTERFACE)
target_include_directories(luisa-compute-ext-stb-interface INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

add_library(luisa-compute-ext-stb OBJECT stb.c)
target_link_libraries(luisa-compute-ext-stb PRIVATE luisa-compute-ext-stb-interface)
set_target_properties(luisa-compute-ext-stb PROPERTIES POSITION_INDEPENDENT_CODE ON)
