cmake_minimum_required(VERSION 3.12)

project(Pac-Man)

find_package(OpenGL REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(external/glfw-3.3.2)
add_subdirectory(external/glew-cmake-2.1.0)
add_subdirectory(external/glm-0.9.9.8)

add_executable(${PROJECT_NAME}

	main.cpp

	header/dictionary.h

	header/level.h
	level.cpp

	header/camera.h
	camera.cpp

	header/functionality.h
	functionality.cpp

	class/header/score.h
	class/shader/score.h
	class/score.cpp

	class/header/maze.h
	class/shader/maze.h
	class/maze.cpp

	class/header/pellet.h
	class/shader/pellet.h
	class/pellet.cpp

	class/header/gameState.h
	class/shader/gameState.h
	class/gameState.cpp

	class/header/character.h
	class/shader/character.h
	class/character.cpp

	class/header/ghost.h
	class/ghost.cpp

	class/header/pacman.h
	class/pacman.cpp
	)

target_include_directories(${PROJECT_NAME}
	PRIVATE
	${CMAKE_SOURCE_DIR}/external/stb/include
	${CMAKE_SOURCE_DIR}/external/tiny_obj_loader/include)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE
	GLEW_STATIC
	STB_IMAGE_IMPLEMENTATION
	TINYOBJLOADER_IMPLEMENTATION)

target_link_libraries(${PROJECT_NAME}
	libglew_static
	glfw
	glm
	OpenGL::GL)

add_custom_command(
	TARGET ${PROJECT_NAME} PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/sprite
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sprite)

add_custom_command(
	TARGET ${PROJECT_NAME} PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/level
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/level)

add_custom_command(
	TARGET ${PROJECT_NAME} PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/model
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/model)
