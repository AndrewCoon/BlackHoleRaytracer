// Local Headers
#include "boiler.hpp"
#include "display.h"
#include "camera.h"
#include "blackhole.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Headers
#include <cstdio>
#include <cstdlib>

bool paused = false;

bool isDragging = false;
double lastX, lastY;

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
            isDragging = true;
            // Record the starting position so the first movement doesn't "jump"
            glfwGetCursorPos(window, &lastX, &lastY);
        } else {
            isDragging = false;
        }
    }
}

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (isDragging) {
        float dx = (float)(xpos - lastX);
        float dy = (float)(ypos - lastY);
        
        Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
        cam->SetAzimuth(cam->GetAzimuth() - dx * 0.01f);
        cam->SetPolar(cam->GetPolar() - dy * 0.01f);
        
        lastX = xpos;
        lastY = ypos;
    }
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
    cam->SetRadius (cam->GetRadius() - (float)yoffset * 0.5f); // Adjust sensitivity
    if (cam->GetRadius() < 1.0f) cam->SetRadius(1.0f); // Don't fly through the BH
}

void checkKeys(GLFWwindow* mWindow) {
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

    
    
    static bool spaceWasPressed = false;
    bool spaceIsPressed = glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS;
    if (spaceIsPressed && !spaceWasPressed) {
        paused = !paused;
        printf("\nState: %s\n", paused ? "ON" : "OFF");
    }
    spaceWasPressed = spaceIsPressed;
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

    // Initialize scene objects and settings
    BlackHole blackhole(2.0f, glm::dvec3(0.0f, 0.0f, 0.0f));
    Display display(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    Camera camera(20.0f, 0.0f, Constants::PI / 2.0f);

    // Set glfw pointers 
    glfwSetWindowUserPointer(mWindow, &camera);
    glfwSetScrollCallback(mWindow, ScrollCallback); 
    glfwSetCursorPosCallback(mWindow, CursorPosCallback);
    glfwSetMouseButtonCallback(mWindow, MouseButtonCallback);

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
        printf("\rCamera: Radius=%.2f, Azimuth=%.2f, Polar=%.2f", 
               camera.GetRadius(), 
               camera.GetAzimuth(), 
               camera.GetPolar()
            );        
        glm::vec3 camPos = camera.GetPosition();
        printf(" | Pos: (%.2f, %.2f, %.2f)", camPos.x, camPos.y, camPos.z);
        printf(" | Frame time: %.6f ms | FPS: %.1f  ", msPerFrame, fps);
        float distance = glm::distance(camera.GetPosition(), blackhole.GetPosition());
        printf(" | Dist: %.2f", distance);

        fflush(stdout);

        glfwPollEvents();
    }   
    
    glfwTerminate();
    return EXIT_SUCCESS;
}
