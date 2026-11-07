#include "VertexShader.hpp"
#include "Logger.hpp"

bool VertexShader::Init(std::int32_t width, std::int32_t height)
{
    if (m_isInit) {
        LOG_WARN("Vertex shader has been initialized.");
        return true;
    }

    m_width = width;
    m_height = height;

    InitModelMatrix();
    InitViewMatrix();
    InitPrespectiveMatrix();

    m_isInit = true;
    LOG_INFO("Vertex shader initialization successful.");
    return true;
}

void VertexShader::Transform(Triangle3& triangle, Triangle3& ndc, Triangle2& screen)
{
    Point4 tmpA { triangle.vertexA.x, triangle.vertexA.y, triangle.vertexA.z, 1 };
    Point4 tmpB { triangle.vertexB.x, triangle.vertexB.y, triangle.vertexB.z, 1 };
    Point4 tmpC { triangle.vertexC.x, triangle.vertexC.y, triangle.vertexC.z, 1 };

    ndc.vertexA = TransformNDC(TransformMVP(tmpA));
    ndc.vertexB = TransformNDC(TransformMVP(tmpB));
    ndc.vertexC = TransformNDC(TransformMVP(tmpC));

    screen.vertexA = TransformScreen(ndc.vertexA);
    screen.vertexB = TransformScreen(ndc.vertexB);
    screen.vertexC = TransformScreen(ndc.vertexC);
}

Point4 VertexShader::TransformMVP(Point4 point)
{
    Point4 ret = m_prespectiveMatrix * (m_viewMatrix * (m_modelMatrix * point));
    return ret;
}

Point4 VertexShader::TransformNDC(Point4 point)
{
    Point4 ret = point / point.w;
    return ret;
}

Point2 VertexShader::TransformScreen(Point4 point)
{
    static double horizontalMargin = m_width / 16.0;
    static double verticalMargin = m_height / 16.0;
    static double viewWidth = m_width * 7 / 8.0;
    static double viewHeight = m_height * 7 / 8.0;
    static Matrix4_4 screenMatrix = {
        {
            { viewWidth / 2.0, 0, 0, horizontalMargin + viewWidth / 2.0 },
            { 0, viewHeight / 2.0, 0, verticalMargin + viewHeight / 2.0 },
            { 0, 0, 1, 0 },
            { 0, 0, 0, 1 }
        }
    };

    Point4 screenPoint = screenMatrix * point;
    return { screenPoint.x, screenPoint.y };
}

// Scale the model down by a factor of 2, rotate it 90 degrees to the left around the y-axis, shift it 500 pixels in the positive x-direction, and place it into the space.
void VertexShader::InitModelMatrix()
{
    // m_modelMatrix = TRANSLATION_RIGHT_500_MATRIX_4 * (ROTATION_LEFT_90_MATRIX_4 * SCALE_HALF_MATRIX_4);
    m_modelMatrix = IDENTITY_MATRIX_4;
}

// Using the camera's focus as the center, take the opposite of the camera's viewing direction as the z-axis, and then add the top of the camera to construct the view matrix.
void VertexShader::InitViewMatrix()
{
    static Vector<3> camera { -1, 0, 2 };
    static Vector<3> focus { 0, 0, 0 };
    static Vector<3> up { 0, 1, 0 };
    OrthonormalBasis3 basis = BuildOrthonormalBasis(up, camera - focus);
    Vector<3> xAxis = basis.xAxis;
    Vector<3> yAxis = basis.yAxis;
    Vector<3> zAxis = basis.zAxis;

    Matrix4_4 matrix1 = {
        {
            { 1, 0, 0, -focus.x },
            { 0, 1, 0, -focus.y },
            { 0, 0, 1, -focus.z },
            { 0, 0, 0, 1 }
        }
    };
    Matrix4_4 matrix2 = {
        {
            { xAxis.x, xAxis.y, xAxis.z, 0 },
            { yAxis.x, yAxis.y, yAxis.z, 0 },
            { zAxis.x, zAxis.y, zAxis.z, 0 },
            { 0, 0, 0, 1 }
        }
    };
    m_viewMatrix = matrix2 * matrix1;
};

void VertexShader::InitPrespectiveMatrix()
{
    m_prespectiveMatrix = {
        {
            { 1, 0, 0, 0 },
            { 0, 1, 0, 0 },
            { 0, 0, 1, 0 },
            { 0, 0, -1.0 / 2.236, 1 }
        }
    };
}
