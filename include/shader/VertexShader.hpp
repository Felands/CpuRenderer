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

    void Init()
    {
        if (isInit) {
            return;
        }

        InitModelMatrix();
        InitViewMatrix();
        InitPrespectiveMatrix();

        isInit = true;
    }

    void Transform();

private:
    void InitModelMatrix();
    void InitViewMatrix();
    void InitPrespectiveMatrix();

    bool isInit { false };
    Matrix4_4 modelMatrix;
    Matrix4_4 viewMatrix;
    Matrix4_4 prespectiveMatrix;
};

#endif