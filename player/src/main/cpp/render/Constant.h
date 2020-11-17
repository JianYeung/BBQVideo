//
// Created by jian.yeung on 2020/11/1.
//

#ifndef BBQVIDEO_CONSTANT_H
#define BBQVIDEO_CONSTANT_H

//每个Float占用4个字节
const int BYTES_PER_FLOAT = 4;
//每个Short占用2个字节
const int BYTES_PER_SHORT = 2;
//每个顶点有3个坐标值
const int COORDS_PER_VERTEX = 3;
//每个颜色有4个坐标值
const int COORDS_PER_COLOR = 4;
//每个纹理有2个坐标值
const int COODS_PRE_TEXTURE = 2;

const int CFG_RED_SIZE = 8;
const int CFG_GREEN_SIZE = 8;
const int CFG_BLUE_SIZE = 8;
const int CFG_ALPHA_SIZE = 8;
const int CFG_DEPTH_SIZE = 16;
const int CFG_STENCIL_SIZE = 8;
const int CFG_RENDERABLE_TYPE = 3;

#endif //BBQVIDEO_CONSTANT_H
