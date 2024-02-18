#include <GL/glew.h>
#include "SSBO.h"
#include "GlError.h"
#include "glm/gtc/type_ptr.hpp"

SSBO::SSBO(const void* data, unsigned int size, unsigned int bi, unsigned int usage) 
	: binding_index(bi), buffer_size(size) {

	GLCall(glGenBuffers(1, &m_Renderer_id));
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Renderer_id));
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usage);
	Bind();

}

SSBO::~SSBO() {
	Unbind();
	GLCall(glDeleteBuffers(1, &m_Renderer_id));
}

void SSBO::Bind() {
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Renderer_id));
	GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_index, m_Renderer_id));
}

void SSBO::Unbind() {
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

void* SSBO::retrieve() {
	
	Bind();
	void* data = malloc(buffer_size);
	GLCall(glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, buffer_size, data));

	return data;
}
