#version 330 core

// lighting
struct DirLightData
{
	vec3 dir;
	vec3 color;
	float intensity;

	float ambientStr;
	vec3 ambientColor;

	float specStr;
	float specPhong;
};

struct PointLightData
{
	float radius;
	vec3 pos;
	vec3 color;
	float intensity;

	float ambientStr;
	vec3 ambientColor;

	float specStr;
	float specPhong;
};

uniform DirLightData dirLight;
uniform PointLightData pointLight;
uniform vec3 camPos;

// texture
uniform int isOnlyColor;
uniform vec3 newColor;
uniform sampler2D tex0;


// other variables
in vec3 normCoord;
in vec3 fragPos;
in vec2 texCoord;
out vec4 FragColor;


// method signatures
vec3 CalculateDirLight();
vec3 CalculatePointLight();


void main()
{
	vec3 overallLightValue = CalculateDirLight();
	overallLightValue += CalculatePointLight();

	if (isOnlyColor == 1)
	{
		FragColor = vec4(newColor.x, newColor.y, newColor.z, 1.0f);
	}
	else
	{
		FragColor = vec4(overallLightValue, 1.0) * texture(tex0, texCoord) * vec4(newColor.x, newColor.y, newColor.z, 1.0f);
	}
}

vec3 CalculateDirLight()
{
	// Get Normals
	vec3 normal = normalize(normCoord);

	// Compute diffuse
	float diff = max(dot(normal, -dirLight.dir), 0.0);
	vec3 diffuse = diff * dirLight.color;

	// Compute ambience
	vec3 ambient = dirLight.ambientColor * dirLight.ambientStr;

	// Compute specularity
	vec3 viewDir = normalize(camPos - fragPos);
	vec3 reflectDir = reflect(dirLight.dir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.1), dirLight.specPhong);
	vec3 specular = spec * dirLight.specStr * dirLight.color;
	
	
	return dirLight.intensity * (specular + ambient + diffuse);
}

vec3 CalculatePointLight()
{
	// Get LightDir
	vec3 normal = normalize(normCoord);
	vec3 lightDir = normalize(pointLight.pos - fragPos);

	// Compute diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * pointLight.color;

	// Compute ambience
	vec3 ambient = pointLight.ambientColor * pointLight.ambientStr;

	// Compute specularity
	vec3 viewDir = normalize(camPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.1), pointLight.specPhong);
	vec3 specular = spec * pointLight.specStr * pointLight.color;
	
	// Compute Point Light
	float distanceToLight = length(pointLight.pos - fragPos);
    float distanceOverRadius = distanceToLight / pointLight.radius;                    
    float pointLightStr = pointLight.intensity / (1 + pow(5 * distanceOverRadius, 2)); 
	
	return pointLightStr * (specular + ambient + diffuse);
}