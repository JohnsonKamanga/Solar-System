#include "../../include/Planet.h"
#include <iostream>
#include <fstream>

Planet::Planet(const char * name, float radius, float orbitRadius, string texturePath): Sphere(radius, 60, 30, true, 3){
 
    this->name = name;

    this->orbitRadius = orbitRadius;

    this->texturePath = texturePath;

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
