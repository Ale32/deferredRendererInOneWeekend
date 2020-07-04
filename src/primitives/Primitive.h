#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <vector>
#include <memory>

class Shader;

class Primitive
{
private:
    std::uint32_t _vboID;
    std::uint32_t _vaoID;
    std::uint32_t _iboID;

    glm::mat4 _modelMatrix;
    std::shared_ptr<Shader> _shader;

protected:
    int m_NumVertices;
    int m_NumIndices;

    std::vector<int> m_Indices;
    std::vector<glm::vec3> m_Vertices;
    std::vector<glm::vec2> m_TexCoords;
    std::vector<glm::vec3> m_Normals;

    inline int GetNumVertices() const { return m_NumVertices; }
    inline int GetNumIndices() const { return m_NumIndices; }

    inline std::vector<glm::vec3> GetVertices()  const { return m_Vertices; }
    inline std::vector<int> GetIndices()         const { return m_Indices; }
    inline std::vector<glm::vec2> GetTexCoords() const { return m_TexCoords; }
    inline std::vector<glm::vec3> GetNormals()   const { return m_Normals; }

    void _generateBuffers();

public:
    Primitive();
    virtual ~Primitive();

    /**
     *  Implement this to fill the data arrays of the primitive
     */
    virtual void Construct() = 0;

    void Bind();
    void UnBind();

    void SetModelMatrix(glm::mat4 matrix) { _modelMatrix = matrix; }
    glm::mat4 GetModelMatrix() const { return _modelMatrix; }

    void SetShader(std::shared_ptr<Shader> shader) { _shader = shader; }
    std::shared_ptr<Shader> GetShader() const { return _shader; }

    void Draw();
};