#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Shader.h"
#include "GlError.h"
#include "glm/gtc/type_ptr.hpp"

/*
struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
	std::string ComputeSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath) {
	std::ifstream stream(filePath);

	if (!stream) {
		std::cout << "ParseShader: error while reading file: " << filePath << std::endl;
	}

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1, COMPUTE = 2
	};

	std::string line;
	std::stringstream ss[3];
	ShaderType type = ShaderType::NONE;

	while (std::getline(stream, line)) {

		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
			else if (line.find("compute") != std::string::npos) {
				type = ShaderType::COMPUTE;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str(), ss[2].str()};
}
*/

static std::string ParseShader(const std::string filePath) {
	std::ifstream stream("Resources/Shaders/" + filePath);

	if (!stream) {
		std::cout << "ParseShader: error while reading file: " << filePath << std::endl;
	}

	std::stringstream ss[1];

	std::string line;
	while (std::getline(stream, line)) {
		ss[0] << line << "\n";
	}

	return ss[0].str();
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {

	const char* src = source.c_str();
	GLCall(unsigned int id = glCreateShader(type));
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE) {

		std::string typeOutput;
		if (type == GL_VERTEX_SHADER) {
			typeOutput = "GL_VERTEX_SHADER";
		}
		else if (type == GL_FRAGMENT_SHADER) {
			typeOutput = "GL_FRAGMENT_SHADER";
		}
		else if (type == GL_COMPUTE_SHADER) {
			typeOutput = "GL_COMPUTE_SHADER";
		}
		else {
			std::cout << "Type error, only vertex, compute or fragment allowed.";
			return id;
		}

		std::cout << "Error while compiling shader!" << std::endl;
		std::cout << "Type: " << typeOutput << std::endl;

		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		char * message = (char*) alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		std::cout << message << std::endl;
	}
	

	return id;
}

static void linkProgram(GLuint program) {
	GLCall(glLinkProgram(program));

	GLint linkSuccess;
	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess));
	if (linkSuccess == GL_FALSE) {

		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<char> infoLog(logLength);
		glGetProgramInfoLog(program, logLength, nullptr, infoLog.data());

		std::cout << __FILE__ << " " << __LINE__ << " " << "Compute Shader linking error" << std::endl;

		if (infoLog.data() == nullptr) {
			std::cout << "No log data could be retrieved" << std::endl;
		}
		else {
			std::cout << infoLog.data() << std::endl;
		}
	}
}

static void validateProgram(GLuint program) {
	GLCall(glValidateProgram(program));

	GLint validateSuccess;
	GLCall(glGetProgramiv(program, GL_VALIDATE_STATUS, &validateSuccess));
	if (validateSuccess == GL_FALSE) {

		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<char> infoLog(logLength);
		glGetProgramInfoLog(program, logLength, nullptr, infoLog.data());

		std::cout << __FILE__ << " " << __LINE__ << " " << "Compute Shader validation error" << std::endl;

		if (infoLog.data() == nullptr) {
			std::cout << "No log data could be retrieved" << std::endl;
		}
		else {
			std::cout << infoLog.data() << std::endl;
		}
	}
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));

	linkProgram(program);
	validateProgram(program);

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

static unsigned int CreateShader(const std::string& path) {
	GLCall(unsigned int program = glCreateProgram());
	unsigned int cs = CompileShader(GL_COMPUTE_SHADER, path);

	GLCall(glAttachShader(program, cs));

	linkProgram(program);
	validateProgram(program);

	GLCall(glDeleteShader(cs));

	return program;
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {

	std::string vertexSource = ParseShader(vertexPath);
	std::string fragmentSource = ParseShader(fragmentPath);

	m_Renderer_id = CreateShader(vertexSource, fragmentSource);
}

Shader::Shader(const std::string& computePath) {

	std::string computeSource = ParseShader(computePath);

	m_Renderer_id = CreateShader(computeSource);
}


Shader::~Shader() {
	GLCall(glDeleteProgram(m_Renderer_id));
}

void Shader::Attach() {
	GLCall(glUseProgram(m_Renderer_id));
}

void Shader::Deattach() {
	GLCall(glUseProgram(0));
}

unsigned int Shader::getId() {
	return m_Renderer_id;
}


void Shader::setUniformV4f(const std::string& identifier, float v1, float v2, float v3, float v4) {

	const char* ident = identifier.c_str();
	
	unsigned int location;
	if (uniformCache.find(ident) == uniformCache.end()) {

		GLCall(location = glGetUniformLocation(m_Renderer_id, ident));
		ASSERT(location != -1);
		uniformCache.insert(std::make_pair(ident, location));
	}
	else {
		location = uniformCache[ident];
	}
	
	GLCall(glUniform4f(location, v1, v2, v3, v4));
}

void Shader::setUniformV3f(const std::string& identifier, float v1, float v2, float v3) {

	const char* ident = identifier.c_str();
	
	unsigned int location;
	if (uniformCache.find(ident) == uniformCache.end()) {

		GLCall(location = glGetUniformLocation(m_Renderer_id, ident));
		ASSERT(location != -1); //Uniform location not found
		uniformCache.insert(std::make_pair(ident, location));
	}
	else {
		location = uniformCache[ident];
	}
	
	GLCall(glUniform3f(location, v1, v2, v3));
}

void Shader::setUniformM4f(const std::string& identifier, glm::mat4 mat) {
	
	const char* ident = identifier.c_str();
	
	unsigned int location;
	if (uniformCache.find(ident) == uniformCache.end()) {

		GLCall(location = glGetUniformLocation(m_Renderer_id, ident));
		ASSERT(location != -1);
		uniformCache.insert(std::make_pair(ident, location));
	}
	else {
		location = uniformCache[ident];
	}
	
	GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat)));
}

