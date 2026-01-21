#include <iostream>
#include <jni.h>

extern "C" {

JNIEXPORT void JNICALL Java_org_example_App_sayHello (JNIEnv* env, jobject thisObject) {
    std::cout << "Здарова чувак, я с плюсов" << std::endl;
}

}
