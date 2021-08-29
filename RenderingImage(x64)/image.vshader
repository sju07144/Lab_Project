#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out VS_OUT
{
	vec3 fragPos;
	vec3 fragPosViewCoordinates;
	vec2 texCoords;
	vec3 viewDir;
	vec4 lightDir;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec4 lightDir;
uniform mat4 viewForLightDir;

void main()
{
	mat4 viewModelMatrix = view * model;
	vs_out.fragPos = vec3(model * vec4(aPos, 1.0)); // World Coordinates ÁÂÇ¥
	vs_out.fragPosViewCoordinates = vec3(viewModelMatrix * vec4(aPos, 1.0)); // View Coordinates ÁÂÇ¥
	vs_out.texCoords = aTexCoords;

	vec3 viewPosViewCoordinates = vec3(view * vec4(viewPos, 1.0));
	vs_out.viewDir = viewPosViewCoordinates - vs_out.fragPosViewCoordinates;

	vs_out.lightDir = viewForLightDir * lightDir; // light direction¸¸ view matrix¸¦ ´Ù¸£°Ô °öÇÔ

	gl_Position = projection * viewModelMatrix * vec4(aPos, 1.0);
}