varying vec3 N;
varying vec3 L;
varying vec3 E;
varying vec2 fTextureCoord;
varying mat4 inverseTBN;
varying float height;

uniform vec4 materialAmbient, materialDiffuse, materialSpecular;
uniform float materialShininess;
uniform mat4 lightSource;
uniform mat4 model;
uniform sampler2D tex1;
uniform sampler2D bumpTex1;
uniform sampler2D tex2;
uniform sampler2D bumpTex2;
uniform sampler2D tex3;
uniform sampler2D bumpTex3;
uniform sampler2D tex4;
uniform sampler2D bumpTex4;
uniform bool textureBlend;
uniform vec4 texHeights;

void main()
{
	vec4 texWeights;
	texWeights.x = clamp(1.0 - abs(height - texHeights.x) / 0.2, 0.0, 1.0);
	texWeights.y = clamp(1.0 - abs(height - texHeights.y) / 0.25, 0.0, 1.0);
	texWeights.z = clamp(1.0 - abs(height - texHeights.z) / 0.25, 0.0, 1.0);
	texWeights.w = clamp(1.0 - abs(height - texHeights.w) / 0.25, 0.0, 1.0);
	texWeights = texWeights / (texWeights.x + texWeights.y + texWeights.z + texWeights.w);

	vec4 bump1 = texture2D(bumpTex1, fTextureCoord);
	vec4 bump2 = texture2D(bumpTex2, fTextureCoord);
	vec4 bump3 = texture2D(bumpTex3, fTextureCoord);
	vec4 bump4 = texture2D(bumpTex4, fTextureCoord);
	vec4 bump = bump1 * texWeights.x + bump2 * texWeights.y + bump3 * texWeights.z + bump4 * texWeights.w;

	bump = normalize(2.0*bump-1.0);
	bump = model * inverseTBN * bump;
	//vec3 NN = normalize(normalize(bump.xyz) + normalize(N));
	vec3 NN = normalize(bump.xyz);

	vec3 EE = normalize(E);

	//vec4 texColor = texture2D(tex, fTextureCoord);

	vec4 texColor1 = texture2D(tex1, fTextureCoord);
	vec4 texColor2 = texture2D(tex2, fTextureCoord);
	vec4 texColor3 = texture2D(tex3, fTextureCoord);
	vec4 texColor4 = texture2D(tex4, fTextureCoord);
	vec4 texColor = texColor1 * texWeights.x + texColor2 * texWeights.y + texColor3 * texWeights.z + texColor4 * texWeights.w;

	vec4 ambientProduct, diffuseProduct, specularProduct;
	float shininess;
	if (textureBlend)
	{
		ambientProduct = mix(materialAmbient, texColor, 0.5) * lightSource[0];
		diffuseProduct = mix(materialDiffuse, texColor, 0.5) * lightSource[1];
		specularProduct = mix(materialSpecular, texColor, 0.5) * lightSource[2];
		shininess = materialShininess;
	}
	else
	{
		ambientProduct = 0.25 * texColor * lightSource[0];
		diffuseProduct = 0.7 * texColor * lightSource[1];
		specularProduct = 0.5 * texColor * lightSource[2];
		shininess = 32.0;
	}

	float distance;
	//if (lightSource[3].w == 0.0)
		distance = 1.0;
	//else
	//	distance = pow(length(L), 2.0);

	vec3 LL = normalize(L);
	float LdotN = dot(LL, NN);

	vec4 ambient, diffuse, specular;

	// ambient
	ambient = ambientProduct / distance;

	// diffuse
	float Kd = max(LdotN, 0.0);
	//float Kd = abs(LdotN);
	diffuse = Kd * diffuseProduct / distance;

	// specular
	vec3 H = normalize(LL+EE);
	float Ks = pow(max(dot(NN, H), 0.0), shininess) / distance;
	if (LdotN < 0.0)
		specular = vec4(0.0, 0.0, 0.0, 1.0);
	else
		specular = Ks*specularProduct;

	gl_FragColor = vec4((ambient + diffuse + specular).xyz, 1.0);
}
