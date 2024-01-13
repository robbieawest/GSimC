#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Shader.h"
#include "GlError.h"

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath) {
	std::ifstream stream(filePath);

	if (!stream) {
		std::cout << "ParseShader: error while reading file." << std::endl;
	}

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (std::getline(stream, line)) {

		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {

	const char* src = source.c_str();
	GlCall(unsigned int id = glCreateShader(type));
	GlCall(glShaderSource(id, 1, &src, nullptr));
	GlCall(glCompileShader(id));

	int result;
	GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE) {

		std::string typeOutput;
		if (type == GL_VERTEX_SHADER) {
			typeOutput = "GL_VERTEX_SHADER";
		}
		else if (type == GL_FRAGMENT_SHADER) {
			typeOutput = "GL_FRAGMENT_SHADER";
		}
		else {
			std::cout << "Type error, only vertex or fragment allowed.";
			return id;
		}

		std::cout << "Error while compiling shader!" << std::endl;
		std::cout << "Type: " << typeOutput << std::endl;

		int length;
		GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		char * message = (char*) alloca(length * sizeof(char));
		GlCall(glGetShaderInfoLog(id, length, &length, message));

		std::cout << message << std::endl;
	}
	

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	GlCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GlCall(glAttachShader(program, vs));
	GlCall(glAttachShader(program, fs));
	GlCall(glLinkProgram(program));
	GlCall(glValidateProgram(program));

	GlCall(glDeleteShader(vs));
	GlCall(glDeleteShader(fs));

	return program;
}

Shader::Shader(const std::string& filePath) {
	ShaderProgramSource source = ParseShader(filePath);
	m_Renderer_id = CreateShader(source.VertexSource, source.FragmentSource);
	Attach();
}

Shader::~Shader() {
	GlCall(glDeleteProgram(m_Renderer_id));
}

void Shader::Attach() {
	GlCall(glUseProgram(m_Renderer_id));
}

void Shader::Deattach() {
	GlCall(glUseProgram(0));
}

unsigned int Shader::getId() {
	return m_Renderer_id;
}


void Shader::setUniformV4f(const char* uniform, const glm::vec4 v) {

	if (uniformCache.find(uniform) == uniformCache.end()) {

		GlCall(unsigned int location = glGetUniformLocation(m_Renderer_id, uniform));
		ASSERT(location != -1);
		uniformCache.insert(std::make_pair(uniform, location));
	}

	GlCall(glUniform4f(uniformCache[uniform], v.x, v.y, v.z, v.w));
}
