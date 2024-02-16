#include <GL/glew.h>
#include "VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout()
	: m_Stride(0) {

}

void VertexBufferLayout::Push(unsigned int count) {
	m_Elements.push_back({ GL_FLOAT, count, GL_FALSE } );
	m_Stride += sizeof(GL_FLOAT) * count;
}

unsigned int VertexBufferLayout::getStride() {
	return m_Stride;
}

const std::vector<VertexBufferElement> VertexBufferLayout::getElements() {
	return m_Elements;
}
