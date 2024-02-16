#pragma once

class IndexBuffer {
private:
	unsigned int m_Renderer_id;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, const unsigned int count);
	~IndexBuffer();

	void Bind();
	void Unbind();

	unsigned int getRendererID();
	unsigned int getCount();

};
