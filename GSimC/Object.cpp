#include "Object.h"
#include "Renderer.h"
#include "GLError.h"
#include "glm/glm.hpp"


Object::Object(ObjectType type, const char* shaderPath, ObjectParameters params) 
	: position(0.0f){

	ASSERT(type == OBJECT_CUBE);

	std::string shaderStr = "Resources/Shaders/" + std::string(shaderPath);

	switch (type) {
	case OBJECT_CUBE:
		
		float width = params.floatParam1;
		float height = params.floatParam2;
		float length = params.floatParam3;
		
		vertices = new glm::vec3[8]{
			glm::vec3(-0.5, 0.5, -0.5),
			glm::vec3(0.5, 0.5, -0.5),
			glm::vec3(0.5, 0.5, 0.5),
			glm::vec3(-0.5, 0.5, 0.5),
			glm::vec3(-0.5, -0.5, -0.5),
			glm::vec3(0.5, -0.5, -0.5),
			glm::vec3(0.5, -0.5, 0.5),
			glm::vec3(-0.5, -0.5, 0.5)
		};


		indices = new unsigned int[6 * 2 * 3]{
			1, 2, 3, //top face
			1, 3, 4,
			2, 3, 7, //right face
			2, 6, 7,
			1, 4, 8, //left face
			1, 5, 8,
			1, 2, 6, //back face
			1, 5, 6,
			3, 4, 7, //front face
			4, 7, 8,
			5, 6, 8, //bottom face
			6, 8, 7
		};
		
		/*
		float vertices01[] = {
		  -1.0f,1.0f,0.0f,
		  -1.0f,-1.0f,0.0f,
		  1.0f,1.0f,0.0f,
		  1.0f,-1.0f,0.0f,
		  -1.0f,1.0f,-1.0f,
		  -1.0f,-1.0f,-1.0f,
		  1.0f,1.0f,-1.0f,
		  1.0f,-1.0f,-1.0f
		};
		unsigned int indices01[] = {
		  0, 2, 3, 0, 3, 1,
		  2, 6, 7, 2, 7, 3,
		  6, 4, 5, 6, 5, 7,
		  4, 0, 1, 4, 1, 5,
		  0, 4, 6, 0, 6, 2,
		  1, 5, 7, 1, 7, 3,
		};
		*/
		m_Vertex_Array = new VertexArray();
		m_Index_Buffer = new IndexBuffer(indices, 36);
		m_Vertex_Buffer = new VertexBuffer(vertices, 24 * sizeof(float));

		m_Vertex_Layout = new VertexBufferLayout();
		m_Vertex_Layout->Push(3);
		m_Vertex_Array->AddBuffer(m_Vertex_Buffer, m_Vertex_Layout);

		m_Shader = new Shader(shaderStr);
	}


	m_Shader->Deattach();
	m_Vertex_Array->Unbind();
	m_Index_Buffer->Unbind();
}

Object::~Object() {
	m_Vertex_Array->Unbind();
	m_Index_Buffer->Unbind();
	m_Shader->Deattach();

	delete(m_Shader);
	delete(m_Vertex_Array);
	delete(m_Index_Buffer);
	delete(m_Vertex_Buffer);
	delete(m_Vertex_Layout);
}

void Object::Draw(Renderer* renderer) {
	renderer->Draw(m_Vertex_Array, m_Index_Buffer, m_Shader, 0x004);
}



void Object::setPosition(float x, float y, float z) {
	position = glm::vec3(x, y, z);
}

void Object::setPosition(glm::vec3 pos) {
	position = pos;
}

glm::vec3 Object::getPosition() {
	return position;
}


void Object::setColour(float r, float g, float b, float a, std::string identifier) {
	m_Shader->setUniformV4f(identifier, r, g, b, a);
}
void Object::setColour(glm::vec4 col, std::string identifier) {
	m_Shader->setUniformV4f(identifier, col.x, col.y, col.z, col.w);
}

void Object::setMat(glm::mat4 mat, std::string identifier) {
	m_Shader->setUniformM4f(identifier, mat);
}
