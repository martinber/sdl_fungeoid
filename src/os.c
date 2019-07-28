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

void os_android_open_file_chooser()
{

    JNIEnv *env = (JNIEnv*) SDL_AndroidGetJNIEnv();

    jobject activity = (jobject) SDL_AndroidGetActivity();

    jclass class = (*env)->GetObjectClass(env, activity);

    jmethodID method_id = (*env)->GetMethodID(env, class, "open_file", "()V");

    (*env)->CallVoidMethod(env, activity, method_id);

    (*env)->DeleteLocalRef(env, activity);
    (*env)->DeleteLocalRef(env, class);
}

#endif

int os_get_default_program_path(char* buf) {

#ifdef __ANDROID__
    int storage_state = SDL_AndroidGetExternalStorageState();
    SDL_Log("ExternalStoragePath: %s", SDL_AndroidGetExternalStoragePath());
    SDL_Log("ExternalStorageState: %x, read: %x, write: %x",
            storage_state,
            SDL_ANDROID_EXTERNAL_STORAGE_READ, SDL_ANDROID_EXTERNAL_STORAGE_WRITE);
    if (storage_state
            & SDL_ANDROID_EXTERNAL_STORAGE_READ
            & SDL_ANDROID_EXTERNAL_STORAGE_WRITE
            != 0
    ) {
        strcpy(buf, SDL_AndroidGetExternalStoragePath());
        strcat(buf, "/program.bf");
    }
    else
    {
        SDL_Log("No external storage Read/Write permissions");
        return 1;
    }
#else
    strcpy(buf, "./program.bf");
#endif
    return 0;
}
