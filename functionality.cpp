/* library */
#include "header/functionality.h"
#include "header/level.h"
#include "header/camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <tiny_obj_loader.h>
#include <set>
#include <random>
#include <iostream>
#include <algorithm>
/* global data */
extern Level *g_level;
extern Camera *g_camera;
/**
 * @brief Create the minimap model matrix
 * 
 * @return glm::mat4 
 */
glm::mat4 getMinimapModelMatrix() {
    //move scene to top right corner
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.75f, 0.75f, 0.f));
    //scale scene down to 1/4 of the size
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
    return modelMatrix;
}
/**
 * @brief Change gamemode according to user input
 * 
 * @param window 
 */
void changeDimension(GLFWwindow* window) {
	if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		g_level->gamemode = FIRST_PERSON;
	} else if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		g_level->gamemode = TWO_DIMENSIONAL;
	} else if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		g_level->gamemode = THIRD_PERSON;
	}
}
/**
 * @brief Generate random index and set position
 * 
 * @param arr 
 * @param row 
 * @param col 
 */
void getGhostPos(std::vector<std::vector<int>> &arr, int &row, int &col) {
	//get random index
	int index = randomIndex(0, arr.size() - 1);
	//set position
	col = arr[index][0];
	row = arr[index][1];
	//remove position from array so ghosts don't spawn on same tile
	arr.erase(arr.begin() + index);
}
/**
 * @brief Generate random integer
 * 
 * @param min 
 * @param max 
 * @return int 
 */
int randomIndex(const int min, const int max) {
	//initialise (seed) engine
	std::random_device rd;
	//send seed to random-number engine (Mersenne-Twister)
	std::mt19937 rng(rd());
	//ubiased integer between 0 and array size
	std::uniform_int_distribution<int> uni(min, max);
	return uni(rng);
}
/**
 * @brief Generate VAO (rectangle shape)
 * 
 * @return GLuint 
 */
GLuint genObject(const std::vector<GLfloat> &arr, const int size) {
    //get indicies
    std::vector<GLuint> arrIndices = genIndices(size);
    //create VAO
    return createVAO(arr, arrIndices);
}
/**
 * @brief Generate indices for rectangle shape
 * 
 * @param size 
 * @return std::vector<GLuint> 
 */
std::vector<GLuint> genIndices(const int size) {
    std::vector<GLuint> arrIndices;
    for(int i = 0, j = 0; i < size; i++, j += 4) {
		//row 1
		arrIndices.push_back(j);
		arrIndices.push_back(j + 1);
		arrIndices.push_back(j + 2);
		//row 2
		arrIndices.push_back(j);
		arrIndices.push_back(j + 2);
		arrIndices.push_back(j + 3);
	}
    return arrIndices;
}
/**
 * @brief Compile the vertex and fragment shader.
 * 
 * @param vertexShaderSrc 
 * @param fragmentShaderSrc 
 * @return GLuint 
 */
GLuint compileShader(const std::string &vertexShaderSrc, const std::string &fragmentShaderSrc) {
    //declare debugging value
    GLint isCompiled = 0;
    //set shader paths
    auto vertexSrc = vertexShaderSrc.c_str();
    auto fragmentSrc = fragmentShaderSrc.c_str();
    //compile vertex shader 
    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
    glCompileShader(vertexShader);
    //get compile status
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    //branch if vertex shader is not compiled
    if(!isCompiled) {
        //get error length
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
        //set the error values
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);
        //print error message to user
        std::cout << errorLog.data() << std::endl;
        std::cin.get();
    }
    //compile fragment shader 
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
    glCompileShader(fragmentShader);
    //get compile status
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    //branch if fragment shader is not compiled
    if(!isCompiled) {
        //get error length
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
        //set the error values
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);
        //print error message to user
        std::cout << errorLog.data() << std::endl;
        std::cin.get();
    }
    //create shader program
    auto shaderProgram = glCreateProgram();
    //set shaderprogram to vertex and fragment data
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //set color value in the fragment shader
	glBindFragDataLocation(shaderProgram, 0, "outColor");
    //use program
    glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
    //delete junk values
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //send shader program
    return shaderProgram;
}
/**
 * @brief Loads model from filepath.
 * 
 * @param path 
 * @param size 
 * @return GLuint 
 */
GLuint loadModel(const std::string path, const std::string file, int &size) {
    //set structure and create a vector of them
	struct Vertex {
		glm::vec3 location;
		glm::vec3 normals;
		glm::vec2 textureCoordinate;
	};
    std::vector<Vertex> vertices;
    //declare data that is stored in the object file
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    //declare debugging data
    std::string warn;
    std::string err;
    //set all varibles from object file
    tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, (path + file).c_str(), path.c_str());
    //print warning message
    if(!warn.empty()) {
        std::cout << warn << std::endl;
    }
    //print error message
    if(!err.empty()) {
        std::cerr << err << std::endl;
    }
    //loop through data
    for(auto shape : shapes) {
        //loop through each mesh
        for(auto meshIndex : shape.mesh.indices) {
            //set mesh values in struct
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
            glm::vec2 textureCoordinate = {
                attrib.texcoords[meshIndex.texcoord_index * 2],
                attrib.texcoords[(meshIndex.texcoord_index * 2) + 1]
            };
            //push struct to vector
            vertices.push_back({vertice, normal, textureCoordinate});
        }
    }
    //create and bind the vertex array object
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    //create the vertex buffer object
    GLuint VBO;
    glGenBuffers(1, &VBO);
    //set VBO to vector of structs
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	//set the vertex attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(6 * sizeof(float)));
    //set size for draw loops
    size = vertices.size();
    return VAO;
}
/**
 * @brief Loads texture from filepath.
 * 
 * @param filepath
 * @param slot
 * @return GLuint
 */
