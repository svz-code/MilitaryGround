package org.example
import java.io.File

class App {
	external fun sayHello();
	companion object {
		init {
			val jarPath = File(
				App::class.java.protectionDomain.codeSource.location.toURI()
			).parent
			val libraryPath = "$jarPath/libarducat.so"
			System.load(libraryPath);
		}
	}
}

fun main() {
	println("Привет всем. Сегодня мы попытаемся подружить C++ и Kotlin");

	App().sayHello();
}



