#pragma once
#include "Sphere.h"
#include "Shader.h"
#include "PlanetarySatellite.h"
#include "CosmicObject.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class Planet: public CosmicObject
{

public:
    inline Planet()
    {
        cout << "Default planet constructor" << endl;
    };

    Planet(char const *name, float radius, float orbitRadius, float orbitSpeed, float rotationSpeed, string texturePath, glm::vec3 mMatrix, PlanetarySatellite *ym);
    
    
    inline PlanetarySatellite* getMoon() const
    {
        return this->moon;
    };

    inline void setMoons(PlanetarySatellite &m){
        this->moon = &m;
    }

     void draw(unsigned int shaderId);

private:
    char const *name;
    unsigned int VAO, VBO, EBO;
    string texturePath;
    unsigned int textureId;
    float orbitRadius, orbitSpeed, rotationSpeed;
    glm::vec3 modelMatrix;
    glm::vec3 position;
    PlanetarySatellite *moon;//planet's moons
};