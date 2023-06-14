package com.example.jniproject

import android.app.NativeActivity
import android.content.Intent
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.View
import android.view.View.OnClickListener
import androidx.appcompat.app.AppCompatActivity
import com.android.car.zcrash.NativeHandler
import com.android.car.zcrash.demo.Static
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
            "Shared: init: " + NativeHandler.nativeInit(
                Build.VERSION.SDK_INT,
                Build.VERSION.RELEASE,
                getAbiList()
            )
        )

        binding.sampleText.setOnClickListener(object : OnClickListener{
            override fun onClick(p0: View?) {
                val intent = Intent(this@MainActivity, NativeActivity::class.java)
                startActivity(intent)
            }

        })
    }
}