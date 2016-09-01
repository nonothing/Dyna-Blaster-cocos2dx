const char* ccUIShaderWhite_vert = STRINGIFY(

\nattribute vec4 a_position;\n
\nattribute vec2 a_texCoord;\n
\nattribute vec4 a_color;\n
\n\n
\n#ifdef GL_ES\n
\nvarying lowp vec4 v_fragmentColor;\n
\nvarying mediump vec2 v_texCoord;\n
\n#else\n
\nvarying vec4 v_fragmentColor;\n
\nvarying vec2 v_texCoord;\n
\n#endif\n
\n\n
\nvoid main()\n
\n{\n
    \ngl_Position = CC_PMatrix * a_position;\n
    \nv_fragmentColor = a_color;\n
    \nv_texCoord = a_texCoord;\n
\n}\n
);