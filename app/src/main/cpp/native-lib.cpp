#include <jni.h>
#include <string>

/**
 * 这个最后会被打包成.so库。在Build->Analyze APK中的lib包下可以看到。
 */

extern "C"//这个貌似是必需加上的。要不然会报错。
JNIEXPORT jstring JNICALL//这里写法是多样的。同样不懂。也可以直接写成   jstring
//但是要注意这里要和java定义的返回值类型相同。

//C++方法签名规则:Java_调用类所在完整包名(其中"."用"_"代替)_类名_Java中定义的方法名
//如果Java中定义的方法是静态的,则第二个参数类型为jclass,如果不是静态的,则为jobject。
Java_com_csupportproject_test_JniClient_getStaticJniString(JNIEnv *env, jclass type) {

    std::string hello = "test";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_csupportproject_test_JniClient_getJniString(JNIEnv *env, jobject instance) {

    std::string hello = "test";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
jstring
Java_com_csupportproject_MainActivity_string(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_csupportproject_MainActivity_get(
        JNIEnv *env,
        jobject instance) {
    std::string hello = "HelloWorld";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
jintArray
Java_com_csupportproject_MainActivity_getInt(JNIEnv *env, jobject instance) {
    //相当于返回了一个[0,0]。并没有做初始赋值。
    return env->NewIntArray(2);
}