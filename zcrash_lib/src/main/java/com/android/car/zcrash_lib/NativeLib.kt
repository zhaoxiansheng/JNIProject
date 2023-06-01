package com.android.car.zcrash_lib

class NativeLib {

    private val name = "Cat"
    /**
     * A native method that is implemented by the 'zcrash_lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String
    external fun visitField()

    companion object {
        // Used to load the 'zcrash_lib' library on application startup.
        init {
            System.loadLibrary("zcrash_lib")
        }
    }
}