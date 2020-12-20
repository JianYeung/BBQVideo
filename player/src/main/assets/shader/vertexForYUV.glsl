#version 300 es
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texcoord;
uniform mat4 uMVPMatrix;
out vec2 v_texcoord;

void main() {
    v_texcoord = a_texcoord;
    gl_Position = uMVPMatrix * vec4(a_position, 1.0);
}