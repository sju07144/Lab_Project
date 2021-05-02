#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out VS_OUT
{
	// vec3 fragColor;
	vec3 fragPos;
	vec3 fragPosViewCoordinates;
	vec2 texCoords;
	vec3 viewDir;
	vec3 lightDir; 
	// vec4 lightDir;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightDir;

void main()
{
	mat4 viewModelMatrix = view * model;
	vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.fragPosViewCoordinates = vec3(viewModelMatrix * vec4(aPos, 1.0));
	vs_out.texCoords = aTexCoords;

	vec3 viewPosViewCoordinates = vec3(view * vec4(viewPos, 1.0));
	vs_out.viewDir = viewPosViewCoordinates - vs_out.fragPosViewCoordinates;

	vec3 lightPosViewCoordinates = vec3(view * vec4(lightPos, 1.0));
	vs_out.lightDir = lightPosViewCoordinates - vs_out.fragPosViewCoordinates; 
	// vs_out.lightDir = view * vec4(lightDir, 1);

	gl_Position = projection * viewModelMatrix * vec4(aPos, 1.0);
}