#version 330 core
out vec4 fragColor;

in VS_OUT
{
	vec3 fragPos;
	vec3 fragPosViewCoordinates;
	vec2 texCoords;
	vec3 viewDir;
	vec4 lightDir;
} fs_in;

// uniform vec3 lightPos; // point light 부분(현재는 사용하지 않음)
uniform sampler2D normalMap; // normal map 텍스쳐(첫 번째 코드에서 생성한 텍스쳐)
uniform sampler2D albedoMap; // albedo map 텍스쳐(두 번째 코드에서 생성한 텍스쳐)

void main()
{
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	float lightPower = 70.0f;

	vec3 ambientColor = vec3(0.0, 0.0, 0.0);
	vec4 diffuseColor = texture(albedoMap, fs_in.texCoords);
	vec3 specularColor = vec3(0.4, 0.4, 0.4);

	vec3 materialDiffuseColor = vec3(diffuseColor);
	vec3 materialAmbientColor = ambientColor * materialDiffuseColor;
	vec3 materialSpecularColor = specularColor;

	vec3 modifiedAmbient = clamp(ambientColor, 0.65, 1.0) * materialDiffuseColor; // if ambient color value is 0, remultiply
	vec3 modifiedDiffuse = materialDiffuseColor * vec3(0.65);
	vec3 modifiedSpecular = materialSpecularColor * vec3(0.1);

	vec3 normal = texture(normalMap, fs_in.texCoords).rgb;
	
	// directional light version
	float distance = length(vec3(fs_in.lightDir.x, fs_in.lightDir.y, fs_in.lightDir.z));

	// checking background color(normal version)
	/* vec3 tempnormal = normal * 255;
	if ((126 < tempnormal.r && tempnormal.r <= 128) &&
	   (126 < tempnormal.g && tempnormal.g <= 128) &&
	   (126 < tempnormal.b && tempnormal.b <= 128))
	{
		fragColor = vec4(1.0, 0.0, 1.0, 1.0);
	} */

	// 배경부분은 light 계산을 하면 안되므로 이를 제거
	// checking background color(albedo version)
	vec4 tempColor = diffuseColor * 255;
	if ((253 <= tempColor.r && tempColor.r <= 255) && 
	   (tempColor.g == 0) && 
	   (253 <= tempColor.b && tempColor.b <= 255))
	{
		// fragColor = vec4(1.0, 0.0, 1.0, 1.0);
		fragColor = diffuseColor;
	}
	else
	{ 
		normal = normal * 2.0 - 1.0;
		normal = normalize(normal); 
		vec3 lightDir = normalize(-fs_in.lightDir.xyz);

		float diff = clamp(dot(normal, lightDir), 0.0, 1.0);

		vec3 viewDir = normalize(fs_in.viewDir);
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(clamp(dot(viewDir, reflectDir), 0.0, 1.0), 5.0);

		float attenuation = 1.0 / (distance * distance);

		vec3 ambient = modifiedAmbient;

		// directional light version
		vec3 diffuse = modifiedDiffuse * diff;
		vec3 specular = modifiedSpecular; 


		fragColor = vec4(ambient + diffuse + specular, 1.0);
		// 디버깅 용도
		// fragColor = vec4(fs_in.fragColor, 1.0);
		// fragColor = texture(normalMap, fs_in.texCoords);
	} 
}


