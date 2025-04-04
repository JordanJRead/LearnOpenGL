#version 460 core
out vec4 FragColor;

uniform sampler2D screenTex;
uniform float offset;

in vec2 fragTexCoords;

void main() {
	vec2 offsets[9] = vec2[](
	vec2(-offset, offset), // top-left
	vec2( 0.0f, offset), // top-center
	vec2( offset, offset), // top-right
	vec2(-offset, 0.0f), // center-left
	vec2( 0.0f, 0.0f), // center-center
	vec2( offset, 0.0f), // center-right
	vec2(-offset, -offset), // bottom-left
	vec2( 0.0f, -offset), // bottom-center
	vec2( offset, -offset) // bottom-right
	);

	vec3 resultColor = vec3(0);

	float kernel[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 2.0 / 16
	);

	for (int i = 0; i < 9; ++i) {
		resultColor += vec3(texture(screenTex, fragTexCoords + offsets[i]) * kernel[i]);
	}

	//FragColor = vec4(texture(screenTex, fragTexCoords));
	FragColor = vec4(resultColor, 1);
	//FragColor = vec4(fragTexCoords, 0, 1);
}