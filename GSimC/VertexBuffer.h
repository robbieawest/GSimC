#pragma once

class VertexBuffer {
private:
	unsigned int m_Renderer_id;
public:
	VertexBuffer(const void* data, const unsigned int size);
	~VertexBuffer();

	void Bind();
	void Unbind();

	unsigned int getRendererID();

};
