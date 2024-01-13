#include <GL/glew.h>
#include <iostream>

void GlClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "Line " << line << " " << file << "\n| OpenGL ERROR | : [" << error << "] "
			<< function << std::endl;
		return false;
	}
	return true;
}
