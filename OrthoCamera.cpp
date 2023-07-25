#include "pch.h"
#include "OrthoCamera.h"

OrthoCamera::OrthoCamera()
{
	left = -2.0f;
	right = 2.0f;
	bottom = -2.0f;
	top = 2.0f;
	near = -1.0f;
	far = 1.0f;
}

OrthoCamera::OrthoCamera(float left, float right, float bottom, float top, float near, float far) : left(left), right(right), bottom(bottom), top(top), near(near), far(far)
{

}

OrthoCamera::~OrthoCamera()
{

}

void OrthoCamera::ComputeProjectionMatrix()
{
	projectionMatrix = glm::ortho(left, right, bottom, top, near, far); 
}
