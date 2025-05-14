#pragma once
#include "Sphere.h"
#include "Shader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class CosmicObject: public Sphere
{

public:
    inline CosmicObject()
    {
        cout << "Default CosmicObject constructor" << endl;
    };

    CosmicObject(char const *name, float radius, float orbitRadius, float orbitSpeed, float rotationSpeed, string texturePath, glm::vec3 mMatrix);
    
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

    inline float getOrbitSpeed() const
    {
        return this->orbitSpeed;
    };

    inline float getRotationSpeed() const
    {
        return this->rotationSpeed;
    };

    inline glm::vec3 getModelMatrix() const
    {
        return this->modelMatrix;
    };

    inline glm::vec3 getPosition() const
    {
        return this->position;
    };

    inline unsigned int getTextureId() const
    {
        return this->textureId;
    };

    inline void setOrbitRadius(float radius){
        this->orbitRadius = radius;
    }

    inline void setOrbitSpeed(float speed){
        this->orbitSpeed = speed;
    }

    inline void setRotationSpeed(float speed){
        this->rotationSpeed = speed;
    }

    inline void setModelMatrix(vec3 model){
        this->modelMatrix = model;
    }

    inline void setPosition(vec3 pos){
        this->position = pos;
    }

    inline void setTextureId(unsigned int id){
        this->textureId = id;
    }

     virtual void draw(unsigned int shaderId)=0;

private:
    char const *name;
    unsigned int VAO, VBO, EBO;
    string texturePath;
    unsigned int textureId;
    float orbitRadius, orbitSpeed, rotationSpeed;
    glm::vec3 modelMatrix;
    glm::vec3 position;
};