#version 300 es
precision mediump float;
layout(location = 0) uniform sampler2D y_texture;
layout(location = 1) uniform sampler2D u_texture;
layout(location = 2) uniform sampler2D v_texture;
layout(location = 3) uniform sampler2D uv_texture;
uniform in int yuvType;//0 代表 I420, 1 代表 NV12, 2 代表NV21
in vec2 v_texcoord;
out vec4 outColor;

void main() {
    vec3 yuv;
    yuv.x = texture(y_texture, v_texcoord).r;
    if (yuvType == 0) {
        // 因为I420是YUV的一个平面，所以采样后的r,g,b,a这四个参数的数值是一样的
        yuv.y = texture(uv_texture, v_texcoord).r-0.5;
        yuv.z = texture(uv_texture, v_texcoord).r-0.5;
    } else if (yuvType == 1) {
        // 因为NV12是2平面的，对于UV平面，在加载纹理时，会指定格式，让U值存在r,g,b中，V值存在a中
        yuv.y = texture(uv_texture, v_texcoord).r-0.5;
        yuv.z = texture(uv_texture, v_texcoord).a-0.5;
    } else {
        // 因为NV21是2平面的，对于UV平面，在加载纹理时，会指定格式，让U值存在a中，V值存在r,g,b中
        yuv.y = texture(uv_texture, v_texcoord).a-0.5;
        yuv.z = texture(uv_texture, v_texcoord).r-0.5;
    }

    highp vec3 rgb = mat3(1, 1, 1,
                        0, -0.344, 1.770,
                        1.403, -0.714, 0) * yuv;
    outColor = vec4(rgb, 1);
}
