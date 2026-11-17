if(MSVC AND NOT "${MSVC_VERSION}" LESS 1400)
    add_definitions("/MP")
endif()

add_subdirectory(glad)
add_subdirectory(glfw-3.1.2)

