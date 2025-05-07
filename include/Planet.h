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

    Planet(char const *name, float radius, const char* texturePath);
    
    inline char const *getName() const
    {
        return this->name;
    };

    inline unsigned int getTexture() const
    {
        return this->texture;
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

private:
    char const *name;
    unsigned int VAO, VBO, EBO, texture;
};