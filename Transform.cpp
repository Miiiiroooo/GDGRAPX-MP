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
	eulerAngles = RoundToClosestReferenceAngle(eulerAngles);

	UpdateLocalVectors(); 
}

void Transform::RotateAroundPoint(const glm::vec3& rotation, const glm::vec3& anchorPoint)
{
	// Update rotation
	Rotate(rotation); 

	// Update position
	glm::vec3 displacement = position - anchorPoint;
	glm::mat4 transformationMatrix = glm::toMat4(glm::quat(glm::radians(rotation)));
	displacement = transformationMatrix * glm::vec4(displacement, 1.f);

	position = anchorPoint + displacement; 
}

void Transform::RotateAroundAxis(float angle, const glm::vec3& axis)
{
	orientation = glm::angleAxis(glm::radians(angle), axis) * orientation;

	eulerAngles = glm::degrees(glm::eulerAngles(orientation));
	eulerAngles = RoundToClosestReferenceAngle(eulerAngles);

	UpdateLocalVectors();
}

void Transform::RotateAroundAxisAndPoint(float angle, const glm::vec3& axis, const glm::vec3& anchorPoint)
{
	// Update rotation
	RotateAroundAxis(angle, axis);

	// Update position
	glm::vec3 displacement = position - anchorPoint; 
	glm::mat4 transformationMatrix = glm::toMat4(glm::angleAxis(glm::radians(angle), axis));
	displacement = transformationMatrix * glm::vec4(displacement, 1.f); 

	position = anchorPoint + displacement; 
}

void Transform::ResetRotation()
{
	eulerAngles = glm::vec3(0, 0, 0);
	orientation = glm::quat(glm::vec3(0.f));
	UpdateLocalVectors(); 
}

glm::vec3 Transform::GetEulerRotation()
{
	return eulerAngles;
}

glm::quat Transform::GetRotation()
{
	return orientation;
}

glm::vec3 Transform::RoundToClosestReferenceAngle(glm::vec3 rot)
{
	glm::vec3 newRot = rot;
	newRot.x = ((int)(newRot.x * 100000) % 36000000) / 100000.f;
	newRot.y = ((int)(newRot.y * 100000) % 36000000) / 100000.f;
	newRot.z = ((int)(newRot.z * 100000) % 36000000) / 100000.f;

	return newRot;
}

void Transform::UpdateLocalVectors()
{
	localForward = glm::toMat4(orientation) * glm::vec4(worldForward, 1.f);  
	localUp = glm::toMat4(orientation) * glm::vec4(worldUp, 1.f); 
	localRight = glm::toMat4(orientation) * glm::vec4(worldRight, 1.f); 

}
