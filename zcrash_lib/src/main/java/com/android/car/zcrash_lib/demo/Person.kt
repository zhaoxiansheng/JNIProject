package com.android.car.zcrash_lib.demo

class Person(var name : String = "", var age: Int = 0) {

    constructor(name: String) : this() {
        this.name = name
    }

    override fun toString(): String {
        return "$name: $age"
    }
}