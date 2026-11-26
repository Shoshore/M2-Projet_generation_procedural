# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (sparse)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-g)
find_package(OpenGL REQUIRED)
if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
    message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
    message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
# Compile external dependencies 
add_subdirectory (external)
# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP
if(INCLUDE_DISTRIB)
    add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)
# Main executable
add_executable(sparse
    main.cpp

	src/Camera.h
	src/Camera.cpp
	src/Mesh.h
	src/Mesh.cpp
	src/Scene.h
	src/Transform.h
	src/Controls.h
	src/Controls.cpp
	src/Engine.cpp
	src/Engine.h
	src/Viewport.cpp
	src/Viewport.h
	src/OMP.h
	src/OMP.cpp

	lib/stb_image.h
)
target_include_directories(sparse PRIVATE
	${CMAKE_SOURCE_DIR}
	external/glfw-3.1.2/include
	#external/glm-0.9.7.1
)
target_include_directories(sparse SYSTEM PRIVATE
    external/eigen3
	external/eigen3/Eigen
)
target_link_libraries(sparse PRIVATE
	glad
	glfw
	${OPENGL_gl_LIBRARY}
	${GLFW_LIBRARIES}
	${CMAKE_DL_LIBS}
)
target_compile_features(sparse PRIVATE cxx_std_17)
# Xcode and Visual working directories - set to project root
set_target_properties(sparse PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")
create_target_launcher(sparse WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")
SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
add_custom_command(
TARGET sparse POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/sparse${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/"
)
elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )
endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )