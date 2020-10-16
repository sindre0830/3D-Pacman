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

	header/levelData.h
	levelData.cpp

	header/function.h
	function.cpp

	class/StaticDrawing/StaticDrawing.h
	class/StaticDrawing/StaticDrawing.cpp

		class/StaticDrawing/Maze/Maze.h
		class/StaticDrawing/Maze/MazeShader.h
		class/StaticDrawing/Maze/Maze.cpp

		class/StaticDrawing/Pellet/Pellet.h
		class/StaticDrawing/Pellet/PelletShader.h
		class/StaticDrawing/Pellet/Pellet.cpp

		class/StaticDrawing/GameState/GameState.h
		class/StaticDrawing/GameState/GameStateShader.h
		class/StaticDrawing/GameState/GameState.cpp

	class/DynamicDrawing/DynamicDrawing.h
	class/DynamicDrawing/DynamicDrawing.cpp

		class/DynamicDrawing/Character/Character.h
		class/DynamicDrawing/Character/CharacterShader.h
		class/DynamicDrawing/Character/Character.cpp

			class/DynamicDrawing/Character/Ghost/Ghost.h
			class/DynamicDrawing/Character/Ghost/Ghost.cpp

			class/DynamicDrawing/Character/Pacman/Pacman.h
			class/DynamicDrawing/Character/Pacman/Pacman.cpp

		class/DynamicDrawing/Score/Score.h
		class/DynamicDrawing/Score/ScoreShader.h
		class/DynamicDrawing/Score/Score.cpp)

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
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/sprite/pacman.png
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sprite/pacman.png)

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/sprite/number.png
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sprite/number.png)

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/sprite/gameover.png
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sprite/gameover.png)

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/level/level0
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/level/level0)

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/level/level1
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/level/level1)