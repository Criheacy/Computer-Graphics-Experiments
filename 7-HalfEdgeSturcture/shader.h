#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
	unsigned int ID;
	// 构造器读取并构建着色器
	Shader(const GLchar *vertexPath, const GLchar *geometryPath, const GLchar *fragmentPath);
	// 使用/激活程序
	void Activate() const;
	// uniform工具函数
	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string &name, float value) const;
	void SetVec2(const std::string &name, glm::vec2 value) const;
	void SetVec3(const std::string &name, glm::vec3 value) const;
	void SetVec4(const std::string &name, glm::vec4 value) const;
	void SetMat4(const std::string &name, glm::mat4 value) const;
};

#endif// SHADER_H
