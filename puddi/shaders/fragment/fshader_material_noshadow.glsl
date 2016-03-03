varying vec3 N;
varying vec3 L;
varying vec3 E;

uniform vec4 materialAmbient, materialDiffuse, materialSpecular;
uniform float materialShininess;
uniform mat4 lightSource;

void main()
{
	vec3 NN = normalize(N);

	vec3 EE = normalize(E);

	vec4 ambientProduct = materialAmbient * lightSource[0];
	vec4 diffuseProduct = materialDiffuse * lightSource[1];
	vec4 specularProduct = materialSpecular * lightSource[2];

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
	float Ks = pow(max(dot(NN, H), 0.0), materialShininess) / distance;
	if (LdotN < 0.0)
		specular = vec4(0.0, 0.0, 0.0, 1.0);
	else
		specular = Ks*specularProduct;

	//gl_FragColor = vec4((ambient + diffuse + specular).xyz, 1.0);

//	if (shadowMode == 1)
//		gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
//	else if (shadowMode == 2)
//		gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
//	else
		gl_FragColor = vec4((ambient + diffuse + specular).xyz, 1.0);
}
