#include "pch.h"
#include "Texture.h"

Texture::Texture(const char* imagePath) : imagePath (imagePath)
{

}

Texture::~Texture()
{

}

void Texture::LoadTexture(GLint imageFormat)
{
    stbi_set_flip_vertically_on_load(true);
    unsigned char* tex_bytes = stbi_load(imagePath, &width, &height, &colorChannels, 0);

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, tex_bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);
    glEnable(GL_DEPTH_TEST);
}

GLuint& Texture::GetTexture()
{
	return texture;
}
