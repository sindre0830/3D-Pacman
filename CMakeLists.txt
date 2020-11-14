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

	external/tiny_obj_loader.h

	header/dictionary.h

	header/levelData.h
	levelData.cpp

	header/Camera.h
	Camera.cpp

	header/functionality.h
	functionality.cpp

	class/Maze/Maze.h
	class/Maze/MazeShader.h
	class/Maze/Maze.cpp

	class/Pellet/Pellet.h
	class/Pellet/PelletShader.h
	class/Pellet/Pellet.cpp

	class/GameState/GameState.h
	class/GameState/GameStateShader.h
	class/GameState/GameState.cpp

	class/Score/Score.h
	class/Score/ScoreShader.h
	class/Score/Score.cpp

	class/Character/Character.h
	class/Character/CharacterShader.h
	class/Character/Character.cpp

		class/Character/Ghost/Ghost.h
		class/Character/Ghost/Ghost.cpp

		class/Character/Pacman/Pacman.h
		class/Character/Pacman/Pacman.cpp
	)

target_include_directories(${PROJECT_NAME}
	PRIVATE
	${CMAKE_SOURCE_DIR}/external/stb/include)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE
	GLEW_STATIC
	STB_IMAGE_IMPLEMENTATION)

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
	${CMAKE_SOURCE_DIR}/models
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/models)
