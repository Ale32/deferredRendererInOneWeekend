#include "Primitive.h"

/**
 *  A sphere primitive constructed around the origin between -1.0 and 1.0 (radius 1)
 */
class Sphere :
    public Primitive
{
private:
    std::uint16_t _precision;

public:
    Sphere::Sphere(std::uint16_t precision = 32) : Primitive()
    {
        _precision = precision;

        Construct();
        _generateBuffers();
    }

    void Sphere::Construct() override
    {
        // Extra vertex needed for the uv
        m_NumVertices = (_precision + 1) * (_precision + 1);
        m_NumIndices = _precision * _precision * 6;

        // Fill the vectors
        for (int i = 0; i < m_NumVertices; i++)
        {
            m_Vertices.push_back(glm::vec3());
            m_TexCoords.push_back(glm::vec2());
            m_Normals.push_back(glm::vec3());
        }

        for (int i = 0; i < m_NumIndices; i++)
            m_Indices.push_back(0);

        // Calculating triangle vertices
        for (int i = 0; i <= _precision; i++)
        {
            for (int j = 0; j <= _precision; j++)
            {
                float y =  (float)cos(glm::radians(180.0f - i * 180.0f / _precision));
                float x = -(float)cos(glm::radians(j * 360.0f / _precision)) * (float)abs(cos(asin(y)));
                float z =  (float)sin(glm::radians(j * 360.0f / _precision)) * (float)abs(cos(asin(y)));

                m_Vertices[i * (_precision + 1) + j] = glm::vec3(x, y, z);
                m_TexCoords[i * (_precision + 1) + j] = glm::vec2(((float)j / _precision), ((float)i / _precision));
                m_Normals[i * (_precision + 1) + j] = glm::vec3(x, y, z);
            }
        }

        // Calculating triangle indices
        for (int i = 0; i < _precision; i++)
        {
            for (int j = 0; j < _precision; j++)
            {
                m_Indices[6 * (i * _precision + j) + 0] = i * (_precision + 1) + j;
                m_Indices[6 * (i * _precision + j) + 1] = i * (_precision + 1) + j + 1;
                m_Indices[6 * (i * _precision + j) + 2] = (i + 1) * (_precision + 1) + j;
                m_Indices[6 * (i * _precision + j) + 3] = i * (_precision + 1) + j + 1;
                m_Indices[6 * (i * _precision + j) + 4] = (i + 1) * (_precision + 1) + j + 1;
                m_Indices[6 * (i * _precision + j) + 5] = (i + 1) * (_precision + 1) + j;
            }
        }
    }
};