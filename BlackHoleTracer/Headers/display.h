#ifndef DISPLAY_H
#define DISPLAY_H

#include "boiler.hpp"
#include "framebuffer.h"

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
    
private:
    void InitializeOpenGL();
    void CreateShaders();
    void CreateQuad();
    
    // OpenGL resources
    GLuint m_TextureID;
    GLuint m_VAO, m_VBO;
    GLuint m_ShaderProgram;
    
    int m_Width, m_Height;
};

#endif