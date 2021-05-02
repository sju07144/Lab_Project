#version 330 core
out vec4 fragColor;

in VS_OUT
{
	// vec3 fragColor;
	vec3 fragPos;
	vec3 fragPosViewCoordinates;
	vec2 texCoords;
	vec3 viewDir;
	vec3 lightDir; 
	// vec4 lightDir;
} fs_in;

// uniform vec3 lightPos;
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
	
	// point light version
	float distance = length(fs_in.lightDir);
	// directional light version
	// float distance = length(vec3(fs_in.lightDir.x, fs_in.lightDir.y, fs_in.lightDir.z));

	// checking background color
	vec3 tempnormal = normal * 255;
	if ((126 < tempnormal.r && tempnormal.r <= 127) &&
	   (126 < tempnormal.g && tempnormal.g <= 127) &&
	   (126 < tempnormal.b && tempnormal.b <= 127))
	{
		fragColor = vec4(1.0, 0.0, 1.0, 1.0);
	} 
	else
	{
		normal = normal * 2.0 - 1.0; 
		/* vec3 tempnormal = vec3(normal.x, normal.y, -normal.z);
		normal = vec3(tempnormal.z, tempnormal.y, tempnormal.x);
		normal = normalize(normal); */
		vec3 lightDir = normalize(fs_in.lightDir);
		// vec3 lightDir = normalize(-fs_in.lightDir.xyz);

		// float diff = max(dot(normal, lightDir), 0.0);
		float diff = clamp(dot(normal, lightDir), 0.0, 1.0);

		vec3 viewDir = normalize(fs_in.viewDir);
		vec3 reflectDir = reflect(-lightDir, normal);
		// float spec = pow(max(dot(viewDir, reflectDir), 0.0), 5.0);
		float spec = pow(clamp(dot(viewDir, reflectDir), 0.0, 1.0), 5.0);

		float attenuation = 1.0 / (distance * distance);

		vec3 ambient = modifiedAmbient;
		// point light version
		vec3 diffuse = modifiedDiffuse * diff * lightColor * lightPower;
		vec3 specular = modifiedSpecular * spec * lightColor * lightPower; 

		// directional light version
		/* vec3 diffuse = modifiedDiffuse * diff;
		vec3 specular = modifiedSpecular; */

		// ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;

		fragColor = vec4(ambient + diffuse + specular, 1.0);
		// fragColor = vec4(fs_in.fragColor, 1.0);
		// fragColor = texture(normalMap, fs_in.texCoords);
	} 
}


