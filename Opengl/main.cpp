#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "GLError.h"
#include "Object.h"


int main() {


	//Setup

	if (!glfwInit()) return -1;
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World!", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cout << "Err while initializing GLEW" << std::endl;
	}

	//End of setup
	
	glm::vec3 data[] = {
		glm::vec3(-0.5, -0.5, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.0f)
	};
	
	/*
	glm::vec2 data[] = {
		glm::vec2(-0.5, -0.5),
		glm::vec2(0.5f, -0.5f),
		glm::vec2(0.5f, 0.5f),
		glm::vec2(-0.5f, 0.5f)
	};
	*/
	unsigned int indices[]{
		0, 1, 2,
		2, 3, 0
	};

	{
	
	VertexArray va;
	VertexBuffer vbo(data, 12 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push(3);
	va.AddBuffer(vbo, layout);

	IndexBuffer ibo(indices, 6);


	Shader shader("Resources/Shaders/Basic.shader");

	shader.setUniformV4f("u_Colour", glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));

	shader.Deattach();
	va.Unbind();
	vbo.Unbind();
	ibo.Unbind();

	float r = 0.0f;
	float increment = 0.05f;


	Renderer renderer;


	while (!glfwWindowShouldClose(window)) {

		GlCall(glClear(GL_COLOR_BUFFER_BIT));

		shader.Attach();
		shader.setUniformV4f("u_Colour", glm::vec4(r, 1 - r, r, 1.0f));

		renderer.Draw(&va, &ibo, &shader);

		if (r > 1.0f || r < 0.0f) increment *= -1.0f;
		r += increment;


		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	} //Need this to make buffer go out of scope before termination.

	glfwTerminate();
	return 0;
}