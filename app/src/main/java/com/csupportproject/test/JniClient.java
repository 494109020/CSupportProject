package com.csupportproject.test;

/**
 * Created by Magina on 16/9/27.
 * 类功能介绍:
 */

public class JniClient {

    static {
        System.loadLibrary("native-lib");
    }


    public static native String getStaticJniString();

    public native String getJniString();

}
