# my-dir宏返回Android.mk所在位置 即将LOCAL_PATH赋值
LOCAL_PATH:= $(call my-dir)

# 依赖多个第三方库就分别写

# 清理LOCAL_XXX变量的值
#include $(CLEAR_VARS)
# 模块名字
#LOCAL_MODULE :=testNdk
# 打包给模块的C/C++源码 头文件不用列出 此源文件可以用具体路径表示
#LOCAL_SRC_FILES :=libtestNdk.so
# 链接第三方,则为预编译
#include $(PREBUILT_SHARED_LIBRARY)



# 清理LOCAL_XXX变量的值
include $(CLEAR_VARS)
# 模块名字
LOCAL_MODULE :=bsdiff
# 打包给模块的C/C++源码 头文件不用列出 此源文件可以用具体路径表示
MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/bzip2/*.c)
LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)
# 编译输出 STATIC为静态库 SHARED为动态库 BUILD_EXECUTABLE为可执行程序
include $(BUILD_SHARED_LIBRARY)

# 清理LOCAL_XXX变量的值
include $(CLEAR_VARS)
# 日志模块
LOCAL_LDLIBS :=-llog
# 模块名字
LOCAL_MODULE :=testThirdNdk
# 打包给模块的C/C++源码 头文件不用列出 此源文件可以用具体路径表示
LOCAL_SRC_FILES :=prevent_debug.c test.c testThird.c
# 链接的第三方库,多个之间用空格分开
#LOCAL_SHARED_LIBRARIES:=testNdk
# 编译输出 STATIC为静态库 SHARED为动态库 BUILD_EXECUTABLE为可执行程序
include $(BUILD_SHARED_LIBRARY)


# 有两种编译方式,第一种,分开编译,然后后者链接前者;第二种,直接后者一次性编译两个文件.