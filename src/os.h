#ifndef OS_H
#define OS_H

/// OS-dependent functions

/// Open a file chooser activity
/**
 * The function does not return anything. When the user selects a file, a
 * SDL_DropEvent is raised with the filename on it (using the prefix "open:",
 * e.g.: "saveas:/home/martin/filename.bf").
 */
void os_android_open_file_chooser();

/// Open a "save file as" activity
/**
 * The function does not return anything. When the user selects a file, a
 * SDL_DropEvent is raised with the filename selected on it (using the prefix
 * "saveas:", e.g.: "saveas:/home/martin/filename.bf". Then you should save to
 * that filename.
 */
void os_android_save_file_as_chooser();

/// Open a file chooser GTK dialog
/**
 * Returns filename to open, otherwise leaves the buffer untouched.
 *
 * Use a big enough buffer.
 */
void os_linux_open_file_chooser(char *buf);

/// Open a "save file as" GTK dialog
/**
 * Returns filename where to save to, otherwise leaves the buffer untouched.
 * Then you should save to that filename.
 *
 * Use a big enough buffer.
 */
void os_linux_save_file_as_chooser(char *buf);

/// Get path to where the programs are saved
/**
 * Needs a big enough buffer, expects a buffer of size 256.
 * Returns 0 on success.
 * The path returned ends on /
 * The folder is created if it does not exist.
 */
int os_get_save_dir_path(char* buf);

/// Get path to the default program file
/**
 * Needs a big enough buffer, expects a buffer of size 256.
 * Returns 0 on success.
 */
int os_get_autosave_file_path(char* buf);

#endif
