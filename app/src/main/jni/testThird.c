#include "com_csupportproject_test_ThirdNdk.h"

JNIEXPORT jstring JNICALL Java_com_csupportproject_test_ThirdNdk_getString
        (JNIEnv *env, jclass clz) {
     return Java_com_csupportproject_test_NdkClient_getString(env, clz);
}