GLuint loadTexture(const std::string& filepath, const GLuint slot) {
	//flip image
	stbi_set_flip_vertically_on_load(true);
	//load pixel data from a stored image
    int width, height, bpp;
    auto pixels = stbi_load(filepath.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
    //generate the texture, activate it and bind it
	GLuint tex{};
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, tex);
    //set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//transfer the image data to the texture in GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	//generate different resolutions for distance
	glGenerateMipmap(GL_TEXTURE_2D);
    //free the memory returned by STBI
    if(pixels) stbi_image_free(pixels);
    return tex;
}
/**
 * @brief Create the VAO, VBO and EBO. Bind the objects to memory.
 * 
 * @param arr
 * @param arr_indices
 * @return GLuint
 */
GLuint createVAO(const std::vector<GLfloat> &arr, const std::vector<GLuint> &arr_indices) {
	//create and bind the vertex array object
	GLuint VAO;
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//create the vertex buffer object
	GLuint VBO;
	glGenBuffers(1, &VBO);
	//set VBO to arr data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, arr.size() * sizeof(GLfloat), arr.data(), GL_STATIC_DRAW);
	//create the element buffer object
	GLuint EBO;
	glGenBuffers(1, &EBO);
	//set EBO to arr_indices data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, arr_indices.size() * sizeof(GLuint), arr_indices.data(), GL_STATIC_DRAW);
	return VAO;
}
/**
 * @brief Clear data from memory.
 * 
 * @param vao 
 */
void destroyVAO(GLuint &VAO) {
    //set data
	GLint nAttr = 0;
	std::set<GLuint> arrVBO;
    //get ebo and delete the buffer
	GLint idEBO;
	glGetVertexArrayiv(VAO, GL_ELEMENT_ARRAY_BUFFER_BINDING, &idEBO);
	glDeleteBuffers(1, (GLuint*)&idEBO);
    //get vertex attributes
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttr);
	glBindVertexArray(VAO);
    //get all vbo ids
	for(int iAttr = 0; iAttr < nAttr; iAttr++) {
        //get vbo id and enable attribute buffer
		GLint idVBO = 0;
		glGetVertexAttribiv(iAttr, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &idVBO);
        //branch if id is larger than 0
		if(idVBO > 0) {
			arrVBO.insert(idVBO);
		}
        //disable current attribute buffer
		glDisableVertexAttribArray(iAttr);
	}
    //loop through vbo id's and delete their buffer
	for(auto VBO : arrVBO) {
	    glDeleteBuffers(1, &VBO);
	}
    //delete VAO
	glDeleteVertexArrays(1, &VAO);
}
/**
 * @brief Resize frame to keep aspect ratio.
 * 
 * @param window 
 * @param width 
 * @param height 
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    //set difference values
	int widthDifference = width - g_level->windowWidth;
	int heightDifference = height - g_level->windowHeight;
    //set scene size and position according to width and height difference
	if(widthDifference > 0 && heightDifference > 0) {
		glViewport((widthDifference / 2) - (heightDifference / 2), 0, (width - widthDifference) + heightDifference, height);
	} else if(widthDifference < 0 && heightDifference < 0) {
		glViewport(-(heightDifference / 2), -(widthDifference / 2), (width + heightDifference), (height + widthDifference)); //wrong
	} else if(widthDifference > 0 && heightDifference < 0) {
		glViewport((widthDifference / 2) - (heightDifference / 2), 0, (width - widthDifference) + heightDifference, height);
	} else if(widthDifference < 0 && heightDifference > 0) {
		glViewport(0, -(widthDifference / 2) + (heightDifference / 2), width, (height + widthDifference) - heightDifference);
	} else {
		if(widthDifference > 0) {
			glViewport((widthDifference / 2), 0, (width - widthDifference), height);
		} else if(widthDifference < 0) {
			glViewport(0, -(widthDifference / 2), width, (height + widthDifference));
		} else if(heightDifference > 0) {
			glViewport(0, (heightDifference / 2), width, (height - heightDifference));
		} else if(heightDifference < 0) {
			glViewport(-(heightDifference / 2), 0, (width + heightDifference), height);
		}
	}
}
/**
 * @brief Eanable capture of debug output.
 * 
 */
void enableDebug() {
    //enable debug output
    glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    //send data to messageCallback() when debug is triggered
	glDebugMessageCallback(messageCallback, 0);
    //don't show uninteresting warnings (for NVIDIA drivers)
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
}
/**
 * @brief Customize the error message.
 * 
 * @param source 
 * @param type 
 * @param id 
 * @param severity 
 * @param length 
 * @param message 
 * @param userParam 
 */
void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam ) {
	std::cerr 
		<< "GL CALLBACK:" << ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ) 
		<< "type = 0x" << type 
		<< ", severity = 0x" << severity 
		<< ", message =" << message << '\n';
}
