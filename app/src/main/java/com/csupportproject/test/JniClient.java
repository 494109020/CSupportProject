//package com.csupportproject.test;
//
///**
// * Created by Magina on 16/9/27.
// * 类功能介绍: jni使用的时候一定注意，包名什么的要和原文件所在包名完全一致。
// * 因为native方法签名是和java方法的全限定包名＋方法名相关的。一旦变动过，就会导致方法找不到
// */
//
//public class JniClient {
//
//    static {
//        System.loadLibrary("native-lib");
//    }
//
//
//    public static native String getStaticJniString();
//
//    public native String getJniString();
//
//}
