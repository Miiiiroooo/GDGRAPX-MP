#include "pch.h"
#include "Light.h"


Light::Light()
{
	lightColor = glm::vec3(1.f);
	lightIntensity = 1.f;

	ambientStr = 0.2f;
	ambientColor = lightColor;
	specStr = 0.5f;
	specPhong = 16;
}

Light::~Light()
{

}
