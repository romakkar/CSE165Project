#include "Ship.h"
#include <cstdlib>
#include <time.h>
#include <cmath>

StraightLineObject::StraightLineObject(){
    moveShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(moveShader, 1, &movingShaderSource, NULL);
    glCompileShader(moveShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glGetShaderiv(moveShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(moveShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    moveProgram = glCreateProgram();
    glAttachShader(moveProgram, moveShader);
    glAttachShader(moveProgram, fragmentShader);
    glLinkProgram(moveProgram);
    // check for linking errors
    glGetProgramiv(moveProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(moveProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(moveShader);
    glDeleteShader(fragmentShader);
};

void Ship::createVertex(){
    srand(static_cast<unsigned int>(time(0)));
    if(rand() % 2 == 0){
        if(rand() % 2 == 0){
            vertices[0] = -0.8f - (rand() % 101)/1000.0f;
            vertices[1] = 0.9f - (rand() % 91)/50.0f;
        }
        else{
            vertices[0] = 0.8f + (rand() % 101)/1000.0f;
            vertices[1] = 0.9f - (rand() % 91)/50.0f;
        }
    }
    else{
        if(rand() % 2 == 0){
            vertices[1] = -0.8f - (rand() % 101)/1000.0f;
            vertices[0] = 0.9f - (rand() % 91)/50.0f;
        }
        else{
            vertices[1] = 0.8f + (rand() % 101)/1000.0f;
            vertices[0] = 0.9f - (rand() % 91)/50.0f;
        }
    }
    float magnitude = sqrt(vertices[0] * vertices[0] + vertices[1] * vertices[1]);
    direction[0] = vertices[0] / magnitude * 0.01f;
    direction[1] = vertices[1] / magnitude * 0.01f;
    vertices[2] = vertices[0] + 10 * direction[0] - 4 * direction[1];
    vertices[3] = vertices[1] + 10 * direction[1] + 4 * direction[0];
    vertices[4] = vertices[0] + 10 * direction[0] + 4 * direction[1];
    vertices[5] = vertices[1] + 10 * direction[1] - 4 * direction[0];
}

Ship::Ship(){
    createVertex();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    transform = glm::mat4(1.0f);
};

void StraightLineObject::move(){
    vertices[0] -= direction[0]/10;
    vertices[1] -= direction[1]/10;
    vertices[2] -= direction[0]/10;
    vertices[3] -= direction[1]/10;
    vertices[4] -= direction[0]/10;
    vertices[5] -= direction[1]/10;
    transform = glm::translate(transform, glm::vec3(-direction[0]/10, -direction[1]/10, 0.0f));
}