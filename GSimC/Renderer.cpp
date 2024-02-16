#include <GL/glew.h>
#include "Renderer.h"
#include "GLError.h"

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::Draw(VertexArray* va, IndexBuffer* ib, Shader* shader, unsigned int type) {

	va->Bind();
	ib->Bind();
	shader->Attach();
	GLCall(glDrawElements(type, ib->getCount(), GL_UNSIGNED_INT, 0));
}

void Renderer::DrawA(VertexArray* va, IndexBuffer* ib, Shader* shader, unsigned int type) {
	
	va->Bind();
	ib->Bind();
	shader->Attach();
	GLCall(glDrawArrays(type, 0, ib->getCount()));
}

