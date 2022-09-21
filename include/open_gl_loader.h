#ifndef __OPEN_GL_LOADER__
#define __OPEN_GL_LOADER__

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

GLenum mode = GL_TRIANGLES;
GLsizei _size = 3;
bool start = false;

struct Open_GL_Loader
{
private:
	unsigned int CUR_SCR_WIDTH, CUR_SCR_HEIGHT;

	void glfwContextInit();
	void glfwWindowInit();
	void glfwCallbacksSetter();
	void glfwGladLoader();
	
public:
	GLFWwindow* window;
	Open_GL_Loader(const unsigned int&, const unsigned int&);
	~Open_GL_Loader();
};

Open_GL_Loader::Open_GL_Loader(const unsigned int &_SCR_WIDTH, const unsigned int &_SCR_HEIGHT) : CUR_SCR_WIDTH(_SCR_WIDTH), CUR_SCR_HEIGHT(_SCR_HEIGHT)
{
	glfwInit();
	glfwContextInit();
	glfwWindowInit();
	glfwCallbacksSetter();
	glfwGladLoader();
}

Open_GL_Loader::~Open_GL_Loader()
{
	glfwTerminate();
}

void Open_GL_Loader::glfwContextInit()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Open_GL_Loader::glfwWindowInit()
{
	window = glfwCreateWindow(CUR_SCR_WIDTH, CUR_SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
}

void Open_GL_Loader::glfwCallbacksSetter()
{
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
}

void Open_GL_Loader::glfwGladLoader()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


 
template<class ModifiedObject>
void objectSetterUp(ModifiedObject currentObject, unsigned int& _VBO, unsigned int& _VAO, unsigned int& _EBO)
{
	//Buffer Generators
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);

	//Setting Up Buffers
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(currentObject.get_vertices()) * currentObject.size(), &currentObject.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(currentObject.indices) * currentObject.indices.size(), &currentObject.indices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
	glEnableVertexAttribArray(0);
	
	//Unbinding VAO, VBO and EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


#endif //__OPEN_GL_LOADER__