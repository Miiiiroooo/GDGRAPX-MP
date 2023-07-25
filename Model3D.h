#pragma once

#include "Transform.h"
#include "ModelReference.h"
#include "Texture.h"

class Model3D
{
public:
	Model3D(ModelReference* objRef, Texture* texture);
	glm::mat4 GetTransformationMatrix();


public:
	Transform transform;
	glm::vec3 color;

	ModelReference* objRef;
	Texture* texture;
};

