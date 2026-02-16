#ifndef DISPLAY_H
#define DISPLAY_H

#include "boiler.hpp"
#include "shader.h"
#include "camera.h"
#include "blackhole.h"
#include <string>

class Display {
public:
    Display(int width, int height, const std::string& skyboxPath);
    ~Display();
    
    // Main interface
    void Draw();
    void UpdateUniforms(Camera& camera, BlackHole& bh, uint32_t& flags, float& bhSizeBuffer);
    
    // Utility
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    Shader* GetShader() const { return m_ShaderProgram; }
    
private:
    void InitializeOpenGL();
    void CreateShaders();
    void CreateQuad();
    
    void LoadSkyboxTexture(const std::string& path);
    
    // OpenGL resources
    GLuint m_SkyboxTextureID;
    GLuint m_VAO, m_VBO;
    Shader* m_ShaderProgram;
    
    int m_Width, m_Height;
};

#endif