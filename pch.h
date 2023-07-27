#pragma once

// imported libraries
#define _USE_MATH_DEFINES
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "tiny_obj_loader.h"
#include "stb_image.h"

// standard libraries
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <time.h>
#include <unordered_map>

// templates
template <typename T> using List = std::vector<T>;
template <typename T, typename U> using Pair = std::pair<T, U>;
template <typename T, typename U> using Dictionary = std::unordered_map<T, U>;

// typedefs
typedef std::string String;

// global constants
const glm::vec3 worldForward = glm::vec3(0, 0, 1.f);
const glm::vec3 worldUp = glm::vec3(0, 1.f, 0);
const glm::vec3 worldRight = glm::vec3(1.f, 0, 0);
const glm::mat3 identity_matrix3 = glm::mat3(1.0f);
const glm::mat4 identity_matrix4 = glm::mat4(1.0f);

// global methods
template<typename MainClass, typename ToCheck>
inline bool InstanceOf(const MainClass* ptr) { 
	return dynamic_cast<const ToCheck*>(ptr) != nullptr;
}

template<typename MainClass, typename ToConvert>
inline ToConvert* ConvertTo(MainClass* ptr) {
	return dynamic_cast<ToConvert*>(ptr);
}

// global enums
enum Cameras {
	Third_Person,
	First_Person,
	Birds_Eye,
};

enum LightIntensity {
	Low,
	Medium,
	High,
};