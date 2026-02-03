// Local Headers
#include "boiler.hpp"
#include "framebuffer.h"
#include "display.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Headers
#include <cstdio>
#include <cstdlib>

void update(FrameBuffer& framebuffer) {
    framebuffer.Clear(glm::vec3(0.0f));
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

    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        checkKeys(mWindow);
        update(framebuffer);
        render(display, framebuffer);

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();
    return EXIT_SUCCESS;
}
