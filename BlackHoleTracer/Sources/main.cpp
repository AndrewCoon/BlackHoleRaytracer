// Local Headers
#include "boiler.hpp"
#include "ray.h"
#include "display.h"
#include "camera.h"
#include "blackhole.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Headers
#include <cstdio>
#include <cstdlib>

void checkKeys(GLFWwindow* mWindow) {
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);
    
    // static bool keyWasPressed = false;
    // bool keyIsPressed = glfwGetKey(mWindow, GLFW_KEY_????) == GLFW_PRESS;

    // if (keyIsPressed && !keyWasPressed) {
    //     // Do stuff
    //     printf("State: %s\n", variable ? "ON" : "OFF");
    // }
    // keyWasPressed = keyIsPressed;
}

int main() {
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    BlackHole blackhole(2, glm::dvec3(0.0, 0.0, -30.0));

    Display display(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

    printf("Black Hole State: Mass = %.2f, Radius = %.2f, Position = (%.2f, %.2f, %.2f)\n", 
           blackhole.GetMass(), 
           blackhole.GetRadius(),
           blackhole.GetPosition().x, 
           blackhole.GetPosition().y, 
           blackhole.GetPosition().z);

    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        checkKeys(mWindow);

        double startTime = glfwGetTime();
            display.UpdateUniforms(camera, blackhole);
            display.Draw();

            glfwSwapBuffers(mWindow);
        double endTime = glfwGetTime(); // Capture end time
        double msPerFrame = (endTime - startTime) / 1000.0;
        double fps = 1.0 / msPerFrame;
        printf("\rFrame time: %.6f ms | FPS: %.1f", msPerFrame, fps);
        fflush(stdout);

        glfwPollEvents();
    }   
    
    glfwTerminate();
    return EXIT_SUCCESS;
}
