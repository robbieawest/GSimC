#pragma once
#include <unordered_map>
#include <string>
#include "glm/glm.hpp"

class Shader {
private:
	unsigned int m_Renderer_id;
	std::unordered_map<const char*, unsigned int> uniformCache;

public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	Shader(const std::string& computePath);
	~Shader();

	void Attach();
	void Deattach();
	unsigned int getId();

	void setUniformV4f(const std::string& identifier, float v1, float v2, float v3, float v4);
	void setUniformV3f(const std::string& identifier, float v1, float v2, float v3);
	void setUniformM4f(const std::string& identifier, glm::mat4 mat);
};
