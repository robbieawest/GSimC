#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

struct ObjectParameters {
	float floatParam1;
	float floatParam2;
	float floatParam3;
};

enum ObjectType {
	OBJECT_CUBE,
	OBJECT_SPHERE
};

enum UniformType {
	VEC4,
	MAT4
};

class Renderer;

class Object {
private:
	const glm::vec3* vertices;
	const unsigned int* indices;

	VertexArray* m_Vertex_Array;
	IndexBuffer* m_Index_Buffer;
	VertexBuffer* m_Vertex_Buffer;
	VertexBufferLayout* m_Vertex_Layout;

	Shader* m_Shader;
	glm::vec3 position;


public:
	Object(ObjectType type, const char* shaderPath, ObjectParameters params);
	~Object();

	void Draw(Renderer* renderer);

	void setPosition(float x, float y, float z);
	void setPosition(glm::vec3 pos);
	glm::vec3 getPosition();


	void setColour(float r, float g, float b, float a, std::string identifier);
	void setColour(glm::vec4 col, std::string identifier);

	void setMat(glm::mat4 mat, std::string identifier);

	
};
