#pragma once

#include "Primitive.h"


class Cube :
    public Primitive
{
public:
    Cube::Cube() : Primitive()
    {
        Construct();
        _generateBuffers();
    }

    void Cube::Construct() override
    {
        const int faces = 6;
        m_NumVertices = 4 * faces;
        m_NumIndices = 6 * faces;

        // Fill the vectors
        for (int i = 0; i < m_NumVertices; i++)
        {
            m_Vertices.push_back(glm::vec3());
            m_TexCoords.push_back(glm::vec2());
            m_Normals.push_back(glm::vec3());
        }

        m_Indices.reserve(m_NumIndices);

        const float x = 0.5f;
        const float y = 0.5f;
        const float z = 0.5f;
       
        // front - 012 023
        m_Indices.insert(m_Indices.begin(), {0, 1, 2, 0, 2, 3});

        m_Vertices[0] = glm::vec3(-x, -y, z);    m_TexCoords[0] = glm::vec2(0, 0); m_Normals[0] = glm::vec3(0.f, 0.f, 1.f);
        m_Vertices[1] = glm::vec3( x, -y, z);    m_TexCoords[1] = glm::vec2(1, 0); m_Normals[1] = glm::vec3(0.f, 0.f, 1.f);
        m_Vertices[2] = glm::vec3( x,  y, z);    m_TexCoords[2] = glm::vec2(1, 1); m_Normals[2] = glm::vec3(0.f, 0.f, 1.f);
        m_Vertices[3] = glm::vec3(-x,  y, z);    m_TexCoords[3] = glm::vec2(0, 1); m_Normals[3] = glm::vec3(0.f, 0.f, 1.f);

        // right - 456 467
        m_Indices.insert(m_Indices.begin() + 6, { 4, 5, 6, 4, 6, 7 });

        m_Vertices[4] = glm::vec3(x, -y,  z);    m_TexCoords[4] = glm::vec2(0, 0); m_Normals[4] = glm::vec3(1.f, 0.f, 0.f);
        m_Vertices[5] = glm::vec3(x, -y, -z);    m_TexCoords[5] = glm::vec2(1, 0); m_Normals[5] = glm::vec3(1.f, 0.f, 0.f);
        m_Vertices[6] = glm::vec3(x,  y, -z);    m_TexCoords[6] = glm::vec2(1, 1); m_Normals[6] = glm::vec3(1.f, 0.f, 0.f);
        m_Vertices[7] = glm::vec3(x,  y,  z);    m_TexCoords[7] = glm::vec2(0, 1); m_Normals[7] = glm::vec3(1.f, 0.f, 0.f);

        // back - 8910 81011
        m_Indices.insert(m_Indices.begin() + 12, { 8, 9, 10, 8, 10, 11 });

        m_Vertices[8]  = glm::vec3( x, -y, -z);  m_TexCoords[8]  = glm::vec2(0, 0); m_Normals[8]  = glm::vec3(0.f, 0.f, -1.f);
        m_Vertices[9]  = glm::vec3(-x, -y, -z);  m_TexCoords[9]  = glm::vec2(1, 0); m_Normals[9]  = glm::vec3(0.f, 0.f, -1.f);
        m_Vertices[10] = glm::vec3(-x,  y, -z);  m_TexCoords[10] = glm::vec2(1, 1); m_Normals[10] = glm::vec3(0.f, 0.f, -1.f);
        m_Vertices[11] = glm::vec3( x,  y, -z);  m_TexCoords[11] = glm::vec2(0, 1); m_Normals[11] = glm::vec3(0.f, 0.f, -1.f);

        // left - 121314 121415
        m_Indices.insert(m_Indices.begin() + 18, { 12, 13, 14, 12, 14, 15 });

        m_Vertices[12] = glm::vec3(-x, -y, -z);   m_TexCoords[12] = glm::vec2(0, 0); m_Normals[12] = glm::vec3(-1.f, 0.f, 0.f);
        m_Vertices[13] = glm::vec3(-x, -y,  z);   m_TexCoords[13] = glm::vec2(1, 0); m_Normals[13] = glm::vec3(-1.f, 0.f, 0.f);
        m_Vertices[14] = glm::vec3(-x,  y,  z);   m_TexCoords[14] = glm::vec2(1, 1); m_Normals[14] = glm::vec3(-1.f, 0.f, 0.f);
        m_Vertices[15] = glm::vec3(-x,  y, -z);   m_TexCoords[15] = glm::vec2(0, 1); m_Normals[15] = glm::vec3(-1.f, 0.f, 0.f);

        // top - 161718 161819
        m_Indices.insert(m_Indices.begin() + 24, { 16, 17, 18, 16, 18, 19 });

        m_Vertices[16] = glm::vec3(-x, y,  z);   m_TexCoords[16] = glm::vec2(0, 0); m_Normals[16] = glm::vec3(0.f, 1.f, 0.f);
        m_Vertices[17] = glm::vec3( x, y,  z);   m_TexCoords[17] = glm::vec2(1, 0); m_Normals[17] = glm::vec3(0.f, 1.f, 0.f);
        m_Vertices[18] = glm::vec3( x, y, -z);   m_TexCoords[18] = glm::vec2(1, 1); m_Normals[18] = glm::vec3(0.f, 1.f, 0.f);
        m_Vertices[19] = glm::vec3(-x, y, -z);   m_TexCoords[19] = glm::vec2(0, 1); m_Normals[19] = glm::vec3(0.f, 1.f, 0.f);

        // bottom - 202122 202223
        m_Indices.insert(m_Indices.begin() + 30, { 20, 21, 22, 20, 22, 23 });

        m_Vertices[20] = glm::vec3(-x, -y, -z);  m_TexCoords[20] = glm::vec2(0, 0); m_Normals[20] = glm::vec3(0.f, -1.f, 0.f);
        m_Vertices[21] = glm::vec3( x, -y, -z);  m_TexCoords[21] = glm::vec2(1, 0); m_Normals[21] = glm::vec3(0.f, -1.f, 0.f);
        m_Vertices[22] = glm::vec3( x, -y,  z);  m_TexCoords[22] = glm::vec2(1, 1); m_Normals[22] = glm::vec3(0.f, -1.f, 0.f);
        m_Vertices[23] = glm::vec3(-x, -y,  z);  m_TexCoords[23] = glm::vec2(0, 1); m_Normals[23] = glm::vec3(0.f, -1.f, 0.f);
    }
};
