//
// Created by jian.yeung on 2021/5/14.
//

#ifndef BBQVIDEO_BYTEUTILS_H
#define BBQVIDEO_BYTEUTILS_H

#include <vector>

std::vector<int> convertJavaArrayToVector(JNIEnv *env, jintArray intArray) {
    std::vector<int> v;
    jsize length = env->GetArrayLength(intArray);
    if (length > 0) {
        jint *elements = env->GetIntArrayElements(intArray, nullptr);
        v.insert(v.end(), &elements[0], &elements[length]);
        // Unpin the memory for the array, or free the copy.
        env->ReleaseIntArrayElements(intArray, elements, 0);
    }
    return v;
}

#endif //BBQVIDEO_BYTEUTILS_H
