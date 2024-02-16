#pragma once
#include <vector>

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

};

class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout();

	void Push(unsigned int count);
	unsigned int getStride();
	const std::vector<VertexBufferElement> getElements();
};
