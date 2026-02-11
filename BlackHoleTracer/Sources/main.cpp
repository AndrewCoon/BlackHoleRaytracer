// Local Headers
#include "boiler.hpp"
#include "framebuffer.h"
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


static glm::dvec3 NewtonianAcceleration(glm::dvec3 loc, BlackHole bh) {
    return Constants::G * bh.getMass() * (bh.getPosition() - loc) / (double) glm::pow(glm::length(bh.getPosition() - loc), 3);
}

static double DistanceToSphere(glm::dvec3 p, glm::dvec3 c, double r) {
    return glm::length(p - c) - r;
}

static glm::dvec3 March_Euler(glm::dvec3 loc, glm::dvec3& vel, BlackHole bh) {
    vel += glm::normalize(Constants::dt * NewtonianAcceleration(loc, bh));
    return loc + Constants::dt * vel;
}

static glm::dvec3 March_RK4(glm::dvec3 loc, glm::dvec3& vel, BlackHole bh, double dt = Constants::dt) {
    glm::dvec3 k1v = NewtonianAcceleration(loc, bh);
    glm::dvec3 k1x = vel;

    glm::dvec3 k2v = NewtonianAcceleration(loc + dt / 2.0f * k1x, bh);
    glm::dvec3 k2x = vel + dt / 2.0f * k1v;

    glm::dvec3 k3v = NewtonianAcceleration(loc + dt / 2.0f * k2x, bh);
    glm::dvec3 k3x = vel + dt / 2.0f * k2v;

    glm::dvec3 k4v = NewtonianAcceleration(loc + dt * k3x, bh); 
    glm::dvec3 k4x = vel + dt * k3v;

    vel = vel + dt / 6 * (k1v + 2.0 * k2v + 2.0 * k3v + k4v);
    
    return loc + dt / 6 * (k1x + 2.0 * k2x + 2.0 * k3x + k4x);
}

// Camera looks towards -z
BlackHole bh(0.5, glm::dvec3(0.0, 0.0, -10.0));

void update(FrameBuffer& framebuffer, Camera& camera) {
    for (int j = 0; j < Config::WINDOW_HEIGHT; j+=4) {
        for (int i = 0; i < Config::WINDOW_WIDTH; i+=4) {
            // Center of pixel that this ray is looking at
            glm::vec3 pixel_center = camera.GetPixel00Location() + ((float)i * camera.GetPixelDeltaU()) + ((float)j * camera.GetPixelDeltaV());
            glm::vec3 ray_direction = pixel_center - camera.GetOrigin();
            
            Ray ray(camera.GetOrigin(), ray_direction);

            // Default Color
            // Orbit color????
            glm::vec3 color = glm::vec3(1.0f, 0.6f, 0.4f);

            glm::dvec3 loc = ray.GetOrigin();
            glm::dvec3 vel = (glm::dvec3) glm::normalize(ray.GetDirection()) * Constants::c;

            for (int k = 1; k < RayConfig::MAX_STEPS; k++) {
                double bh_dist = DistanceToSphere(loc, bh.getPosition(), bh.getRadius());

                if (bh_dist < 0.0) {
                    // bh color
                    color = glm::vec3(0.0f);
                    break;
                }

                if (bh_dist > (bh.getRadius() * 20.0)) {
                // Look at the final velocity (the direction the ray is pointing)
                    glm::dvec3 final_dir = glm::normalize(vel);
                    
                    // Create a simple checkerboard based on the direction
                    // Scaling the direction components to create the grid pattern
                    double pattern = std::sin(final_dir.x * 10.0) * std::sin(final_dir.y * 10.0) * std::sin(final_dir.z * 10.0);

                    if (pattern > 0.0) {
                        color = glm::vec3(1.0f, 1.0f, 1.0f); // White
                    } else {
                        color = glm::vec3(0.0f, 0.0f, 1.0f); // Blue
                    }
                    break;
                }
                
                loc = March_RK4(loc, vel, bh, Constants::dt * glm::max(bh_dist * 0.5, 1.0));
            }

            // Fix: Use 'j' instead of '0'
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

    double startTime = glfwGetTime();
        update(framebuffer, camera);
        render(display, framebuffer);

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    double endTime = glfwGetTime(); // Capture end time
    double sPerFrame = (endTime - startTime);
    printf("Frame time: %.2f s\n", sPerFrame);

    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        checkKeys(mWindow);

    }   
    
    glfwTerminate();
    return EXIT_SUCCESS;
}
