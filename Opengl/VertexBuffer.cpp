#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "VertexBuffer.h"
#include "GLError.h"

VertexBuffer::VertexBuffer(const void* data, const unsigned int size)
	: m_Renderer_id(0){
	GlCall(glGenBuffers(1, &m_Renderer_id));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_id));
	GlCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
	GlCall(glDeleteBuffers(1, &m_Renderer_id));
}

void VertexBuffer::Bind() {
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_id));
}
void VertexBuffer::Unbind() {
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

unsigned int VertexBuffer::getRendererID() {
	return m_Renderer_id;
}
