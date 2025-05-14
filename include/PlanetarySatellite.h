#pragma once
#include "Planet.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class PlanetarySatellite: public Planet{
    public:
    PlanetarySatellite(char const *name, float radius, float orbitRadius, float orbitSpeed, float rotationSpeed, float planetOrbitRadius,string texturePath, glm::vec3 mMatrix);
    inline PlanetarySatellite(){};
    void draw(unsigned int shaderId, unsigned int textureId);

    private:
    float planetOrbitradius;
};