#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "IndexBuffer.h"
#include "GLError.h"

IndexBuffer::IndexBuffer(const unsigned int* data, const unsigned int count)
	: m_Renderer_id(0), m_Count(count){

	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &m_Renderer_id));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer_id));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
	GLCall(glDeleteBuffers(1, &m_Renderer_id));
}

void IndexBuffer::Bind(){
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer_id));
}
void IndexBuffer::Unbind(){
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int IndexBuffer::getRendererID(){
	return m_Renderer_id;
}

unsigned int IndexBuffer::getCount(){
	return m_Count;
}
