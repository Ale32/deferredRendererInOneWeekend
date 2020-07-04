#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"


class Shader
{
private:
    std::string m_VertexFilePath;
    std::string m_FragmentFilePath;
    std::uint32_t m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform3f(const std::string& name, const glm::vec3& value);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
    const std::string _parseShader(const std::string& filepath);
    std::uint32_t _createShader(const std::string& vertexShader, const std::string& fragmentShader);
    std::uint32_t _compileShader(unsigned int type, const std::string& source);
    int _getUniformLocation(const std::string& name);
};