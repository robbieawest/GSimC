#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "VertexBuffer.h"
#include "GLError.h"

VertexBuffer::VertexBuffer(const void* data, const unsigned int size)
	: m_Renderer_id(0){
	GLCall(glGenBuffers(1, &m_Renderer_id));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_id));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
	Unbind();
	GLCall(glDeleteBuffers(1, &m_Renderer_id));
}

void VertexBuffer::Bind() {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_id));
}
void VertexBuffer::Unbind() {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

unsigned int VertexBuffer::getRendererID() {
	return m_Renderer_id;
}
