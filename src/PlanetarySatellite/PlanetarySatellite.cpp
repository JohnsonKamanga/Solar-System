#include "../../include/PlanetarySatellite.h"
#include "../../include/Planet.h"
#include "../../include/CosmicObject.h"
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;
using namespace glm;

PlanetarySatellite::PlanetarySatellite(char const *name, float radius, float orbitRadius, float orbitSpeed, float rotationSpeed, string texturePath, glm::vec3 mMatrix): CosmicObject(name, radius, orbitRadius, orbitSpeed, rotationSpeed, texturePath, mMatrix){

}

void PlanetarySatellite::draw(unsigned int shaderId, vec3 pos){
    glBindVertexArray(this->getVAO());
    glBindTexture(GL_TEXTURE_2D, this->getTextureId());
    
    float time = glfwGetTime();
    float posX = cos(time * this->getOrbitSpeed()) * this->getModelMatrix().x;
    float posZ = sin(time * this->getOrbitSpeed()) * this->getModelMatrix().z;        
    
    mat4 model = mat4(1.0f);
    model = translate(model, vec3(posX + pos.x , 0.0f, posZ + pos.z));
    
    model = rotate(model, (time)*this->getRotationSpeed(), vec3(0.0f, 1.0f, 0.0f));

    unsigned int modelLoc = glGetUniformLocation(shaderId, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
    glDrawElements(GL_TRIANGLES, this->getIndexCount(), GL_UNSIGNED_INT, (void *)0);
}

void PlanetarySatellite::draw(unsigned int shaderId){
    cout << "drawing ...." << endl;
}