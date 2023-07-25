#include "pch.h"
#include "Transform.h"

Transform::Transform()
{
	position = glm::vec3(0, 0, 0);
	eulerAngles = glm::vec3(0, 0, 0);
	orientation = glm::quat(glm::vec3(0.f));
	scale = glm::vec3(1, 1, 1);

	localForward = glm::vec3(0.f, 0.f, 1.f);
	localUp = glm::vec3(0.f, 1.f, 0.f);
	localRight = glm::vec3(1.f, 0.f, 0.f);
}

Transform::~Transform()
{

}

void Transform::Rotate(const glm::vec3& rotation)
{
	orientation = glm::quat(glm::radians(rotation)) * orientation;

	eulerAngles += rotation;
	eulerAngles.x = GetClosestReferenceAngle(eulerAngles.x);
	eulerAngles.y = GetClosestReferenceAngle(eulerAngles.y);
	eulerAngles.z = GetClosestReferenceAngle(eulerAngles.z);

	UpdateLocalVectors(); 
}

void Transform::RotateAroundPoint(const glm::vec3& rotation, const glm::vec3& anchorPoint)
{
	// Update rotation
	Rotate(rotation); 

	// Update position
	glm::mat4 transformationMatrix = glm::translate(identity_matrix4, anchorPoint - position);    
	transformationMatrix = glm::toMat4(glm::quat(glm::radians(rotation))) * transformationMatrix; 
	transformationMatrix = glm::translate(transformationMatrix, position - anchorPoint);    

	position = transformationMatrix * glm::vec4(position, 1.f);}

glm::vec3 Transform::GetEulerRotation()
{
	return eulerAngles;
}

glm::quat Transform::GetRotation()
{
	return orientation;
}

float Transform::GetClosestReferenceAngle(float angle)
{
	int referenceAngle = (int)(angle * 100) % 36000;

	return (float)(referenceAngle / 100.f);
}

void Transform::UpdateLocalVectors()
{
	localForward = glm::toMat4(orientation) * glm::vec4(worldForward, 1.f);  
	localUp = glm::toMat4(orientation) * glm::vec4(worldUp, 1.f); 
	localRight = glm::toMat4(orientation) * glm::vec4(worldRight, 1.f); 

}
