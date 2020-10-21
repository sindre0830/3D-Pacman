/* library */
#include "Pellet.h"
#include "PelletShader.h"
#include "../../../header/dictionary.h"
#include "../../../header/levelData.h"
#include "../../../header/tiny_obj_loader.h"

/* global data */
extern LevelData *g_level;
/**
 * @brief Destroy the Pellet:: Pellet object
 * 
 */
Pellet::~Pellet() {}
/**
 * @brief Construct a new Pellet:: Pellet object
 * 
 */
Pellet::Pellet() {
    //compile pellet shader
	shaderProgram = compileShader(pelletVertexShader, pelletFragmentShader);
    //create VAO
	std::vector<GLfloat> arr = genCoordinates();
    VAO = genObject(arr, g_level->pelletSize);
	//set the vertex attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const void*)(2 * sizeof(GLfloat)));
	//set buffer position in refrence to position in level
	for(int i = 0, n = 0; i < g_level->gridHeight; i++) {
		for(int j = 0; j < g_level->gridWidth; j++) {
			//branch if target is either a pellet or a magic pellet
			if(g_level->grid[i][j] == PELLET || g_level->grid[i][j] == MAGICPELLET) {
				bufferPos[std::make_pair(i, j)] = n * pelletByteSize;
				n++;
			}
		}
	}
}
/**
 * @brief Draw object by installing the shader program and binding the VAO to the current rendering state
 * 
 */
void Pellet::draw() {
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (6 * g_level->pelletSize), GL_UNSIGNED_INT, (const void*)0);
}
/**
 * @brief Hide pellet by modifying the buffer array
 * 
 * @param y 
 * @param x 
 */
void Pellet::hidePellet(const int col, const int row) {
	GLfloat display = 0.f;
	for(int i = 8; i < pelletByteSize; i += 12) {
		glBufferSubData(GL_ARRAY_BUFFER, bufferPos[std::make_pair(col, row)] + i, sizeof(GLfloat), &display);
	}
}
/**
 * @brief Generate buffer array (x, y, display) * 4
 * 
 * @param target 
 * @return std::vector<GLfloat> 
 */
std::vector<GLfloat> Pellet::genCoordinates() {
	float
		//display pellet
		display = 1.f,
		//resize pellet
		xResize = g_level->gridElementWidth / 2.8f,
		yResize = g_level->gridElementHeight / 2.8f,
		//rotate pellet
		xRotate = g_level->gridElementWidth / 2.f,
		yRotate = g_level->gridElementHeight / 2.f;
	//buffer array
	std::vector<GLfloat> arr;
	//fills in array with coordinates
	for (int i = 0; i < g_level->gridHeight; i++) {
		for (int j = 0; j < g_level->gridWidth; j++) {
			//branch if target is pellet and insert data or if target is a magic pellet and make it twice as big
			if (g_level->grid[i][j] == PELLET) {
				arr.insert(arr.end(), {
					g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] + xResize, g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - yRotate, display,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] + xRotate, g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + yResize, display,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] - xResize, g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + yRotate, display,
					g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] - xRotate, g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - yResize, display
				});
			} else if (g_level->grid[i][j] == MAGICPELLET) {
				arr.insert(arr.end(), {
					g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] + (xResize * 2.3f), g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - yRotate, display,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] + xRotate, g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + (yResize * 2.3f), display,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] - (xResize * 2.3f), g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + yRotate, display,
					g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] - xRotate, g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - (yResize * 2.3f), display
				});
			}
		}
	}
	return arr;
}
/**
 * @brief Loads the pellet model
 *
 */
GLuint Pellet::loadPellet(const std::string path, int& size)     {
	//Some variables that we are going to use to store data from tinyObj
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials; //This one goes unused for now, seeing as we don't need materials for this model.

	//Some variables incase there is something wrong with our obj file
	std::string warn;
	std::string err;

	//We use tinobj to load our models. Feel free to find other .obj files and see if you can load them.
	tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, (path + "../../../models/pellet/pellet.obj").c_str(), path.c_str());

	if (!warn.empty()) {
		std::cout << warn << std::endl;
	}

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	//For each shape defined in the obj file
	for (auto shape : shapes)
	{
		//We find each mesh
		for (auto meshIndex : shape.mesh.indices)
		{
			//And store the data for each vertice, including normals
			glm::vec3 vertice = {
				attrib.vertices[meshIndex.vertex_index * 3],
				attrib.vertices[(meshIndex.vertex_index * 3) + 1],
				attrib.vertices[(meshIndex.vertex_index * 3) + 2]
			};
			glm::vec3 normal = {
				attrib.normals[meshIndex.normal_index * 3],
				attrib.normals[(meshIndex.normal_index * 3) + 1],
				attrib.normals[(meshIndex.normal_index * 3) + 2]
			};
			glm::vec2 textureCoordinate = {                         //These go unnused, but if you want textures, you will need them.
				attrib.texcoords[meshIndex.texcoord_index * 2],
				attrib.texcoords[(meshIndex.texcoord_index * 2) + 1]
			};

			arrIndices.push_back({ vertice, normal, textureCoordinate }); //We add our new vertice struct to our vector

		}
	}
}
