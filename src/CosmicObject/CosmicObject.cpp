#include "../../include/CosmicObject.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;

CosmicObject::CosmicObject(const char * name, float radius, float orbitRadius, float orbitSpeed, float rotationSpeed, string texturePath, vec3 mMatrix): Sphere(radius, 60, 30, true, 2){
 
    this->name = name;

    this->orbitRadius = orbitRadius;

    this->texturePath = texturePath;

    cout << "Texture: " << this->texturePath << endl;
    this->modelMatrix = mMatrix;

    this->rotationSpeed = rotationSpeed;
    this->orbitSpeed = orbitSpeed;

    this->printSelf();

    glGenVertexArrays(1, &(this->VAO));

    glBindVertexArray(VAO);

    glGenBuffers(1, &(this->VBO));
    glBindBuffer(GL_ARRAY_BUFFER, (this->VBO));
    glBufferData(GL_ARRAY_BUFFER, this->getInterleavedVertexSize(), this->getInterleavedVertices(), GL_STATIC_DRAW);

    glGenBuffers(1, &(this->EBO));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (this->EBO));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->getIndexSize(), this->getIndices(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    unsigned int stride = this->getInterleavedStride(); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)(sizeof(float) * 3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *)(sizeof(float) * 6));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
