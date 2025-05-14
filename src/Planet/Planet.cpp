#include "../../include/Planet.h"
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

Planet::Planet(const char * name, float radius, float orbitRadius, float orbitSpeed, float rotationSpeed, string texturePath, vec3 mMatrix): Sphere(radius, 60, 30, true, 2){
 
    this->name = name;

    this->orbitRadius = orbitRadius;

    this->texturePath = texturePath;
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

void Planet::draw(unsigned int shaderId, unsigned int textureId) const{
            glBindVertexArray(this->VAO);
            glBindTexture(GL_TEXTURE_2D, textureId);
            
            float time = glfwGetTime();
            float posX = sin(time * orbitSpeed) * this->modelMatrix.x;
            float posZ = cos(time * orbitSpeed) * this->modelMatrix.z;
            
            mat4 model = mat4(1.0f);
            model = translate(model, vec3(posX, 0.0f, posZ));
            
            model = rotate(model, (time)*rotationSpeed, vec3(0.0f, 1.0f, 0.0f));
            mat3 normalMatrix(transpose(inverse(model)));

            unsigned int modelLoc = glGetUniformLocation(shaderId, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

            unsigned int normalMatrixLoc = glGetUniformLocation(shaderId, "normalMatrix");
            glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, value_ptr(normalMatrix));

            glDrawElements(GL_TRIANGLES, this->getIndexCount(), GL_UNSIGNED_INT, (void *)0);
}
