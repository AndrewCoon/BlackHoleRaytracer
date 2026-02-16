// Local Headers
#include "boiler.hpp"
#include "display.h"
#include "camera.h"
#include "blackhole.h"
#include "stb_image.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// Headers
#include <cstdio>
#include <cstdlib>
#include <string>

// Assets/*imagename.hdr*
const std::string SKYBOX_PATH = "assets/eso0926a - eagle nebula.hdr";
GLuint skyboxTextureID;

float bhSizeBuffer = 1.08f;
bool useRelativity = true;
bool showDisk = true;

bool isDragging = false;
double lastX, lastY;

void InitializeScene() {
    // ... setup VAO/VBO for screen-filling quad ...

    // 2. Load the HDR image
    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true); 
    
    // Use loadf for float data
    float* data = stbi_loadf(SKYBOX_PATH.c_str(), &width, &height, &nrComponents, 0);
    
    if (data) {
        glGenTextures(1, &skyboxTextureID);
        glBindTexture(GL_TEXTURE_2D, skyboxTextureID);
        
        // 3. Upload to GPU
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, data);
        
        // 4. Set Texture Parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        std::cout << "Successfully loaded HDR" << std::endl;
    } else {
        std::cerr << "Failed to load HDR image!" << std::endl;
    }
}

void RenderImGui(ImGuiIO& io, Camera& camera, BlackHole& blackhole) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Simulation Controls");
    
    ImGui::Text("Black Hole Properties");
    ImGui::SliderFloat("Mass", &blackhole.Mass(), 0.1f, 10.0f);
    ImGui::Text("Schwarzschild Radius: %.3f", blackhole.Radius());
    ImGui::SliderFloat("Size Buffer", &bhSizeBuffer, 1.0f, 1.5f);

    ImGui::Separator();
    ImGui::Text("Simulation Parameters");
    ImGui::Checkbox("Use Relativistic Geodesics", &useRelativity);
    ImGui::Checkbox("Show Accretion Disk", &showDisk);
    
    ImGui::Separator();

    ImGui::Text("Camera Properties");
    ImGui::SliderFloat("Radius", &camera.Radius(), 1.0f, 50.0f);
    ImGui::SliderFloat("Azimuth", &camera.Azimuth(), -3.14159f, 3.14159f);
    ImGui::SliderFloat("Polar", &camera.Polar(), 0.0f, 3.14159f);
    ImGui::SliderFloat("Zoom", &camera.Zoom(), 0.0f, 360.0f);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
                1000.0f / io.Framerate, io.Framerate);
    
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void RenderScene(Display& display, Camera& camera, BlackHole& blackhole) {
    uint32_t flags = 0;

    if (useRelativity) flags |= (1 << 0);
    if (showDisk) flags |= (1 << 1);

    display.UpdateUniforms(camera, blackhole, flags, bhSizeBuffer);
    display.Draw();
}
void PrintTelemetry(double startTime, double endTime, Camera& camera, BlackHole& bh) {
    double msPerFrame = (endTime - startTime) / 1000.0;
    double fps = 1.0 / msPerFrame;

    printf("\rCamera: Radius=%.2f, Azimuth=%.2f, Polar=%.2f", 
           camera.Radius(), 
           camera.Azimuth(), 
           camera.Polar()
        );
    // printf("Black Hole State: Mass = %.2f, Radius = %.2f, Position = (%.2f, %.2f, %.2f)", 
    //     bh.GetMass(), 
    //     bh.Radius(),
    //     bh.GetPosition().x, 
    //     bh.GetPosition().y, 
    //     bh.GetPosition().z);        
    glm::vec3 camPos = camera.GetPosition();
    printf(" | Pos: (%.2f, %.2f, %.2f)", camPos.x, camPos.y, camPos.z);
    printf(" | Frame time: %.6f ms | FPS: %.1f  ", msPerFrame, fps);
    float distance = glm::distance(camera.GetPosition(), bh.Position());
    printf(" | Dist: %.2f", distance);

    fflush(stdout);
}
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

    if (ImGui::GetIO().WantCaptureMouse)
        return;

    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
            isDragging = true;
            glfwGetCursorPos(window, &lastX, &lastY);
        } else {
            isDragging = false;
        }
    }
}

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

    if (ImGui::GetIO().WantCaptureMouse)
        return;

    if (isDragging) {
        float dx = (float)(xpos - lastX);
        float dy = (float)(ypos - lastY);
        
        Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
        cam->SetAzimuth(cam->Azimuth() - dx * 0.01f);
        cam->SetPolar(cam->Polar() - dy * 0.01f);
        
        lastX = xpos;
        lastY = ypos;
    }
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);

    if (ImGui::GetIO().WantCaptureMouse)
        return;

    Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
    cam->SetRadius (cam->Radius() - (float)yoffset * 0.5f); // Adjust sensitivity
    if (cam->Radius() < 1.0f) cam->SetRadius(1.0f); // Don't fly through the BH
}

void CheckKeys(GLFWwindow* mWindow) {
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

    // ImGui Setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    ImGui_ImplOpenGL3_Init("#version 400");

    // Skybox setup
    InitializeScene();
    // Initialize scene objects and settings
    BlackHole blackhole(2.0f, glm::vec3(0.0f, 0.0f, 0.0f));
    Display display(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, SKYBOX_PATH);
    Camera camera(40.0f, 0.0f, 1.46f);

    // Set glfw pointers 
    glfwSetWindowUserPointer(mWindow, &camera);
    glfwSetScrollCallback(mWindow, ScrollCallback); 
    glfwSetCursorPosCallback(mWindow, CursorPosCallback);
    glfwSetMouseButtonCallback(mWindow, MouseButtonCallback);

    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        // double startTime = glfwGetTime();
        CheckKeys(mWindow);

        RenderScene(display, camera, blackhole);
        RenderImGui(io, camera, blackhole);


        // double endTime = glfwGetTime();

        // PrintTelemetry(startTime, endTime, camera, blackhole);

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   
    
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return EXIT_SUCCESS;
}
