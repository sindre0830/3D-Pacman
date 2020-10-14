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
	header/dictionary.h
	main.cpp
	header/levelData.h
	levelData.cpp
	header/function.h
	function.cpp
	header/geometricShape.h
	geometricShape.cpp
		header/number.h
		shader/number.h
		number.cpp
		header/gameover.h
		shader/gameover.h
		gameover.cpp
		header/wall.h
		shader/wall.h
		wall.cpp
		header/pellet.h
		shader/pellet.h
		pellet.cpp
		header/character.h
		character.cpp
			shader/character.h
			header/pacman.h
			pacman.cpp
			header/ghost.h
			ghost.cpp)

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