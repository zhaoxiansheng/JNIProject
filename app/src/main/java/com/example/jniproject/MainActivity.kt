package com.example.jniproject

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.example.jniproject.databinding.ActivityMainBinding

const val staticName= "Static Cat"
class MainActivity : AppCompatActivity() {

    private val name = "Cat"

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.sampleText.text = stringFromJNI()
        visitField()
    }

    external fun stringFromJNI(): String
    external fun visitField()

    companion object {
        init {
            System.loadLibrary("jniproject")
        }
    }
}