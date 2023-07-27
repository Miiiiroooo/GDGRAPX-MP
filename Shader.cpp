#include "pch.h"
#include "Shader.h"

Shader::Shader()
{

}

Shader::~Shader()
{

}

void Shader::InitializeProgram(String vShaderPath, String fShaderPath)
{
    String vString = GetShaderData(vShaderPath);  
    vShaderData = vString.c_str();    
    String fString = GetShaderData(fShaderPath);
    fShaderData = fString.c_str();

    vertShader = glCreateShader(GL_VERTEX_SHADER);  
    glShaderSource(vertShader, 1, &vShaderData, NULL); 
    glCompileShader(vertShader); 

    fragShader = glCreateShader(GL_FRAGMENT_SHADER);  
    glShaderSource(fragShader, 1, &fShaderData, NULL); 
    glCompileShader(fragShader); 

    shaderProgram = glCreateProgram(); 
    glAttachShader(shaderProgram, vertShader); 
    glAttachShader(shaderProgram, fragShader); 
    glLinkProgram(shaderProgram); 
}

GLuint& Shader::GetShaderProgram()
{
    return shaderProgram;
}

String Shader::GetShaderData(String path) 
{
    std::fstream src(path); 
    std::stringstream buffer; 
    buffer << src.rdbuf(); 

    return buffer.str();
}
