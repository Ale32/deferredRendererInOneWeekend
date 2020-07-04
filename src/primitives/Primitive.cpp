#include "Primitive.h"

#include "glad/glad.h"


Primitive::Primitive()
    : _vboID(0),
    _vaoID(0),
    _iboID(0),
    _shader(nullptr)
{
}

Primitive::~Primitive()
{
    glDeleteBuffers(1, &_vboID);
    glDeleteBuffers(1, &_iboID);
    glDeleteVertexArrays(1, &_vaoID);
    
    _shader = nullptr;
}

void Primitive::_generateBuffers()
{
    const int elemsForVertex = 8;
    std::vector<float> vbData;

    // VA first!
    glGenVertexArrays(1, &_vaoID);
    glBindVertexArray(_vaoID);

    // VB
    glGenBuffers(1, &_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, _vboID);

    // Unroll all the data in a flat vector
    for (size_t i = 0; i < GetNumVertices(); ++i)
    {
        glm::vec3 position = m_Vertices[i];
        glm::vec2 uv = m_TexCoords[i];
        glm::vec3 normal = m_Normals[i];
        vbData.insert(vbData.end(), {position.x, position.y, position.z, uv.x, uv.y, normal.x, normal.y, normal.z});
    }
    glBufferData(GL_ARRAY_BUFFER, GetNumVertices() * sizeof(float) * elemsForVertex, vbData.data(), GL_STATIC_DRAW);
    
    // IB
    glGenBuffers(1, &_iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetNumIndices() * sizeof(std::uint32_t), GetIndices().data(), GL_STATIC_DRAW);
    
    // Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, elemsForVertex * sizeof(float), (void*)0);

    // UVs
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, elemsForVertex * sizeof(float), (void*)(3 * sizeof(float)));

    // Normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, elemsForVertex * sizeof(float), (void*)(5 * sizeof(float)));
}

void Primitive::Bind()
{
    glBindVertexArray(_vaoID);
}

void Primitive::UnBind()
{
    glBindVertexArray(0);
}

void Primitive::Draw()
{
    Bind();

    // Draw - passing nullptr because we are already selecting the index buffer
    glDrawElements(GL_TRIANGLES, GetNumIndices(), GL_UNSIGNED_INT, (void*)0);

    UnBind();
}
