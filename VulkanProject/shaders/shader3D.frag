# version 450

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform sampler2D normalSampler;
layout(binding = 3) uniform sampler2D roughnessSampler;
layout(binding = 4) uniform sampler2D metallicSampler;

layout(push_constant) uniform Constants {
	vec3 lightDirection;
	float lightIntensity;
	vec3 cameraPos;
	float shininess;
	bool useNormal;
} constant;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main()
{
	outColor = texture(texSampler,fragTexCoord);
}