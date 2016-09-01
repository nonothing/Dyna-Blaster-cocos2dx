const char* ccUIShaderWhite_frag = STRINGIFY(

\n#ifdef GL_ES\n
\nprecision lowp float;\n
\n#endif\n
\nvarying vec4 v_fragmentColor;\n
\nvarying vec2 v_texCoord;\n
\nuniform float t;\n
\n\n
\nvoid main()\n
\n{\n
	\nvec4 color = texture2D(CC_Texture0, v_texCoord);\n
	\ngl_FragColor = color.a * mix(color, vec4(1.0,1.0,1.0,1.0), t);\n
\n}\n
);