#include "Shader.h"
#include "Renderer.h"

#include "glad/glad.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    : m_VertexFilePath(vertexShaderPath), m_FragmentFilePath(fragmentShaderPath), m_RendererID(0)
{
    const std::string vertexShaderSource = _parseShader(vertexShaderPath);
    const std::string fragmentShaderSource = _parseShader(fragmentShaderPath);
    m_RendererID = _createShader(vertexShaderSource, fragmentShaderSource);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

const std::string Shader::_parseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    std::string line;
    std::stringstream ss;

    while (getline(stream, line))
    {
        ss << line << "\n";
    }

    return ss.str();
}

std::uint32_t Shader::_createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    std::uint32_t program = glCreateProgram();
    std::uint32_t vs = _compileShader(GL_VERTEX_SHADER, vertexShader);
    std::uint32_t fs = _compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

std::uint32_t Shader::_compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int lenght;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);

        // alloca will allocate on the stack dynamically
        // Can't really do char message[lenght] because it's not const
        char* message = (char*)alloca(lenght * sizeof(char));
        glGetShaderInfoLog(id, lenght, &lenght, message);

        // TODO: Consolidate this print because it's assuming we only pass vertex OR fragment shaders
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader." << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(_getUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    glUniform1f(_getUniformLocation(name), value);
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& value)
{
    glUniform3f(_getUniformLocation(name), value.x, value.y, value.z);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    glUniform3f(_getUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(_getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(_getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader::_getUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_RendererID, name.c_str());

    if (location == -1)
        std::cout << "Warning: uniform " << name << " doesn't exist." << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}