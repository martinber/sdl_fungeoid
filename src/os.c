#include "os.h"

#ifdef __ANDROID__
#include <jni.h>

#include "juan.h"

void os_android_clipboard_test()
{
    /* JNIEnv* env = Android_JNI_GetEnv(); */
    JNIEnv *env = (JNIEnv*) SDL_AndroidGetJNIEnv();

    jobject activity = SDL_AndroidGetActivity();
    jclass mActivityClass = (*env)->GetObjectClass(env, activity);
    jmethodID midClipboardGetText = (*env)->GetStaticMethodID(env, mActivityClass,
                                "clipboardGetText", "()Ljava/lang/String;");


    char* text = NULL;
    jstring string;

    string = (*env)->CallStaticObjectMethod(env, mActivityClass, midClipboardGetText);
    if (string) {
        const char* utf = (*env)->GetStringUTFChars(env, string, 0);
        if (utf) {
            text = SDL_strdup(utf);
            (*env)->ReleaseStringUTFChars(env, string, utf);
        }
        (*env)->DeleteLocalRef(env, string);
    }
    SDL_Log("clipboard: %s", text);

    return;
}

void os_android_test()
{

    JNIEnv *env = (JNIEnv*) SDL_AndroidGetJNIEnv();

    jobject activity = (jobject) SDL_AndroidGetActivity();

    jclass class = (*env)->GetObjectClass(env, activity);

    jmethodID method_id = (*env)->GetMethodID(env, class, "test", "()V");

    (*env)->CallVoidMethod(env, activity, method_id);

    (*env)->DeleteLocalRef(env, activity);
    (*env)->DeleteLocalRef(env, class);
}

#endif
