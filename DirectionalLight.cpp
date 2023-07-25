#include "pch.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	lightDir = glm::vec3(1.f, 0.f, 0.f);
}

DirectionalLight::~DirectionalLight()
{

}
