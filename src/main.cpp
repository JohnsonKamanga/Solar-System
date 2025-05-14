#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "../include/Sphere.h"
#include "../include/Shader.h"
#include "../include/Planet.h"
#include "../include/PlanetarySatellite.h"
#include "../include/CosmicObject.h"
#include "../include/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;
using namespace glm;

// ########global variables#############

bool firstMouse = true;

float sunRotationSpeed = 1.0f;
float moonOrbitSpeed = 1.0f;

float Zoom;
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float lastX = 400, lastY = 300;

vec3 sunPos(0.0f, 0.0f, 0.0f);
Camera camera(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
// ########function declarations#############

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// ##########main function##############
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Johnson's Solar System", NULL, NULL);

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    Shader shader("./Shader/vertexShader.vsh", "./Shader/fragmentShader.fsh");

    PlanetarySatellite* moon = new PlanetarySatellite("moon", 0.6f, 3.8f, 1.0f, 4.5f, "./PlanetTextureMaps/moonmap1k.jpg", vec3(3.8f, 0.0f, 3.8f));

    Planet *planets[] = {
         new Planet("sun", 3.5f, 0.0f, 1.0f, 1.0f, "./PlanetTextureMaps/sunmap.jpg", sunPos, nullptr),
         new Planet("mercury", 1.2f, 10.0f, 1.1f, 2.5f, "./PlanetTextureMaps/mercurymap.jpg", vec3(15.0f, 0.0f, 15.0f),  nullptr),
         new Planet("venus", 1.5f, 25.0f, 1.2f, 3.5f, "./PlanetTextureMaps/venusmap.jpg", vec3(10.0f, 0.0f, 10.0f), nullptr),
         new Planet("earth", 1.8f, 40.0f, 1.3f, 4.5f, "./PlanetTextureMaps/earthmap1k.jpg", vec3(30.0f, 0.0f, 30.0f), moon),
         new Planet("mars", 1.6f, 50.0f, 1.5f, 5.5f, "./PlanetTextureMaps/marsmap1k.jpg", vec3(40.0f, 0.0f, 40.0f), nullptr)
    };

    //used to initialize textures
    CosmicObject* bodies[] ={
        planets[0],//sun
        planets[1],//mercury
        planets[2],//venus
        planets[3],//earth
        moon,//moon
        planets[4]//mars
    } ;

    unsigned int textures[6];

    glGenTextures(6, textures);

    for (int i = 0; i < size(textures); i++)
    {

        // bind buffer to texture buffer
        glBindTexture(GL_TEXTURE_2D, textures[i]);

        (*bodies[i]).setTextureId(textures[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        int width, height, nrChannels;

        // stbi_set_flip_vertically_on_load(true);

        unsigned char *data = stbi_load((*bodies[i]).getTexturePath().c_str(), &width, &height, &nrChannels, 0);

        if (data)
        {
            GLenum channelColors;
            if (nrChannels == 4)
            {
                channelColors = GL_RGBA;
            }
            else if (nrChannels == 3)
            {
                channelColors = GL_RGB;
            }
            else if (nrChannels == 2)
            {
                channelColors = GL_RG;
            }
            else if (nrChannels == 1)
            {
                channelColors = GL_RED;
            }
            // generate texture using loaded image
            glTexImage2D(GL_TEXTURE_2D, 0, channelColors, width, height, 0, channelColors, GL_UNSIGNED_BYTE, data);
            // generate mipmap
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            cout << "Failed to load texture from " << (*bodies[i]).getTexturePath() << endl;
        }

        stbi_image_free(data);
    }

    glEnable(GL_DEPTH_TEST);

    bool first = true;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        shader.use();

        mat4 view = camera.GetViewMatrix();

        mat4 projection = perspective(radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

        unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));

        unsigned int projectionLoc = glGetUniformLocation(shader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projection));

        //point light
        unsigned int pointLightPosLoc = glGetUniformLocation(shader.ID, "pointLightPosition");
        glUniform3f(pointLightPosLoc, sunPos.x, sunPos.y + 1.0f, sunPos.z);

        unsigned int pointLightColorLoc = glGetUniformLocation(shader.ID, "pointLightColor");
        glUniform3f(pointLightColorLoc, 0.0f, 0.0f, 1.0f);

        unsigned int pointLightAmbientStrengthLoc = glGetUniformLocation(shader.ID, "pointLightAmbientStrength");
        glUniform1f(pointLightAmbientStrengthLoc, 0.15f);

        //directional light
        unsigned int directionalLightPosLoc = glGetUniformLocation(shader.ID, "directionalLightPosition");
        glUniform3f(directionalLightPosLoc, 0.0f, 0.0f , 1.0f);

        unsigned int directionalLightColorLoc = glGetUniformLocation(shader.ID, "directionalLightColor");
        glUniform3f(directionalLightColorLoc, 1.0f, 1.0f, 1.0f);

        unsigned int directionalLightAmbientStrengthLoc = glGetUniformLocation(shader.ID, "directionalLightAmbientStrength");
        glUniform1f(directionalLightAmbientStrengthLoc, 0.15f);


        for (unsigned int i = 0; i < size(planets); i++)
        {

            unsigned int viewPosLoc = glGetUniformLocation(shader.ID, "viewPos");
            glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

            if ((*planets[i]).getName() == "sun")
            {
                (*planets[i]).setRotationSpeed(sunRotationSpeed);
            }

                if ((*planets[i]).getName() == "earth")
                {
                    //get first and only moon
                    (*planets[i]).getMoon()->setOrbitSpeed(moonOrbitSpeed);
                }
        
            (*planets[i]).draw(shader.ID);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// #######function implementations###########

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        sunRotationSpeed += camera.MovementSpeed * deltaTime * 0.2f;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        sunRotationSpeed -= camera.MovementSpeed * deltaTime * 0.2f;
        sunRotationSpeed = sunRotationSpeed < 0.0f ? 0.0f : sunRotationSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        moonOrbitSpeed += camera.MovementSpeed * deltaTime * 0.2f;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        moonOrbitSpeed -= camera.MovementSpeed * deltaTime * 0.2f;
        moonOrbitSpeed = moonOrbitSpeed < 0.0f ? 0.0f : moonOrbitSpeed;
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset, false);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
