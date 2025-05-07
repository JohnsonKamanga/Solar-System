#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "../include/Sphere.h"
#include "../include/Shader.h"
#include "../include/Planet.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);

    if (window == NULL)
    {
        cout << "Failed to create glfw window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize glad" << endl;
        return -1;
    }

    glViewport(0, 0, 800, 800);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader shader("./Shader/vertexShader.vsh", "./Shader/fragmentShader.fsh");
    
    Planet planets []=  {
        Planet("sun", 3.0f, "./PlanetTextureMaps/sunmap.jpg"),
        Planet("mercury", 0.3f, "./PlanetTextureMaps/mercurymap.jpg"),
        Planet("venus", 0.4f, "./PlanetTextureMaps/venusmap.jpg"),
        Planet("earth", 0.5f, "./PlanetTextureMaps/earthmap1k.jpg"),
        Planet("moon", 0.15f, "./PlanetTextureMaps/moonmap1k.jpg"),
        Planet("mars", 0.6f, "./PlanetTextureMaps/marsmap1k.jpg"),
    };

    vec3 planetPositons[] = {
        vec3(0.0f, 0.0f, 0.0f),//sun position
        vec3(0.0f, 0.0f, 2.0f),//mercury position
        vec3(0.0f, 0.0f, 6.0f),//venus position
        vec3(0.0f, 0.0f, 10.0f),//earth position
        vec3(0.0f, 0.0f, 11.8f),//moon position
        vec3(0.0f, 0.0f, 14.0f)//mars position
    };

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        mat4 view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        mat4 projection = perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));

        unsigned int projectionLoc = glGetUniformLocation(shader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projection));

        for (unsigned int i = 0; i < size(planets); i++)
        {
            glBindTexture(GL_TEXTURE_2D, planets[i].getTexture());
            glBindVertexArray(planets[i].getVAO());
            const float radius = 10.0f;
            float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
            
            mat4 model = mat4(1.0f);
            model = planets[i].getName() == "sun" ? translate(model, planetPositons[i]) : translate(model, planetPositons[i] + vec3(camX, 0.0, camZ) );
            // float angle = 20.0f * i;
            model = rotate(model, (float)glfwGetTime() * radians(20.0f), vec3(1.0f, 1.0f, 0.5f));

            unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
            glDrawElements(GL_TRIANGLES, planets[i].getIndexCount(), GL_UNSIGNED_INT, (void*)0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    const float cameraSpeed = 0.05f; // adjust accordingly
if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
cameraPos += cameraSpeed * cameraFront;
if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
cameraPos -= cameraSpeed * cameraFront;
if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
cameraSpeed;
if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *
cameraSpeed;
}

/*
unsigned int VAO;
    Planet sun("sun", 0.5f, &VAO);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        float color = sin(glfwGetTime()) / 2.0f + 0.5f;
        unsigned int ColorLocation = glGetUniformLocation(shader.ID, "SphereColor");
        glUniform4f(ColorLocation, 0.5, color, 0.2, 1.0);
        

        unsigned int transformationMatrixLocation = glGetUniformLocation(shader.ID, "TransformationMatrix");
        
        mat4 trans = mat4(1.0f);
        trans = scale(trans, vec3(0.5, 0.5, 0.5));

       glUniformMatrix4fv(transformationMatrixLocation, 1, GL_FALSE, value_ptr(trans));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sun.getIndexCount(), GL_UNSIGNED_INT, (void*)0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

*/