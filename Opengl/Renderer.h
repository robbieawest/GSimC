#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer {
public:
	Renderer();
	~Renderer();

	void Draw(VertexArray* va, IndexBuffer* ib, Shader* shader);
};
