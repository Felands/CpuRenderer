#include "VertexShader.hpp"
#include "Logger.hpp"

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
    return true;
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

// Scale the model down by a factor of 2, rotate it 90 degrees to the left around the y-axis, shift it 500 pixels in the positive x-direction, and place it into the space.
void VertexShader::InitModelMatrix()
{
    m_modelMatrix = TRANSLATION_RIGHT_500_MATRIX_4 * (ROTATION_LEFT_90_MATRIX_4 * SCALE_HALF_MATRIX_4);
}

// Using the camera's focus as the center, take the opposite of the camera's viewing direction as the z-axis, and then add the top of the camera to construct the view matrix.
void VertexShader::InitViewMatrix()
{
    static Vector<3> camera { -10, 8, 5 };
    static Vector<3> focus { 0, 0, 0 };
    static Vector<3> up { 0, 1, 0 };
    OrthonormalBasis3 basis = BuildOrthonormalBasis(up, camera - focus);
    Vector<3> xAxis = basis.xAxis;
    Vector<3> yAxis = basis.yAxis;
    Vector<3> zAxis = basis.zAxis;

    Matrix4_4 move = {
        {
            { 1, 0, 0, -focus.x },
            { 0, 1, 0, -focus.y },
            { 0, 0, 1, -focus.z },
            { 0, 0, 0, 1 }
        }
    };
    Matrix4_4 transform = {
        {
            { xAxis.x, xAxis.y, xAxis.z, 0 },
            { yAxis.x, yAxis.y, yAxis.z, 0 },
            { zAxis.x, zAxis.y, zAxis.z, 0 },
            { 0, 0, 0, 1 }
        }
    };
    m_viewMatrix = transform * move;
};

void VertexShader::InitPrespectiveMatrix()
{
    m_prespectiveMatrix = {
        {
            { 1, 0, 0, 0 },
            { 0, 1, 0, 0 },
            { 0, 0, 1, 0 },
            { 0, 0, -1 / 2, 1 }
        }
    };
}
