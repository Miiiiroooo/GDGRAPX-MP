#pragma once


class Transform
{
public:
	Transform();
	~Transform();

	void Rotate(const glm::vec3& rotation);
	void RotateAroundPoint(const glm::vec3& rotation, const glm::vec3& anchorPoint);
	glm::vec3 GetEulerRotation();
	glm::quat GetRotation();

private:
	float GetClosestReferenceAngle(float angle);
	void UpdateLocalVectors();


public:
	glm::vec3 position;
	glm::vec3 scale;

	glm::vec3 localForward;
	glm::vec3 localUp;
	glm::vec3 localRight;

private:
	glm::vec3 eulerAngles;
	glm::quat orientation;
};

