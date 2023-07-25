#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
	camTarget = glm::vec3(0.f, 0.f, 0.f);
}

Camera::~Camera()
{
}

void Camera::SetCamTarget(glm::vec3 camTarget)
{
	this->camTarget = camTarget;
}

glm::vec3 Camera::GetCamTarget()
{
	return camTarget;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}
