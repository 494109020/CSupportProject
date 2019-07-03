#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <jni.h>
#include <android/log.h>

#define  LOGD(...)  __android_log_print(ANDROID_LOG_INFO, "========= Debug =========", __VA_ARGS__)

// 这是一个反调试的功能.通过检测/proc/(pid)/status中的内容,来判断当前进程是否正在被
// 另一个进程监控调试.如果是,则杀掉调试进程.

// 自身应用的签名值
const char *origin = "308202c1308201a9a003020102020414c26059300d06092a864886f70d01010b05003011310f300d06035504031306776977696465301e170d3136313031383037333732315a170d3431313031323037333732315a3011310f300d0603550403130677697769646530820122300d06092a864886f70d01010105000382010f003082010a0282010100b5b79f3dea6b3b37528fb7e941a79440f9883b7e453333dbb3d0cafadc3fb89dab59b85338d12b0b2fe1df2e9457e06d358db00dfdebdf5334b7e07cf1b07624aef83eed39fefe68cf15deeb1a22016043e87d07b24e805c2e5319c1453fd0f2600a38459506e6577c1271c442c1010565d2552ce1cb2a52120e5b47fa0050efec6253e86054b364c6a5433c3fdc8012f8d9eeeaa4c4180e9456e74021b4fb217a5508c678cee0c757550e5a95ddc58c0fc045af81b4b968979867362d7c06b09ed5a987fd5c0cd59695bc6d4ca12e3abb9b69a0493d105b951d549cd9ecf11da9940ace45e41b538e64fda5c6f0a47df671cf9f478286a9b2b7cfefa3e30d4d0203010001a321301f301d0603551d0e041604142e474582ddb194708f51ebcab7702339680181cf300d06092a864886f70d01010b0500038201010024286bf5416433a21207b58573fd27a2fed82be52a43539bb102f67c3afefe984a34e9f978ddf2ef4f9c477d50332d908652465a5f137328adc525c40ad624fff23caa4db7d0bd7d252526b1d614c04b430ba925a19712e91eac3a0fdb41538d221c8198c9a1f9f14d598eed3ee5a1423ca1d27ee84266f305e45728a84fcb476a44f45c9eb7e67151b8455fb8c8a3f9f67c6f212f55d472965f6cfe97e2e392debbec4c067667f2839aa6c9db2c458194c51699b6b8816fe25f38dac1c000bd62ce2bbb0e82843f4110acdf9c39b384d73f63fb867074bbe0e0c9659e10606c93bd906f303e30d04836809e97cd0b3eaec9a184c6aaaab78a87d394fb6b1b8f";

void check_debug(JNIEnv *env);

jint check_sign(JNIEnv *env);

jobject getApplication(JNIEnv *env);

void *thread_function(void *arg) {
    int pid = getpid();
    char file_name[20] = {'\0'};
    sprintf(file_name, "proc/%d/status", pid);
    const int bufsize = 1024;
    char line_str[bufsize];
    int i = 0, traceid;
    FILE *fp;
    while (1) {
        i = 0;
        fp = fopen(file_name, "r");
        if (fp == NULL) {
            break;
        }
        while (fgets(line_str, bufsize, fp)) {
            if (strncmp(line_str, "TracerPid", 9) == 0) {
                traceid = atoi(&line_str[10]);
                if (traceid > 0) {
                    fclose(fp);
                    kill(pid, SIGKILL);
                }
            }
        }
        fclose(fp);
        sleep(5);
    }
}


void create_thread_check_traceid() {
    pthread_t thread_id;
    int err = pthread_create(&thread_id, NULL, thread_function, NULL);
    if (err != 0) {

    }
}

void check_debug(JNIEnv *env) {
    jclass clz = (*env)->FindClass(env, "com/csupportproject/BuildConfig");
    if (clz == NULL) {
        LOGD("not found com.csupportproject.BuildConfig");
    } else {
        LOGD("found com.csupportproject.BuildConfig");
        jfieldID field = (*env)->GetStaticFieldID(env, clz, "DEBUG", "Z");
        jboolean is_debug = (*env)->GetStaticBooleanField(env, clz, field);
        if (!is_debug) {
            create_thread_check_traceid();
        }
    }
    (*env)->DeleteLocalRef(env, clz);
}

jobject getApplication(JNIEnv *env) {
    jclass localClass = (*env)->FindClass(env, "android/app/ActivityThread");
    if (localClass != NULL) {
        // LOGI("class have find");
        jmethodID getapplication = (*env)->GetStaticMethodID(env, localClass, "currentApplication",
                                                             "()Landroid/app/Application;");
        if (getapplication != NULL) {
            jobject application = (*env)->CallStaticObjectMethod(env, localClass, getapplication);
            return application;
        }
        return NULL;
    }
    return NULL;
}

