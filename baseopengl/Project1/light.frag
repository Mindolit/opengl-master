#version 330 core
out vec4 FragColor;

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
}fs_in;

uniform sampler2D floorTexture;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform vec3 viewPos;
uniform bool gamma;
uniform bool Blinn;
vec3 BlinnPhng(vec3 normal,vec3 fragPos,vec3 lightPos,vec3 lightColor)
{
	//difuse 설정
	vec3 lightDir=normalize(lightPos-fragPos);//광원 방향 빛 위치- 프래그 위치
	float diff=max(dot(lightDir,normal),0.0);
	vec3 diffuse=diff*lightColor;

	//specular
	vec3 viewDir=normalize(viewPos-fragPos); // 보는 방향
	vec3 reflectDir=reflect(-lightDir,normal);

	float spec=0.0;
	vec3 specular;
	if(Blinn){
		vec3 halfwayDir=normalize(lightDir+viewDir);
		spec=pow(max(dot(normal,halfwayDir),0.0),64.0);
	    specular=spec+lightColor;
	}
	else
	{
		spec=pow(max(dot(reflectDir,viewDir),0.0),64.0);
		specular=spec+lightColor;
	}
	//attenuation
	float max_distance=1.5;
	float distance=length(lightPos-fragPos);
	float attenuation=1.0/(gamma?distance*distance:distance);

	diffuse*=attenuation;
	specular*=attenuation;
	return diffuse+specular;
}


void main()
{
	vec3 color=texture(floorTexture,fs_in.TexCoords).rgb;
	vec3 lighting=vec3(0.0);
	for(int i=0; i<4; i++)
	{
		lighting+=BlinnPhng(normalize(fs_in.Normal),fs_in.FragPos,lightPositions[i],lightColors[i]);
	}
	color*=lighting;

	if(gamma)
	{
		color=pow(color,vec3(1.0/2.2));
	}

	FragColor=vec4(color,1.0);

}