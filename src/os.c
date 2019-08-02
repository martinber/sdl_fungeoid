#include "os.h"

#include <string.h>

#include "juan.h"

#ifdef __ANDROID__

#include <jni.h>

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

void os_android_save_file_as_chooser()
{

    JNIEnv *env = (JNIEnv*) SDL_AndroidGetJNIEnv();

    jobject activity = (jobject) SDL_AndroidGetActivity();

    jclass class = (*env)->GetObjectClass(env, activity);

    jmethodID method_id = (*env)->GetMethodID(env, class, "save_file_as", "()V");

    (*env)->CallVoidMethod(env, activity, method_id);

    (*env)->DeleteLocalRef(env, activity);
    (*env)->DeleteLocalRef(env, class);
}

#else

#include <gtk/gtk.h>
#include <stdio.h>
#include <sys/stat.h>

static void open_gtk_file_chooser
(
    char *title,
    char *button_text,
    GtkFileChooserAction action,
    char *buf
) {
    gtk_init(NULL, NULL);

    GtkWidget *dialog;
    gint result;

    dialog = gtk_file_chooser_dialog_new(title, NULL, action,
            "_Cancel", GTK_RESPONSE_CANCEL,
            button_text, GTK_RESPONSE_ACCEPT,
            NULL
        );
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);

    char folder[256] = "\0";
    os_get_save_dir_path(folder);
    gtk_file_chooser_set_current_folder(chooser, folder);

    result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
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

void os_linux_open_file_chooser(char *buf)
{
    open_gtk_file_chooser("Open file...", "_Open",
            GTK_FILE_CHOOSER_ACTION_OPEN, buf);
}

void os_linux_save_file_as_chooser(char *buf)
{
    open_gtk_file_chooser("Save file as...", "_Save",
            GTK_FILE_CHOOSER_ACTION_SAVE, buf);
}

#endif

int os_get_save_dir_path(char* buf) {

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
    }
    else
    {
        SDL_Log("No external storage Read/Write permissions");
        return 1;
    }
#else
    // Get the documents folder path
    FILE *process = popen("xdg-user-dir DOCUMENTS 2>/dev/null", "r");

    // Read stdout until a newline or end of FILE is found.
    fgets(buf, 256, process);

    // Remove last character because the process appends a newline at the end of
    // the path.
    buf[strlen(buf) - 1] = '\0';

    // Append folder to path
    strcat(buf, "/sdl_fungeoid/");

    // Create folder, should be recursive but I'm never going to create more
    // than one directory. If the folder already exists fails silently.
    mkdir(buf, 0755);

    SDL_Log("Path: %s", buf);

#endif
    return 0;
}

int os_get_autosave_file_path(char* buf) {

    char filename[] = "_autosaved.bf";

    if (os_get_save_dir_path(buf) == 0)
    {
        strcat(buf, filename);
        SDL_Log("Filename: %s", buf);
        return 0;
    } else {
        return 1;
    }
}
