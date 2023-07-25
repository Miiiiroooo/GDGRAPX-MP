#pragma once


class ModelReference
{
public:
	ModelReference(String modelPath);
	~ModelReference();

    bool LoadObject();
    List<GLfloat>& GetFullVertexData();
    GLuint GetVAO();
    void DeleteBufferObjects();

private:
    void SetupBufferObjects();


private:
    String modelPath;

    tinyobj::attrib_t attributes; 
    List<tinyobj::shape_t> shapes;  
    List<tinyobj::material_t> materials; 
    List<GLuint> mesh_indices; 
    List<GLfloat> fullVertexData;

    GLuint VAO;
    GLuint VBO;
};

