package com.android.car.zcrash;

public class NativeHandler {
    public static native int nativeInit(int apiLevel, String osVersion, String abiList);
}
