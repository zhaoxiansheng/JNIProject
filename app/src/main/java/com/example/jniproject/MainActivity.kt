package com.example.jniproject

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.android.car.zcrash_lib.NativeLib
import com.example.jniproject.databinding.ActivityMainBinding

const val staticName= "Static Cat"
class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val nativeLib = NativeLib();
        binding.sampleText.text = nativeLib.stringFromJNI()
        nativeLib.visitField()
    }
}