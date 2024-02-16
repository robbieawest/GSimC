#include <GL/glew.h>
#include <iostream>

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		std::cout << "Line " << line << " " << file << "\n| OpenGL ERROR | : [" << error << "] "
			<< function << std::endl;
	}
	return true;
}
