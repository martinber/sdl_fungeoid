#include "os.h"

#include <string.h>

#include "juan.h"

#ifdef __ANDROID__

#include <jni.h>

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

#else

#include <gtk/gtk.h>

void os_linux_open_file_chooser(char *buf)
{
    gtk_init(NULL, NULL);

    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint result;

    dialog = gtk_file_chooser_dialog_new("Open File", NULL, action,
            "_Cancel", GTK_RESPONSE_CANCEL,
            "_Open", GTK_RESPONSE_ACCEPT,
            NULL
        );

    result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        strcpy(buf, filename);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);

    // Otherwise the window freezes there
    while(gtk_events_pending())
    {
        gtk_main_iteration();
    }
}

#endif

int os_get_default_program_path(char* buf) {

    char filename[] = "_autosaved.bf";

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
        strcat(buf, "/");
        strcat(buf, filename);
    }
    else
    {
        SDL_Log("No external storage Read/Write permissions");
        return 1;
    }
#else
    strcpy(buf, "./");
    strcat(buf, filename);
#endif
    return 0;
}
