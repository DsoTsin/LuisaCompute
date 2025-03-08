if (LUISA_COMPUTE_ENABLE_GUI)

    # imgui
    set(LUISA_IMGUI_SOURCES
            ../ext/imgui/imgui.cpp
            ../ext/imgui/imgui_demo.cpp
            ../ext/imgui/imgui_draw.cpp
            ../ext/imgui/imgui_tables.cpp
            ../ext/imgui/imgui_widgets.cpp
            ../ext/imgui/backends/imgui_impl_glfw.cpp)

    set(LUISA_IMGUI_HEADERS
            ../ext/imgui/imconfig.h
            ../ext/imgui/imgui.h
            ../ext/imgui/imgui_internal.h
            ../ext/imgui/imstb_rectpack.h
            ../ext/imgui/imstb_textedit.h
            ../ext/imgui/imstb_truetype.h
            ../ext/imgui/backends/imgui_impl_glfw.h)

    set(LUISA_COMPUTE_GUI_SOURCES
            framerate.cpp
            imgui_window.cpp
            window.cpp
            ${LUISA_IMGUI_SOURCES})

    add_library(luisa-compute-gui SHARED ${LUISA_COMPUTE_GUI_SOURCES})
    target_include_directories(luisa-compute-gui PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../ext/imgui>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../ext/imgui/backends>)
    target_link_libraries(luisa-compute-gui PUBLIC luisa-compute-runtime luisa-compute-dsl)
    target_compile_definitions(luisa-compute-gui
            PRIVATE LC_GUI_EXPORT_DLL
            PUBLIC LUISA_ENABLE_GUI=1 GLFW_INCLUDE_NONE IMGUI_DEFINE_MATH_OPERATORS)
    if (WIN32)
        target_compile_definitions(luisa-compute-gui PRIVATE
                "IMGUI_API=__declspec(dllexport)"
                "IMGUI_IMPL_API=__declspec(dllexport)")
    endif ()
    set_target_properties(luisa-compute-gui PROPERTIES
            UNITY_BUILD ${LUISA_COMPUTE_ENABLE_UNITY_BUILD}
            OUTPUT_NAME lc-gui)
    luisa_compute_install(gui SOURCES ${LUISA_COMPUTE_GUI_SOURCES})
    install(FILES ${LUISA_IMGUI_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/luisa/ext)
else ()
    add_library(luisa-compute-gui INTERFACE)
    luisa_compute_install(gui)
endif ()
