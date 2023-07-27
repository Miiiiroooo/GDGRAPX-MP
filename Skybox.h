#pragma once

class Skybox
{
public:
	Skybox(GLuint* shaderProgram);
	~Skybox();

	void IntializeSkybox(const List<String> skyboxFaces);
	void Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

private:
	void InitializeBufferObjects();
	void SetupTextures();


private:
	GLuint* skyboxShaderProgram;

	float* skyboxVerticestemp;
	unsigned int* skyboxIndicestemp;

	List<float> skyboxVertices;
	List<unsigned int> skyboxIndices;
	unsigned int skyboxVAO;
	unsigned int skyboxVBO;
	unsigned int skyboxEBO;

	List<String> skyboxTextureFaces;
	unsigned int skyboxTexture;
};

