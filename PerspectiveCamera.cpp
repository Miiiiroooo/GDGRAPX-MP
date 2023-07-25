#include "pch.h"
#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera()
{
	fov = 90.f;
	width = 600.f;
	height = 600.f;
	near = 0.1f;
	far = 100.f;
}

PerspectiveCamera::PerspectiveCamera(float fov, float width, float height, float near, float far) : fov(fov), width(width), height(height), near(near), far(far)
{

}

PerspectiveCamera::~PerspectiveCamera()
{

}

void PerspectiveCamera::ComputeProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(fov), height / width, near, far);
}
