#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

#include "GLError.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Object.h"
#include "SSBO.h"

glm::vec3 moveCamera(GLFWwindow* window, glm::vec3 cameraDirection) {

	glm::vec3 cameraMovement(0.0f);
	const float cameraSpeed = 0.005f;

	if (glfwGetKey(window, GLFW_KEY_A)) {
		cameraMovement += glm::vec3(cameraDirection.z, 0.0f, -cameraDirection.x);
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		cameraMovement += glm::vec3(-cameraDirection.x, 0.0f, -cameraDirection.z);
	}
	if (glfwGetKey(window, GLFW_KEY_W)) {
		cameraMovement += glm::vec3(cameraDirection.x, 0.0f, cameraDirection.z);
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		cameraMovement += glm::vec3(-cameraDirection.z, 0.0f, cameraDirection.x);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE)) {
		cameraMovement += glm::vec3(0.0f, 1.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_Q)) {
		cameraMovement += glm::vec3(0.0f, -1.0f, 0.0f);
	}

	return cameraMovement * cameraSpeed;
}


int main() {

	//Setup

	if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	unsigned int SCREEN_WIDTH = 900;
	unsigned int SCREEN_HEIGHT = 900;

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GSimC", NULL, NULL);

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

	//Simulation
	unsigned int SIMULATION_WIDTH = 900;
	unsigned int SIMULATION_HEIGHT = 900;
	unsigned int SIMULATION_DEPTH = 900;

	//Starter program

	glm::vec3 points[] = {
		glm::vec3(-0.5f, -0.5f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.0f)
	};

	unsigned int indices[] = {
		0, 1, 2, 3
	};


	glm::vec4 col(1.0, 1.0, 1.0, 1.0);
	float r = 0;
	bool incrementing = true;

	VertexArray VAO;
	VertexBuffer VBO(points, sizeof(points));
	VertexBufferLayout layout;
	layout.Push(3);

	VAO.AddBuffer(&VBO, &layout);

	IndexBuffer IBO(indices, 4);
	Renderer renderer;

	Shader mainShader("vertex.glsl", "fragment.glsl");

	//3D
	//Camera

	glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraVec(0.0f, 0.0f, -1.0f);
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraVec, glm::vec3(0.0f, 1.0f, 0.0f));

	//Model
	glm::mat4 model = glm::mat4(1.0f);

	//Perspective projection
	glm::mat4 projection = glm::perspective(glm::radians(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		0.1f, 100.0f);


	glm::mat4 fullProj = projection * view * model;

	mainShader.Attach();
	mainShader.setUniformM4f("projectionAllMat", fullProj);
	mainShader.Deattach();

	const glm::vec3 emptyMovementVector(0.0f);
	double cursorXPos, cursorYPos;
	glfwGetCursorPos(window, &cursorXPos, &cursorYPos);

	//Particles setup
	const int numParticles = 4;
	const int numTreeNodes = numParticles * 8 + 1;

	glm::vec3 positions[numParticles]{
		glm::vec3(1.0f, -1.0f, 1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(1.0, 1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f)
	};

	
	//Compute shaders
	Shader s_TreeCreation("TreeCreation.glsl");
	s_TreeCreation.Attach();

	//Set bounds
	//s_TreeCreation.setUniformV3f("simulationBounds", float(SIMULATION_WIDTH), float(SIMULATION_HEIGHT), float(SIMULATION_DEPTH));
	

	int tree[numTreeNodes] = { 0 };
	glm::vec3 centerOfMasses[numTreeNodes] = { glm::vec3(0) };
	unsigned int centerOfMassesN[numTreeNodes] = { 0 };


	SSBO treeBuffer(tree, numTreeNodes * sizeof(int), 0, GL_DYNAMIC_COPY);
	SSBO positionsBuffer(positions, numParticles * sizeof(glm::vec3), 1, GL_STATIC_DRAW);
	SSBO centerOfMassesBuffer(centerOfMasses, numTreeNodes * sizeof(glm::vec3), 2, GL_DYNAMIC_COPY);
	SSBO centerOfMassesNBuffer(centerOfMassesN, numTreeNodes * sizeof(unsigned int), 3, GL_DYNAMIC_COPY);

	GLCall(glDispatchCompute(numParticles, 1, 1));
	GLCall(glMemoryBarrier(GL_ALL_BARRIER_BITS));

	int* returnedTree = (int*) treeBuffer.retrieve();
	for (int i = 0; i < numTreeNodes; i++) {
		std::cout << returnedTree[i] << std::endl;
	}

	return 0;

	while (!glfwWindowShouldClose(window)) {

		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		mainShader.Attach();
		mainShader.setUniformV4f("u_Colour", r, 1.0f - r, 0.0f, 1.0f);
		
		renderer.DrawA(&VAO, &IBO, &mainShader, GL_POINTS);


		if (r > 1.0f) {
			r = 1.0f;
			incrementing = false;
		}
		else if (r < 0.0f) {
			r = 0.0f;
			incrementing = true;
		}

		r += incrementing ? 0.01f : -0.01f;

		glfwSwapBuffers(window);
		glfwPollEvents();


		double n_cursorXPos, n_cursorYPos;
		glfwGetCursorPos(window, &n_cursorXPos, &n_cursorYPos);
		if (cursorXPos != n_cursorXPos || cursorYPos != n_cursorYPos) {
			//Mouse moved
			cameraVec.x += (n_cursorXPos - cursorXPos) * 0.0005f;
			cameraVec.y -= (n_cursorYPos - cursorYPos) * 0.0005f;
			cameraVec = glm::normalize(cameraVec);

			cursorXPos = n_cursorXPos;
			cursorYPos = n_cursorYPos;

			view = glm::lookAt(cameraPos, cameraPos + cameraVec, glm::vec3(0.0f, 1.0f, 0.0f));
			fullProj = projection * view * model;
			mainShader.setUniformM4f("projectionAllMat", fullProj);
		}

		glm::vec3 camMovement = moveCamera(window, cameraVec);
		if (camMovement != emptyMovementVector) {
			cameraPos += camMovement;

			view = glm::lookAt(cameraPos, cameraPos + cameraVec, glm::vec3(0.0f, 1.0f, 0.0f));
			fullProj = projection * view * model;
			mainShader.setUniformM4f("projectionAllMat", fullProj);
		}

	}

	return 0;
}
