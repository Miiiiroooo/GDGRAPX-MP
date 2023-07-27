#pragma once


class Shader
{
public:
	Shader();
	~Shader();

	void InitializeProgram(String vShaderPath, String fShaderPath);
	GLuint& GetShaderProgram();

private:
	String GetShaderData(String path);


private:
	const char* vShaderData;
	const char* fShaderData;

	GLuint vertShader;
	GLuint fragShader;
	GLuint shaderProgram;
};

