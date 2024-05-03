#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

struct StraightLineObject{
    const char *movingShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        uniform mat4 transform;

        void main() {
            gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
    )";
    glm::mat4 transform;
    float direction[2];
    float vertices[6];
    StraightLineObject(){};
    void move();
};

struct Ship: StraightLineObject{
    Ship();
};
struct Bullet: StraightLineObject{
    Bullet();
};