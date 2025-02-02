#version 460 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;

out vec3 fragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
	vec3 worldNorm = normalize(mat3(transpose(inverse(model))) * vNorm); // transposing is expensive

	vec4 worldPos4 = model * vec4(vPos, 1);
	vec3 worldPos = worldPos4.xyz;
	gl_Position = projection * view * worldPos4;

	float ambientFactor = 0.3;
	vec3 ambient = lightColor * ambientFactor;

	vec3 lightDir = normalize(lightPos - worldPos);
	float diffuseFactor = max(dot(lightDir, worldNorm), 0);
	vec3 diffuse = lightColor * diffuseFactor;

	float specStrength = 0.5;
	vec3 reflectDir = normalize(reflect(-lightDir, worldNorm));

	vec3 viewDir = normalize(viewPos - worldPos);
	float specFactor = pow(max(dot(reflectDir, viewDir), 0), 32.0);
	vec3 spec = lightColor * specFactor;

	fragColor = (ambient + diffuse + spec) * objectColor;
}