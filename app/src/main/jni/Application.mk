APP_PLATFORM := android-9
#NDK_TOOLCHAIN_VERSION := 4.9
#APP_CPPFLAGS := -frtti -std=c++11
APP_CXXFLAGS := -std=c++11 -D__STDC_LIMIT_MACROS
APP_CFLAGS += -O3
APP_CPPFLAGS += -O3
APP_CPPFLAGS += -fpermissive #for void* to pointer*
# 要构建的平台架构,多个之间用空格或者,分开
APP_ABI := armeabi-v7a