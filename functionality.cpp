/* library */
#include "header/functionality.h"
#include "header/levelData.h"
#include "header/camera.h"
#include <stb_image.h>
#include <tiny_obj_loader.h>
#include <set>
#include <random>
#include <iostream>
#include <algorithm>
/* global data */
extern LevelData *g_level;
extern Camera *g_camera;
/**
 * @brief Compile the vertex and fragment shader.
 * 
 * @param vertexShaderSrc 
 * @param fragmentShaderSrc 
 * @return GLuint 
 */
GLuint compileShader(const std::string &vertexShaderSrc, const std::string &fragmentShaderSrc) {
    GLint isCompiled = 0;

    auto vertexSrc = vertexShaderSrc.c_str();
    auto fragmentSrc = fragmentShaderSrc.c_str();

    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if(!isCompiled) {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);
        // Provide the infolog in whatever manor you deem best.
        std::cout << errorLog.data() << std::endl;
        std::cin.get();
    }
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if(!isCompiled) {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        std::cout << errorLog.data() << std::endl;
        std::cin.get();
    }

    auto shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

	glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
/**
 * @brief Generate VAO (rectangle shape)
 * 
 * @return GLuint 
 */
GLuint genObject(const std::vector<GLfloat> &arr, const int size) {
    std::vector<GLuint> arrIndices = genIndices(size);
    return createVAO(arr, arrIndices);
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
	GLuint vao;
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//create the vertex buffer object
	GLuint vbo;
	glGenBuffers(1, &vbo);
	//set vbo to arr data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, arr.size() * sizeof(GLfloat), arr.data(), GL_STATIC_DRAW);
	//create the element buffer object
	GLuint ebo;
	glGenBuffers(1, &ebo);
	//set ebo to arr_indices data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, arr_indices.size() * sizeof(GLuint), arr_indices.data(), GL_STATIC_DRAW);
	return vao;
}
/**
 * @brief Generate indices for rectangle shape
 * 
 * @param size 
 * @return std::vector<GLuint> 
 */
std::vector<GLuint> genIndices(const int size) {
    std::vector<GLuint> arrIndices;
    for (int i = 0, j = 0; i < size; i++, j += 4) {
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
 * @brief Clear data from memory.
 * 
 * @param vao 
 */
void destroyVAO(GLuint &vao) {
	GLint nAttr = 0;
	std::set<GLuint> vbos;

	GLint eboId;
	glGetVertexArrayiv(vao, GL_ELEMENT_ARRAY_BUFFER_BINDING, &eboId);
	glDeleteBuffers(1, (GLuint*)&eboId);

	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttr);
	glBindVertexArray(vao);

	for (int iAttr = 0; iAttr < nAttr; ++iAttr) {
		GLint vboId = 0;
		glGetVertexAttribiv(iAttr, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vboId);
		if (vboId > 0) {
			vbos.insert(vboId);
		}
		glDisableVertexAttribArray(iAttr);
	}

	for(auto vbo : vbos) {
	  glDeleteBuffers(1, &vbo);
	}

	glDeleteVertexArrays(1, &vao);
}
/**
 * @brief Get the Minimap Model Matrix object
 * 
 * @return glm::mat4 
 */
glm::mat4 getMinimapModelMatrix() {
    float angle = 0.f;
    if(g_level->gamemode == FIRST_PERSON) {
        if(g_camera->direction == LEFT) {
            angle = 90.f;
        } else if(g_camera->direction == DOWN) {
            angle = 180.f;
        } else if(g_camera->direction == RIGHT) {
            angle = 270.f;
        }    
    }
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.75f, 0.75f, 0.f));
    //modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(0.f, 0.f, 1.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
    return modelMatrix;
}
/**
 * @brief 
 * 
 * @param path 
 * @param size 
 * @return GLuint 
 */
GLuint loadModel(const std::string path, const std::string file, int &size) {
	struct Vertex {
		glm::vec3 location;
		glm::vec3 normals;
		glm::vec2 textureCoordinate;
	};
    //We create a vector of Vertex structs. OpenGL can understand these, and so will accept them as input.
    std::vector<Vertex> vertices;

    //Some variables that we are going to use to store data from tinyObj
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials; //This one goes unused for now, seeing as we don't need materials for this model.
    
    //Some variables incase there is something wrong with our obj file
    std::string warn;
    std::string err;

    //We use tinobj to load our models. Feel free to find other .obj files and see if you can load them.
    tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, (path + file).c_str(), path.c_str());

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

            vertices.push_back({ vertice, normal, textureCoordinate  }); //We add our new vertice struct to our vector

        }
    }

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //As you can see, OpenGL will accept a vector of structs as a valid input here
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float)*3));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
    //This will be needed later to specify how much we need to draw. Look at the main loop to find this variable again.
    size = vertices.size();

    return VAO;
}
/**
 * @brief 
 * 
 * @param window 
 */
void changeDimension(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		g_level->gamemode = FIRST_PERSON;
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		g_level->gamemode = TWO_DIMENSIONAL;
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		g_level->gamemode = THIRD_PERSON;
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}
/**
 * @brief 
 * 
 * @param window 
 * @param xpos 
 * @param ypos 
 */
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	g_camera->updateDirection(xpos, ypos);
}
/**
 * @brief 
 * 
 * @param window 
 * @param width 
 * @param height 
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);

	int widthDifference = width - g_level->windowWidth;
	int heightDifference = height - g_level->windowHeight;
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
 * @brief 
 * 
 * @param min 
 * @param max 
 * @return int 
 */
int randomIndex(const int min, const int max) {
	//initialise (seed) engine
	std::random_device rd;
	//random-number engine (Mersenne-Twister)
	std::mt19937 rng(rd());
	//ubiased integer between 0 and array size
	std::uniform_int_distribution<int> uni(min, max);
	return uni(rng);
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
 * @brief Eanable capture of debug output.
 * 
 */
void enableDebug() {
    glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(messageCallback, 0);
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
