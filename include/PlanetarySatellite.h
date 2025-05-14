#pragma once
#include "CosmicObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class PlanetarySatellite: public CosmicObject{
    public:
    PlanetarySatellite(char const *name, float radius, float orbitRadius, float orbitSpeed, float rotationSpeed, string texturePath, glm::vec3 mMatrix);
    inline PlanetarySatellite(){};
    void draw(unsigned int shaderId, vec3 pos);
    void draw(unsigned int);
};