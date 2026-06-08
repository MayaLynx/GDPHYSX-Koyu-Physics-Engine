#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model3D/model3D.h"
#include "Physics/PhysicsParticle.h"
#include "Cameras/Camera.h"
#include "Cameras/PerspectiveCamera.h"
#include "Cameras/OrthoCamera.h"
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <random>

using namespace std::chrono_literals;

float rotMod = 0.f;
bool isSimulationActive = true;
int currCam = 1; // Ortho cam by default

static void Key_Callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods)
{
    if (key == GLFW_KEY_A &&
        action == GLFW_PRESS)
    {
        rotMod += 0.01f;
    }
    else if (key == GLFW_KEY_A &&
        action == GLFW_RELEASE)
    {
        rotMod = 0.f;
    }
    if (key == GLFW_KEY_D &&
        action == GLFW_PRESS)
    {
        rotMod -= 0.01f;
    }
    else if (key == GLFW_KEY_D &&
        action == GLFW_RELEASE)
    {
        rotMod = 0.f;
    }
    if (key == GLFW_KEY_SPACE &&
        action == GLFW_PRESS)
    {
        isSimulationActive = !isSimulationActive; // Pauses / plays the simulation when pressed
    }

    // Changes active camera
    if (key == GLFW_KEY_1 &&
        action == GLFW_PRESS)
    {
        currCam = 1;
    }
    if (key == GLFW_KEY_2 &&
        action == GLFW_PRESS)
    {
        currCam = 2;
    }
}

int main(void)
{
    // Time in between frames
    constexpr std::chrono::nanoseconds timestep(16ms);

    GLFWwindow* window;

    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count()); // Random number generator for values
    std::uniform_int_distribution<> randomScale(2, 10); // Limits random scale values to 2-10

    std::vector<Koyu::PhysicsParticle*> particles; // Vector to store the particles
    std::vector<Camera*> cameras; // Stores the ortho and perspective cameras
    int maxParticleCount = 0;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // Window size variables
    float windowWidth = 800;
    float windowHeight = 800;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowWidth, windowHeight, "Koyu Engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glfwSetKeyCallback(window, Key_Callback);

    glViewport(
        0,
        0,
        windowWidth,
        windowHeight
    );

    // Shader initialization
    std::fstream vertSrc("Shaders/shader.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/shader.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragShader);
    glLinkProgram(shaderProg);
    
    // Create cameras
    OrthoCamera* orthoCam = new OrthoCamera(0.f, 0.f, 50.f, windowWidth/2, windowHeight/2, -400, 400);

    PerspectiveCamera* perspectiveCam = new PerspectiveCamera(0.f, -350.f, 80.f, 100.f, windowWidth, windowHeight);
    
    Camera* activeCam = orthoCam;

    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Asks user the max number of particles on screen
        if (maxParticleCount <= 0)
        {
            std::cout << "Enter number of sparks to simulate: ";
            std::cin >> maxParticleCount;
        }

        curr_time = clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - prev_time);
        prev_time = curr_time;       

        curr_ns += dur;
        if (curr_ns >= timestep && isSimulationActive)
        {
            constexpr float timestep_sec = timestep.count() / (float)(1E09); // Convert ns -> secs

            curr_ns -= timestep;

            //std::cout << "Physics update" << std::endl;

            // Create particles each physics update until max particle count is reached.
            // New ones are created after particles are destroyed.
            if (particles.size() < maxParticleCount)
            {
                model3D* newSphere = new model3D("3D/sphere.obj", glm::vec3(0.f, 0.f, 0.f), shaderProg);
                float newScale = randomScale(rng);
                newSphere->setScale(newScale, newScale, newScale);
                Koyu::PhysicsParticle* newParticle = new Koyu::PhysicsParticle(newSphere);
                particles.push_back(newParticle);
            }
            
            int count = 0; // Used to get index of particle to be destoyed.
            for (auto p : particles)
            {
                p->update(timestep_sec);

                // Destroy particle when its lifespan reaches 0.
                if (p->getLifespan() <= 0)
                {
                    particles.erase(particles.begin() + count);
                }
                count++;
            }
        }
        //std::cout << "Normal update" << std::endl;
        
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        for (Camera* cam : cameras)
        {
            cam->updateRotation(rotMod);
        }

        // Switch camera from keyboard input
        if (currCam == 1)
        {
            activeCam = orthoCam;
        }
        else if (currCam == 2)
        {
            activeCam = perspectiveCam;
        }

        // Projection matrix
        glm::mat4 projection = activeCam->getProjection();

        // Position of camera in the world
        glm::vec3 cameraPos = activeCam->getCameraPos();

        // View matrix
        glm::mat4 viewMatrix = activeCam->getViewMatrix();

        // Projection matrix assignment
        unsigned int projLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projLoc,
            1,
            GL_FALSE,
            glm::value_ptr(projection));

        // View matrix assignment
        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUniformMatrix4fv(viewLoc,
            1,
           GL_FALSE,
            glm::value_ptr(viewMatrix));

        // Draw objects
        for (auto p : particles)
        {
            p->draw();
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}