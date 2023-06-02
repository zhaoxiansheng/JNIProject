package com.example.jniproject

import android.annotation.SuppressLint
import android.os.Build
import android.text.TextUtils

class Utils {
    companion object {

        @SuppressLint("ObsoleteSdkInt")
        fun getAbiList(): String {
            return if (Build.VERSION.SDK_INT >= 21) {
                TextUtils.join(",", Build.SUPPORTED_ABIS)
            } else {
                val abi = Build.CPU_ABI
                val abi2 = Build.CPU_ABI2
                if (TextUtils.isEmpty(abi2)) {
                    abi
                } else {
                    "$abi,$abi2"
                }
            }
        }
    }
}