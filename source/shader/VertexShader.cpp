#include "VertexShader.hpp"
#include "Log.hpp"

bool VertexShader::Init()
{
    if (m_isInit) {
        LOG_WARN("Vertex shader has been initialized.");
        return true;
    }

    InitModelMatrix();
    InitViewMatrix();
    InitPrespectiveMatrix();

    m_isInit = true;
    LOG_INFO("Vertex shader initialization successful.");
}

void VertexShader::Transform(Triangle3& triangle)
{
    Point4 tmpA { triangle.vertexA.x, triangle.vertexA.y, triangle.vertexA.z, 1 };
    Transform(tmpA);
    triangle.vertexA.x = tmpA.x;
    triangle.vertexA.y = tmpA.y;
    triangle.vertexA.z = tmpA.z;

    Point4 tmpB { triangle.vertexB.x, triangle.vertexB.y, triangle.vertexB.z, 1 };
    Transform(tmpB);
    triangle.vertexB.x = tmpB.x;
    triangle.vertexB.y = tmpB.y;
    triangle.vertexB.z = tmpB.z;

    Point4 tmpC { triangle.vertexC.x, triangle.vertexC.y, triangle.vertexC.z, 1 };
    Transform(tmpC);
    triangle.vertexC.x = tmpC.x;
    triangle.vertexC.y = tmpC.y;
    triangle.vertexC.z = tmpC.z;
}

void VertexShader::Transform(Point4& point)
{
    point = m_prespectiveMatrix * (m_viewMatrix * (m_modelMatrix * point));
}

void VertexShader::InitModelMatrix()
{
    // Scale the model down by a factor of 2, rotate it 90 degrees to the left around the y-axis, shift it 500 pixels in the positive x-direction, and place it into the space.
    m_modelMatrix = TRANSLATION_RIGHT_500_MATRIX_4 * (ROTATION_LEFT_90_MATRIX_4 * SCALE_HALF_MATRIX_4);
}

void VertexShader::InitViewMatrix()
{
    static Vector<3> cameraPosition { -10, 8, 5 };
    static Vector<3> focusPositin { 0, 0, 0 };
    Vector<3> direction = focusPositin - cameraPosition;
    m_viewMatrix = {
        {
            { 1, 0, 0, -direction.x },
            { 0, 1, 0, -direction.y },
            { 0, 0, 1, -direction.z },
            { 0, 0, 0, 1 }
        }
    };
};

void VertexShader::InitPrespectiveMatrix()
{
    m_prespectiveMatrix = IDENTITY_MATRIX_4;
    return;
}
