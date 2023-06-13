package com.android.car.zcrash.demo;

public class Static {

    public Static() {
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
