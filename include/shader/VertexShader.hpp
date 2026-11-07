#ifndef VERTEX_SHADER_HPP
#define VERTEX_SHADER_HPP

#include <cstdint>
#include "Geometry.hpp"

// 使用右手坐标系：+x向右、+y向下、+z向后
class VertexShader {
public:
    static VertexShader& GetInstance()
    {
        static VertexShader vertexShader;
        return vertexShader;
    }

    bool Init(std::int32_t windowWidth, std::int32_t windowHeight);

    void Transform(Triangle3& triangle, Triangle3& ndc, Triangle2& screen);

private:
    VertexShader() = default;
    ~VertexShader() = default;
    void InitModelMatrix();
    void InitViewMatrix();
    void InitPrespectiveMatrix();
    Point4 TransformMVP(Point4 point);
    Point4 TransformNDC(Point4 point);
    Point2 TransformScreen(Point4 point);

    std::int32_t m_width { 0 };
    std::int32_t m_height { 0 };
    bool m_isInit { false };
    Matrix4_4 m_modelMatrix;
    Matrix4_4 m_viewMatrix;
    Matrix4_4 m_prespectiveMatrix;
};

#endif