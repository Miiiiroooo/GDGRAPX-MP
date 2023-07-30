#include "pch.h"
#include "Skybox.h"

Skybox::Skybox(GLuint* shaderProgram) : skyboxShaderProgram(shaderProgram)
{
    skyboxVertices = {
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    };

    skyboxIndices = {
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };

}

Skybox::~Skybox()
{

}

void Skybox::IntializeSkybox(const List<String> skyboxFaces)
{
    InitializeBufferObjects();

    skyboxTextureFaces = skyboxFaces;
    SetupTextures();
}

void Skybox::Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, bool isNightVision)
{
    // Use skybox shader and remove translations from view matrix
    glDepthMask(GL_FALSE); 
    glDepthFunc(GL_LEQUAL); 
    glUseProgram(*skyboxShaderProgram); 
    glm::mat4 sky_view = glm::mat4(glm::mat3(viewMatrix));

    // Pass in matrices to the shader
    unsigned int skyViewLoc = glGetUniformLocation(*skyboxShaderProgram, "view");
    glUniformMatrix4fv(skyViewLoc, 1, GL_FALSE, glm::value_ptr(sky_view));

    unsigned int skyProjectionLoc = glGetUniformLocation(*skyboxShaderProgram, "projection");
    glUniformMatrix4fv(skyProjectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    GLuint isNightVisionAddress = glGetUniformLocation(*skyboxShaderProgram, "isNightVision");
    glUniform1i(isNightVisionAddress, (int)(isNightVision));

    // Render skybox
    glBindVertexArray(skyboxVAO);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture); 
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

void Skybox::InitializeBufferObjects()
{
    // Generate buffers
    glGenVertexArrays(1, &skyboxVAO); 
    glGenBuffers(1, &skyboxVBO); 
    glGenBuffers(1, &skyboxEBO); 

    // Bind buffers and their data
    glBindVertexArray(skyboxVAO); 
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * skyboxVertices.size(), skyboxVertices.data(), GL_STATIC_DRAW);

    // Reference attirb pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0); 

    // Unbind buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO); 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * skyboxIndices.size(), skyboxIndices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); 
}

void Skybox::SetupTextures()
{
    // Generate skybox textures
    glGenTextures(1, &skyboxTexture); 
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture); 

    // Extra parameters to modify how textures are rendered
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 

    // Loading textures
    for (unsigned int i = 0; i < 6; i++)
    {
        int w, h, sky_color_channel; 
        stbi_set_flip_vertically_on_load(false); 

        unsigned char* data = stbi_load(skyboxTextureFaces[i].c_str(), &w, &h, &sky_color_channel, 0); 

        if (data) 
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data); 
        }

        stbi_image_free(data); 
    }

    stbi_set_flip_vertically_on_load(true); 
}
