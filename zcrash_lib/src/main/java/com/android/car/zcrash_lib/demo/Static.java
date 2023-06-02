package com.android.car.zcrash_lib.demo;

public class Static {

    public Shared() {
        System.loadLibrary("zcrash");
    }

    public static String staticName= "Static Cat";
    public String name = "Cat";

    public native String stringFromJNI();
    public native void visitField();
    public native Person createPerson();
    public native Person[] createPersons();
    public native Person[] getPersons(String[] names);
}
