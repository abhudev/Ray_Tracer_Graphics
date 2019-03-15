cmake_minimum_required (VERSION 2.6)
project (scene)

find_package(OpenGL REQUIRED)

# Compile external dependencies 
add_subdirectory (external)

include_directories(
	external/AntTweakBar-1.16/include/
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	external/bullet-2.81-rev2613/src/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_executable(gen_scene
    src/make_scene.cpp
    common/shader.cpp
    common/shader.hpp
    common/texture.cpp
    common/texture.hpp
    common/controls.cpp
    common/controls.hpp

    src/TransformVertexShader.vertexshader
    src/TextureFragmentShader.fragmentshader
)

target_link_libraries(gen_scene
    ${ALL_LIBS}
)

set_target_properties(gen_scene PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/")