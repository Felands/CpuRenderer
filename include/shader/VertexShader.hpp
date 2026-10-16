#ifndef VERTEX_SHADER_HPP
#define VERTEX_SHADER_HPP

#include "Geometry.hpp"

// 使用右手坐标系：+x向右、+y向上、+z向后
class VertexShader {
public:
    static VertexShader& GetInstance()
    {
        static VertexShader vertexShader;
        return vertexShader;
    }

    bool Init();

    void Transform(Triangle3& triangle);

private:
    VertexShader() {}
    ~VertexShader() {}
    void InitModelMatrix();
    void InitViewMatrix();
    void InitPrespectiveMatrix();
    void Transform(Point4& point);

    bool m_isInit { false };
    Matrix4_4 m_modelMatrix;
    Matrix4_4 m_viewMatrix;
    Matrix4_4 m_prespectiveMatrix;
};

#endif