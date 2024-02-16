#pragma once

class SSBO {
private:
	unsigned int m_Renderer_id;
	unsigned int binding_index;
	unsigned int buffer_size;

public:
	SSBO(const void* data, unsigned int size, unsigned int bi, unsigned int usage);
	~SSBO();

	void Bind();
	void Unbind();
	void* retrieve();

};
