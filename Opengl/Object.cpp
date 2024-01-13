#include "Object.h"
#include "Renderer.h"
#include "GLError.h"
#include "glm/glm.hpp"


Object::Object(ObjectType type, const char* shaderPath, ObjectParameters params) 
	: position(0.0f){

	ASSERT(type == OBJECT_CUBE);

	switch (type) {
	case OBJECT_CUBE:

		float width = params.floatParam1;
		float height = params.floatParam2;
		float length = params.floatParam3;

		glm::vec3 vertexData[24]{
			glm::vec3(-0.5, 0.5, -0.5),
			glm::vec3(0.5, 0.5, -0.5),
			glm::vec3(0.5, 0.5, 0.5),
			glm::vec3(-0.5, 0.5, 0.5),
			glm::vec3(-0.5, -0.5, -0.5),
			glm::vec3(0.5, -0.5, -0.5),
			glm::vec3(0.5, -0.5, 0.5),
			glm::vec3(-0.5, -0.5, 0.5)
		};

		unsigned int indices[6 * 2 * 3]{
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
	

		m_Vertex_Array = new VertexArray();
		VertexBuffer vbo(vertexData, 24 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push(3);
		m_Vertex_Array->AddBuffer(vbo, layout);


		m_Index_Buffer = new IndexBuffer(indices, 36);
		m_Shader = new Shader(shaderPath);
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
}

void Object::Draw(Renderer* renderer) {
	m_Vertex_Array->Bind();
	m_Index_Buffer->Bind();
	m_Shader->Attach();

	renderer->Draw(m_Vertex_Array, m_Index_Buffer, m_Shader);

	m_Vertex_Array->Unbind();
	m_Index_Buffer->Unbind();
	m_Shader->Deattach();
	
}


void Object::setValue(const char* identifier, const void* data, UniformType type) {
	switch (type) {
	case VEC4:

		const glm::vec4 v = *reinterpret_cast<const glm::vec4*>(data);
		m_Shader->setUniformV4f(identifier, v);

		break;
	case MAT4:

		break;
	}
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
