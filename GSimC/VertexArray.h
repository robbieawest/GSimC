#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"


class VertexArray {
private:
	unsigned int m_Renderer_id;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(VertexBuffer*, VertexBufferLayout*);
	void Bind();
	void Unbind();
};
