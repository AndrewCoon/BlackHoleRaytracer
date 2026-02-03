// Local Headers
#include "boiler.hpp"
#include "framebuffer.h"
#include "ray.h"
#include "display.h"
#include "camera.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Headers
#include <cstdio>
#include <cstdlib>

void update(FrameBuffer& framebuffer, Camera& camera) {
    // framebuffer.Clear(glm::vec3(0.0f));
    for (int j = 0; j < Config::WINDOW_HEIGHT; j++) {
        for (int i = 0; i < Config::WINDOW_WIDTH; i++) {
            glm::vec3 pixel_center = camera.GetPixel00Location() + ((float)i * camera.GetPixelDeltaU()) + ((float)j * camera.GetPixelDeltaV());
            glm::vec3 ray_direction = pixel_center - camera.GetOrigin();
            Ray ray(camera.GetOrigin(), ray_direction);

            glm::vec3 color = ray.RayColor(ray);
            framebuffer.SetPixel(i, j, color);
        }
    }
}

void render(Display& display, FrameBuffer& framebuffer) {
    display.UploadFramebuffer(framebuffer);
    display.Draw();
}

void checkKeys(GLFWwindow* mWindow) {
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);
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

    FrameBuffer framebuffer(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    Display display(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    Camera camera;

    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        checkKeys(mWindow);
        
        update(framebuffer, camera);
        render(display, framebuffer);

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   
    
    glfwTerminate();
    return EXIT_SUCCESS;
}
