#include "Ship.h"
#include <cstdlib>
#include <time.h>
#include <cmath>



Ship::Ship(){
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