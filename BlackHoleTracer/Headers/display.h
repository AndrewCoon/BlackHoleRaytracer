#ifndef DISPLAY_H
#define DISPLAY_H

#include "boiler.hpp"
#include "shader.h"

class Display {
public:
    Display(int width, int height);
    ~Display();
    
    // Main interface
    void Draw();
    void UpdateUniforms(const glm::vec3& camPos, const glm::vec3& bhPos);
    
    // Utility
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    Shader* GetShader() const { return m_ShaderProgram; }
    
private:
    void InitializeOpenGL();
    void CreateShaders();
    void CreateQuad();
    
    // OpenGL resources
    GLuint m_TextureID;
    GLuint m_VAO, m_VBO;
    Shader* m_ShaderProgram;
    
    int m_Width, m_Height;
};

#endif