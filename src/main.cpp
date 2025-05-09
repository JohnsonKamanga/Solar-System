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
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;
using namespace glm;

float Yaw = -90.0f;
float Pitch = 0.0f;
bool firstMouse = true;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float lastX = 400, lastY = 300;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    Shader shader("./Shader/vertexShader.vsh", "./Shader/fragmentShader.fsh");

    Planet planets[] = {
        Planet("sun", 3.5f, 0.0f, "./PlanetTextureMaps/sunmap.jpg"),
        Planet("mercury", 1.5f, 15.0f, "./PlanetTextureMaps/mercurymap.jpg"),
        Planet("venus", 1.5f, 10.0f, "./PlanetTextureMaps/venusmap.jpg"),
        Planet("earth", 1.8f, 30.0f, "./PlanetTextureMaps/earthmap1k.jpg"),
        Planet("moon", 0.8f, 33.8f, "./PlanetTextureMaps/moonmap1k.jpg"),
        Planet("mars", 1.6f, 40.0f, "./PlanetTextureMaps/marsmap1k.jpg"),
    };

    vec3 planetPositions[] = {
        vec3(0.0f, 0.0f, 0.0f),  // sun position
        vec3(2.0f, 0.0f, 2.0f),  // mercury position
        vec3(6.0f, 0.0f, 6.0f),  // venus position
        vec3(10.0f, 0.0f, 10.0f), // earth position
        vec3(11.8f, 0.0f, 11.8f), // moon position
        vec3(14.0f, 0.0f, 14.0f)  // mars position
    };

    unsigned int textures[6];

    glGenTextures(6, textures);

    for (int i = 0; i < size(textures); i++)
    {

        // bind buffer to texture buffer
        glBindTexture(GL_TEXTURE_2D, textures[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        int width, height, nrChannels;

        stbi_set_flip_vertically_on_load(true);

        unsigned char *data = stbi_load(planets[i].getTexturePath().c_str(), &width, &height, &nrChannels, 0);

        if (data)
        {
            GLenum channelColors;
            if(nrChannels == 4){
                channelColors = GL_RGBA;
            }
            else if(nrChannels == 3){
                channelColors = GL_RGB;
            }
            else if(nrChannels == 2){
                channelColors = GL_RG;
            }
            else if(nrChannels == 1){
                channelColors = GL_RED;
            }
            // generate texture using loaded image
            glTexImage2D(GL_TEXTURE_2D, 0, channelColors, width, height, 0, channelColors, GL_UNSIGNED_BYTE, data);
            // generate mipmap
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            cout << "Failed to load texture from " << planets[i].getTexturePath() << endl;
        }

        stbi_image_free(data);

    }

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        shader.use();

        mat4 view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        mat4 projection = perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));

        unsigned int projectionLoc = glGetUniformLocation(shader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projection));

        float time = glfwGetTime();
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        for (unsigned int i = 0; i < size(planets); i++)
        {
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            glBindVertexArray(planets[i].getVAO());

            float posX = sin(time) * planets[i].getOrbitRadius();
            float posZ = cos(time) * planets[i].getOrbitRadius();
            // cout << "Planet: " << planets[i].getName() << endl;
            // cout << "x: " << posX << ", z: " << posZ << endl;
            mat4 model = mat4(1.0f);
            model = translate(model, vec3(posX, 0.0f, posZ)  );
            // float angle = 20.0f * i;
            // model = rotate(model, radians(20.0f), vec3(1.0f, 1.0f, 0.5f));

            unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
            glDrawElements(GL_TRIANGLES, planets[i].getIndexCount(), GL_UNSIGNED_INT, (void *)0);
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
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

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
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    Yaw += xoffset;
    Pitch += yoffset;
    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;
    glm::vec3 direction;
    direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    direction.y = sin(glm::radians(Pitch));
    direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    cameraFront = glm::normalize(direction);
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