#pragma once

#include "Camera.h"

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera();
	PerspectiveCamera(float fov, float width, float height, float near, float far);
	~PerspectiveCamera();

	void ComputeProjectionMatrix();


public:
	float fov;
	float width;
	float height;
	float near;
	float far;
};

