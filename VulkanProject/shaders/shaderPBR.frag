# version 450

layout(binding = 1) uniform sampler2D diffuseSampler;
layout(binding = 2) uniform sampler2D normalSampler;
layout(binding = 3) uniform sampler2D specularSampler;
layout(binding = 4) uniform sampler2D glossSampler;

layout(push_constant) uniform Constants {
	vec3 lightDirection;
	float lightIntensity;
	vec3 cameraPos;
	float shininess;
	bool useNormal;
} constant;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragTangent;
layout(location = 4) in vec3 fragBitangent;
layout(location = 5) in vec3 fragWorldPosition;

layout(location = 0) out vec4 outColor;

const float PI = 3.14159265358979323846;


vec3 LambertDiffuse(float kd, vec3 cd)
{
	return (kd*cd) / PI;
}

vec3 GetReflectionSpecular(vec4 glossSampled, vec4 specularSampled, vec3 finalNormal, vec3 viewDirection)
{
    vec3 glossColor = glossSampled.rgb * constant.shininess;
    vec3 reflectLight = reflect(-constant.lightDirection, finalNormal);
    float cosTheta = max(dot(reflectLight, viewDirection), 0.0);
    vec3 reflectionSpecular = specularSampled.rgb * pow(cosTheta, glossColor.x);
    return max(reflectionSpecular, vec3(0.0));
}

void main()
{
	vec4 diffuseSample = texture(diffuseSampler, fragTexCoord);
    vec4 normalSample = texture(normalSampler, fragTexCoord);
    vec4 glossSample = texture(glossSampler, fragTexCoord);
    vec4 specularSample = texture(specularSampler, fragTexCoord);

    vec3 normal = fragNormal;
    vec3 invViewDirection = normalize(constant.cameraPos - fragWorldPosition.xyz);

    if (constant.useNormal)
    {
        vec3 colorNormal = 2.0 * normalSample.rgb - 1.0;
        vec3 binormal = cross(fragNormal, fragTangent);
        mat3 tangentSpaceAxis = mat3(normalize(fragTangent), normalize(binormal), normalize(fragNormal));
        colorNormal = normalize(tangentSpaceAxis * colorNormal);
        normal = colorNormal;
    }

    float observedArea = max(dot(-constant.lightDirection, normal), 0.0);

    if (observedArea > 0.0) {
        vec3 reflexionSpecular = GetReflectionSpecular(glossSample, specularSample, normal, invViewDirection);
        vec3 specularColor = LambertDiffuse(constant.lightIntensity, diffuseSample.rgb) + reflexionSpecular;
        outColor = vec4(specularColor * observedArea, 1.0);
    } else {
        outColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}