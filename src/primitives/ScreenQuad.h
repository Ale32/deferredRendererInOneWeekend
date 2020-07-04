#pragma once

#include "Primitive.h"


class ScreenQuad :
    public Primitive
{
public:
    ScreenQuad::ScreenQuad() : Primitive()
    {
        Construct();
        _generateBuffers();
    }

    void ScreenQuad::Construct() override
    {
        m_NumVertices = 4;
        m_NumIndices = 6;

        // Fill the vectors
        for (int i = 0; i < m_NumVertices; i++)
        {
            m_Vertices.push_back(glm::vec3());
            m_TexCoords.push_back(glm::vec2());
            m_Normals.push_back(glm::vec3());
        }

        for (int i = 0; i < m_NumIndices; i++)
            m_Indices.push_back(0);

        const float z = 0.f;
        m_Vertices[0] = glm::vec3(-1.f, -1.f, z);
        m_Vertices[1] = glm::vec3( 1.f, -1.f, z);
        m_Vertices[2] = glm::vec3( 1.f,  1.f, z);
        m_Vertices[3] = glm::vec3(-1.f,  1.f, z);

        m_TexCoords[0] = glm::vec2(0.f, 0.f);
        m_TexCoords[1] = glm::vec2(1.f, 0.f);
        m_TexCoords[2] = glm::vec2(1.f, 1.f);
        m_TexCoords[3] = glm::vec2(0.f, 1.f);

        // Completely useless honestly - just to use the parent class
        m_Normals[0] = glm::vec3(0.f, 0.f, 1.f);
        m_Normals[1] = m_Normals[0];
        m_Normals[2] = m_Normals[0];
        m_Normals[3] = m_Normals[0];

        m_Indices[0] = 0;
        m_Indices[1] = 1;
        m_Indices[2] = 2;
        m_Indices[3] = 0;
        m_Indices[4] = 2;
        m_Indices[5] = 3;
    }
};