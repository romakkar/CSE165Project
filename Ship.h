#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

struct Ship;
struct Bullet;

struct StraightLineObject{
    friend struct Ship;
    friend struct Bullet;
private:
    const char *movingShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        uniform mat4 transform;

        void main() {
            gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
    )";
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
        "}\n\0";
    unsigned int moveShader, moveProgram, fragmentShader;
    unsigned int VBO, VAO;
    glm::mat4 transform;
    float direction[2];
    float vertices[6];
public:
    StraightLineObject();
    float* getVertices(){return vertices;};
    float* getDirection(){return direction;};
    virtual void createVertex() = 0;
    glm::mat4 getTransform(){return transform;};
    void move();
    unsigned int getProgram(){return moveProgram;};
    unsigned int* getVAO(){return &VAO;};
    unsigned int* getVBO(){return &VBO;};
};

struct Ship: StraightLineObject{
    void createVertex() override;
    Ship();
    ~Ship(){};
};
struct Bullet: StraightLineObject{
    void createVertex() override;
    Bullet();
    ~Bullet(){};
};