void exitApplication(JNIEnv *env, jint flag) {
    jclass temp_clazz = NULL;
    jmethodID mid_static_method;
    // 1、从classpath路径下搜索ClassMethod这个类，并返回该类的Class对象
    temp_clazz = (*env)->FindClass(env, "java/lang/System");
    mid_static_method = (*env)->GetStaticMethodID(env, temp_clazz, "exit", "(I)V");
    (*env)->CallStaticVoidMethod(env, temp_clazz, mid_static_method, flag);
    (*env)->DeleteLocalRef(env, temp_clazz);
}

jint check_sign(JNIEnv *env) {
    // Application object
    jobject application = getApplication(env);
    if (application == NULL) {
        return JNI_ERR;
    }
    // Context(ContextWrapper) class
    jclass context_clz = (*env)->GetObjectClass(env, application);
    // getPackageManager()方法
    jmethodID getPackageManager = (*env)->GetMethodID(env, context_clz, "getPackageManager",
                                                      "()Landroid/content/pm/PackageManager;");
    // 获取PackageManager实例
    jobject package_manager = (*env)->CallObjectMethod(env, application, getPackageManager);
    // PackageManager class
    jclass package_manager_clz = (*env)->GetObjectClass(env, package_manager);
    // getPackageInfo()方法
    jmethodID getPackageInfo = (*env)->GetMethodID(env, package_manager_clz, "getPackageInfo",
                                                   "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    // getPackageName()方法
    jmethodID getPackageName = (*env)->GetMethodID(env, context_clz, "getPackageName",
                                                   "()Ljava/lang/String;");
    // 调用getPackageName()
    jstring package_name = (jstring) ((*env)->CallObjectMethod(env, application, getPackageName));
    // PackageInfo实例
    jobject package_info = (*env)->CallObjectMethod(env, package_manager, getPackageInfo,
                                                    package_name, 64);
    // PackageInfo class
    jclass package_info_clz = (*env)->GetObjectClass(env, package_info);
    // signatures字段
    jfieldID signatures_field = (*env)->GetFieldID(env, package_info_clz, "signatures",
                                                   "[Landroid/content/pm/Signature;");
    jobject signatures = (*env)->GetObjectField(env, package_info, signatures_field);
    jobjectArray signatures_array = (jobjectArray) signatures;
    jobject signature0 = (*env)->GetObjectArrayElement(env, signatures_array, 0);
    // Signature class
    jclass signature_clz = (*env)->GetObjectClass(env, signature0);
    // toCharsString()方法
    jmethodID toCharsString = (*env)->GetMethodID(env, signature_clz, "toCharsString",
                                                  "()Ljava/lang/String;");
    // 调用toCharsString()
    jstring signature_str = (jstring) ((*env)->CallObjectMethod(env, signature0, toCharsString));
    // 最终的签名串
    const char *sign = (*env)->GetStringUTFChars(env, signature_str, NULL);
    int result = strcmp(sign, origin);
    (*env)->DeleteLocalRef(env, application);
    (*env)->DeleteLocalRef(env, context_clz);
    (*env)->DeleteLocalRef(env, package_manager);
    (*env)->DeleteLocalRef(env, package_manager_clz);
    (*env)->DeleteLocalRef(env, package_name);
    (*env)->DeleteLocalRef(env, package_info);
    (*env)->DeleteLocalRef(env, package_info_clz);
    (*env)->DeleteLocalRef(env, signatures);
    (*env)->DeleteLocalRef(env, signature0);
    (*env)->DeleteLocalRef(env, signature_clz);
    (*env)->ReleaseStringUTFChars(env, signature_str, sign);
    (*env)->DeleteLocalRef(env, signature_str);
    if (result == 0) { // 签名一致
        return JNI_OK;
    }
    return JNI_ERR;
}

// * & 在声明变量的时候是分别代表指针类型以及引用类型的.
// 指针(带*)是直接指向值的,不带(*)则是指向内存地址的.而引用是指向存储该值的内存地址的.
// 作为操作符的话,通俗点说,*是取指针指向的值,&是取变量的内存地址.
// 而变量是直接指向值的,变量名称在C中是不分配空间的,直接被编译为内存地址了.
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    // 获取JNIEnv
    (*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_6);
    check_debug(env);
    if (check_sign(env) == JNI_ERR) {
        exitApplication(env, 0);
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}


