#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform vec4 colDiffuse;

out vec4 finalColor;

const float radius = 7.5 / 32.0;

void main() {
	float d = length(fragTexCoord - vec2(0.5));
	float w = fwidth(d);

	float a = 1.0 - smoothstep(radius - w, radius + w, d);
	if (a <= 0.0) discard;
	
	vec4 color = colDiffuse * fragColor;
	finalColor = vec4(color.rgb, color.a * a);
}