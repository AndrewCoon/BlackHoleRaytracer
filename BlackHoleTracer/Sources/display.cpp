#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "display.h"
#include <iostream>
#include <vector>

Display::Display(int width, int height, const std::string& skyboxPath) 
    : m_Width(width), m_Height(height) {
    InitializeOpenGL();
    CreateShaders();
    CreateQuad();
    LoadSkyboxTexture(skyboxPath);
}

Display::~Display() {
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
    if (m_SkyboxTextureID) glDeleteTextures(1, &m_SkyboxTextureID);
}

void Display::InitializeOpenGL() {
    glGenTextures(1, &m_SkyboxTextureID);
    glBindTexture(GL_TEXTURE_2D, m_SkyboxTextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, nullptr);

}

void Display::LoadSkyboxTexture(const std::string& path) {
    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true); 
    
    float* data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
    
    if (data) {
        glGenTextures(1, &m_SkyboxTextureID);
        glBindTexture(GL_TEXTURE_2D, m_SkyboxTextureID);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, data);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        std::cout << "Loaded HDR Skybox: " << path << std::endl;
    } else {
        std::cerr << "Failed to load HDR image: " << path << std::endl;
        stbi_image_free(data);
    }
}

void Display::CreateQuad() {
    float quadVertices[] = {
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Display::CreateShaders() { 
    m_ShaderProgram = new Shader("blackhole.vert", "blackhole.frag");
}

void Display::Draw() {
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_ShaderProgram->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_SkyboxTextureID);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Display::UpdateUniforms(Camera& camera, BlackHole& bh, uint32_t& flags, float& bhSizeBuffer, float& diskThickness) {
    m_ShaderProgram->use();

    glm::vec3 camPos = camera.GetPosition();
    glm::mat4 view = camera.GetViewMatrix();
    m_ShaderProgram->setMat4("invView", glm::inverse(view));

    m_ShaderProgram->setVec3("camPos", camPos);
    m_ShaderProgram->setFloat("u_fov", glm::radians(camera.Zoom()));

    m_ShaderProgram->setVec3("bhPos", bh.Position());
    m_ShaderProgram->setFloat("bhMass", bh.Mass());
    m_ShaderProgram->setFloat("bhRadius", bh.Radius());

    float aspectRatio = (float)Config::WINDOW_WIDTH / (float)Config::WINDOW_HEIGHT;
    m_ShaderProgram->setFloat("u_aspectRatio", aspectRatio);

    m_ShaderProgram->setFloat("bhSizeBuffer", bhSizeBuffer);
    m_ShaderProgram->setFloat("diskThickness", diskThickness);
    
    m_ShaderProgram->setUInt("flags", flags);
    m_ShaderProgram->setInt("u_skybox", 0);
}

void Display::SaveFrame(const std::string& filename) {
    std::vector<unsigned char> pixels(m_Width * m_Height * 3);
    glReadPixels(0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    stbi_flip_vertically_on_write(true);
    if (stbi_write_png(filename.c_str(), m_Width, m_Height, 3, pixels.data(), m_Width * 3)) {
        std::cout << "Saved frame to: " << filename << std::endl;
    } else {
        std::cerr << "Failed to save frame: " << filename << std::endl;
    }
}
     