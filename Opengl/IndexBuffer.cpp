#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "IndexBuffer.h"
#include "GLError.h"

IndexBuffer::IndexBuffer(const unsigned int* data, const unsigned int count)
	: m_Renderer_id(0), m_Count(count){

	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GlCall(glGenBuffers(1, &m_Renderer_id));
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer_id));
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
	GlCall(glDeleteBuffers(1, &m_Renderer_id));
}

void IndexBuffer::Bind(){
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer_id));
}
void IndexBuffer::Unbind(){
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int IndexBuffer::getRendererID(){
	return m_Renderer_id;
}

unsigned int IndexBuffer::getCount(){
	return m_Count;
}
