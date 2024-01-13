#include <GL/glew.h>
#include "VertexArray.h"
#include "GLError.h"

VertexArray::VertexArray() {
	
	GlCall(glGenVertexArrays(1, &m_Renderer_id));
	GlCall(glBindVertexArray(m_Renderer_id));
}
VertexArray::~VertexArray() {
	GlCall(glDeleteVertexArrays(1, &m_Renderer_id));
}

void VertexArray::AddBuffer(VertexBuffer& vb, VertexBufferLayout& layout) {

	Bind();
	vb.Bind();
	const auto& elements = layout.getElements();

	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];

		GlCall(glEnableVertexAttribArray(i));
		GlCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*) offset));
		offset += element.count * 4;
	}
}

void VertexArray::Bind() {
	GlCall(glBindVertexArray(m_Renderer_id));
}
void VertexArray::Unbind() {
	GlCall(glBindVertexArray(0));
}
