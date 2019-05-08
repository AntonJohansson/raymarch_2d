#version 430

layout(binding = 0) uniform sampler2D sampler;
in vec2 v_texcoords;
out vec4 gl_FragColor;

void main(void) {
    // Pass through our original color with full opacity.
    gl_FragColor = texture(sampler, v_texcoords);
}
