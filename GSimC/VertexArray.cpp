#include <GL/glew.h>
#include "VertexArray.h"
#include "GLError.h"

VertexArray::VertexArray() {
	
	GLCall(glGenVertexArrays(1, &m_Renderer_id));
	Bind();
}
VertexArray::~VertexArray() {
	Unbind();
	GLCall(glDeleteVertexArrays(1, &m_Renderer_id));
}

void VertexArray::AddBuffer(VertexBuffer* vb, VertexBufferLayout* layout) {

	Bind();
	vb->Bind();
	const auto& elements = layout->getElements();

	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];

		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout->getStride(), (const void*) offset));
		offset += element.count * sizeof(float);
	}
}

void VertexArray::Bind() {
	GLCall(glBindVertexArray(m_Renderer_id));
}
void VertexArray::Unbind() {
	GLCall(glBindVertexArray(0));
}
