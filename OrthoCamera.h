#pragma once

#include "Camera.h"

class OrthoCamera : public Camera
{
public:
	OrthoCamera();
	OrthoCamera(float left, float right, float bottom, float top, float near, float far);
	~OrthoCamera();

	void ComputeProjectionMatrix();


public:
	float left;
	float right;
	float bottom;
	float top;
	float near;
	float far;
};

