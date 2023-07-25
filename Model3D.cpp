#include "pch.h"
#include "Model3D.h"


Model3D::Model3D(ModelReference* objRef, Texture* texture) : objRef(objRef), texture(texture)
{
	transform.position = glm::vec3(0, 0, 0);
	transform.Rotate(glm::vec3(0, 0, 0));
	transform.scale = glm::vec3(1, 1, 1);
	color = glm::vec3(1.f, 1.f, 1.f);
}

glm::mat4 Model3D::GetTransformationMatrix()
{
	glm::mat4 translation = glm::translate(identity_matrix4, transform.position);
	glm::mat4 rotation = glm::toMat4(transform.GetRotation());
	glm::mat4 scale = glm::scale(identity_matrix4, transform.scale);

	glm::mat4 transformation_matrix = scale * translation * rotation;

	return transformation_matrix;
}