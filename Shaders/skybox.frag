#version 330 core

uniform samplerCube skybox;
uniform int isNightVision;

out vec4 FragColor;
in vec3 texCoord;

void main()
{
	vec4 value = texture(skybox, texCoord) * 0.5;

	if (isNightVision == 1)
	{
		vec3 proportions = vec3(0.30, 0.59, 0.11);

		float intensity = dot(value.rgb, proportions);
		intensity = clamp(0.87 * (intensity - 0.5) + 0.5, 0.0, 1.0);
		intensity = clamp(intensity / 0.8, 0.0, 1.0);

		value = vec4(intensity, intensity, intensity, value.a);  // greyscale is much more pleaser to the eyes
	}

	FragColor = value;
}