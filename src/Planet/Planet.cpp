#include "../../include/Planet.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <fstream>

Planet::Planet(const char * name, float radius, const char* texturePath): Sphere(radius, 36, 18, true, 2){
 
    this->name = name;

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


    // gen texture id
    glGenTextures(1, &(this->texture));

    // bind buffer to texture buffer
    glBindTexture(GL_TEXTURE_2D, this->texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

    if (data)
    {
        // generate texture using loaded image
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // generate mipmap
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture from " << texturePath << endl;
    }

    stbi_image_free(data);
}
