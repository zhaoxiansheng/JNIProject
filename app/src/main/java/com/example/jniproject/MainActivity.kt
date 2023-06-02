package com.example.jniproject

import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import com.android.car.zcrash_lib.demo.NativeLib
import com.android.car.zcrash_lib.demo.Person
import com.example.jniproject.databinding.ActivityMainBinding

const val TAG = "MainActivity"

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val nativeLib = NativeLib()
        binding.sampleText.text = nativeLib.stringFromJNI()
        nativeLib.visitField()
        val persons = nativeLib.createPersons()
        val names: Array<String> = arrayOf("子鼠", "丑牛", "寅虎", "卯兔", "辰龙")
        val personNames = nativeLib.getPersons(names)

        Log.d(
            TAG,
            "onCreate: name: " + nativeLib.name
                    + "\nstaticName: " + NativeLib.staticName
                    + "\ncreatePerson: " + nativeLib.createPerson().toString()
        )
        for (person in persons) {
            Log.d(TAG, "createPersons: $person")
        }

        for (person in personNames) {
            Log.d(TAG, "getPersons: $person")
        }
    }
}