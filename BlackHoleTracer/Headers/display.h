#ifndef DISPLAY_H
#define DISPLAY_H

#include "boiler.hpp"
#include "framebuffer.h"
#include "shader.h"

class Display {
public:
    Display(int width, int height);
    ~Display();
    
    // Main interface
    void UploadFramebuffer(const FrameBuffer& fb);
    void Draw();
    
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