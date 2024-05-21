# version 450

layout(binding = 0) uniform UniformBufferObject {
	mat4 model;
	mat4 view;
	mat4 proj;
} ubo;



layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;
layout(location = 4) in vec3 inTangent;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec3 fragTangent;
layout(location = 4) out vec3 fragBitangent;
layout(location = 5) out vec3 fragWorldPosition;

void main() {
	gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
	fragColor = inColor;
	fragTexCoord = inTexCoord;
	vec4 modelNormal = ubo.model * vec4(inNormal, 0.0);
	fragNormal = normalize(modelNormal.xyz);

	vec4 modelTangent = ubo.model * vec4(inTangent, 0.0);
	fragTangent = normalize(modelTangent.xyz);
	
	fragBitangent = cross(fragNormal, fragTangent);
	fragWorldPosition = vec3(ubo.model * vec4(inPosition, 1.0));
}	