#include "display.h"
#include <iostream>

Display::Display(int width, int height) 
    : m_Width(width),
    m_Height(height) 
{
    InitializeOpenGL();
    CreateQuad();
    CreateShaders();
}

Display::~Display() {
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
    if (m_TextureID) glDeleteTextures(1, &m_TextureID);
}

void Display::InitializeOpenGL() {
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, nullptr);

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
    glClear(GL_COLOR_BUFFER_BIT);
    m_ShaderProgram->use();
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Display::UpdateUniforms(Camera& camera, BlackHole& bh, uint32_t& flags) {
    m_ShaderProgram->use();

    glm::vec3 camPos = camera.GetPosition();
    glm::mat4 view = glm::lookAt(camPos, bh.Position(), glm::vec3(0, 1, 0));
    m_ShaderProgram->setMat4("invView", glm::inverse(view));
    m_ShaderProgram->setVec3("camPos", camPos);

    m_ShaderProgram->setVec3("bhPos", bh.Position());
    m_ShaderProgram->setFloat("bhMass", bh.Mass());
    m_ShaderProgram->setFloat("bhRadius", bh.Radius());

    float aspectRatio = (float)Config::WINDOW_WIDTH / (float)Config::WINDOW_HEIGHT;
    m_ShaderProgram->setFloat("u_aspectRatio", aspectRatio);

    m_ShaderProgram->setUInt("flags", flags);
} 