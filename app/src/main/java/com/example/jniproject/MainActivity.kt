package com.example.jniproject

import android.os.Build
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import com.android.car.zcrash_lib.demo.Shared
import com.android.car.zcrash_lib.demo.Static
import com.example.jniproject.Utils.Companion.getAbiList
import com.example.jniproject.databinding.ActivityMainBinding

const val TAG = "MainActivity"

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val static = Static()
        binding.sampleText.text = static.stringFromJNI()
        static.visitField()
        val persons = static.createPersons()
        val names: Array<String> = arrayOf("子鼠", "丑牛", "寅虎", "卯兔", "辰龙")
        val personNames = static.getPersons(names)

        Log.d(
            TAG,
            "Static: name: " + static.name
                    + "\nstaticName: " + Static.staticName
                    + "\ncreatePerson: " + static.createPerson().toString()
        )
        for (person in persons) {
            Log.d(TAG, "createPersons: $person")
        }

        for (person in personNames) {
            Log.d(TAG, "getPersons: $person")
        }

        Log.d(
            TAG,
            "Shared: init: " + Shared.nativeInit(
                Build.VERSION.SDK_INT,
                Build.VERSION.RELEASE,
                getAbiList()
            )
        )
    }
}