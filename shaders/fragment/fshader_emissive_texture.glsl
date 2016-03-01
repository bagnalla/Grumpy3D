varying vec2 fTextureCoord;

uniform sampler2D tex;

void main()
{
	//gl_FragColor = texture2D(tex, fTextureCoord);

	vec4 texColor = texture2D(tex, fTextureCoord);
	//if (texColor.w == 0.0)
    //    discard;
    gl_FragColor = texColor;
}
