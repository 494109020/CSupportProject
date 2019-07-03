#include "com_csupportproject_test_NdkClient.h"

JNIEXPORT jstring JNICALL Java_com_csupportproject_test_NdkClient_getString
        (JNIEnv *env, jclass clz) {
    return (*env)->NewStringUTF(env, "hello ndk client from third");
}

JNIEXPORT jstring JNICALL Java_com_csupportproject_test_NdkClient_getContent
        (JNIEnv *env, jobject obj) {
    return (*env)->NewStringUTF(env, "hello ndk client");
}