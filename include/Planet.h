#pragma once
#include "Sphere.h"
#include "Shader.h"
#include <iostream>

using namespace std;

class Planet: public Sphere
{

public:
    inline Planet()
    {
        cout << "Default planet constructor" << endl;
    };

    Planet(char const *name, float radius, float orbitRadius, string texturePath);
    
    inline char const *getName() const
    {
        return this->name;
    };

    inline string getTexturePath() const
    {
        return this->texturePath;
    };

    inline unsigned int getVAO() const
    {
        return this->VAO;
    };

    inline unsigned int getEBO() const
    {
        return this->EBO;
    };

    inline unsigned int getVBO() const
    {
        return this->VBO;
    };

    inline float getOrbitRadius() const
    {
        return this->orbitRadius;
    };

private:
    char const *name;
    unsigned int VAO, VBO, EBO;
    string texturePath;
    float orbitRadius;
};