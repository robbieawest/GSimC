#include <GL/glew.h>
#include "Renderer.h"
#include "GLError.h"

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::Draw(VertexArray* va, IndexBuffer* ib, Shader* shader) {

	shader->Attach();
	ib->Bind();
	va->Bind();
	GlCall(glDrawElements(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr));
}

