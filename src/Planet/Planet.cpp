#include "../../include/Planet.h"
#include "../../include/PlanetarySatellite.h"
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

Planet::Planet(const char * name, float radius, float orbitRadius, float orbitSpeed, float rotationSpeed, string texturePath, vec3 mMatrix, PlanetarySatellite * m): CosmicObject(name, radius, orbitRadius, orbitSpeed, rotationSpeed, texturePath, mMatrix){

    this->moon = m;

}

void Planet::draw(unsigned int shaderId){
            glBindVertexArray(this->getVAO());
            glBindTexture(GL_TEXTURE_2D, this->getTextureId());
            
            float time = glfwGetTime();
            float posX = sin(time * this->getOrbitSpeed()) * this->getModelMatrix().x;
            float posZ = cos(time * this->getOrbitSpeed()) * this->getModelMatrix().z;
            vec3 pos(posX, 0.0f, posZ);
            mat4 model = mat4(1.0f);
            model = translate(model, pos);
            
            model = rotate(model, (time)*this->getRotationSpeed(), vec3(0.0f, 1.0f, 0.0f));
            mat3 normalMatrix(transpose(inverse(model)));

            unsigned int modelLoc = glGetUniformLocation(shaderId, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

            unsigned int normalMatrixLoc = glGetUniformLocation(shaderId, "normalMatrix");
            glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, value_ptr(normalMatrix));

            glDrawElements(GL_TRIANGLES, this->getIndexCount(), GL_UNSIGNED_INT, (void *)0);

            glBindVertexArray(0);

            if(moon != nullptr){
                (*moon).draw(shaderId, pos);
            }

        }
