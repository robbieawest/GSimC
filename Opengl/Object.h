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
	VertexArray* m_Vertex_Array;
	IndexBuffer* m_Index_Buffer;
	Shader* m_Shader;
	glm::vec3 position;

public:
	Object(ObjectType, const char* shaderPath, ObjectParameters params);
	~Object();

	void Draw(Renderer* renderer);
	void setValue(const char* identifier, const void* data, UniformType type);
	void setPosition(float x, float y, float z);
	void setPosition(glm::vec3 pos);
	
	glm::vec3 getPosition();
};
