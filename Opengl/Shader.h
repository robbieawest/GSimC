#pragma once
#include <unordered_map>
#include <string>
#include "glm/glm.hpp"

class Shader {
private:
	unsigned int m_Renderer_id;
	std::unordered_map<const char*, unsigned int> uniformCache;

public:
	Shader(const std::string& filePath);
	~Shader();

	void Attach();
	void Deattach();
	unsigned int getId();

	void setUniformV4f(const char* uniform, const glm::vec4 v);
};
