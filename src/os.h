#ifndef OS_H
#define OS_H

/// OS-dependent functions

void os_android_clipboard_test();

/// Open a file chooser activity
/**
 * The function does not return anything. When the user selects a file, a
 * SDL_DropEvent is raised with the filename on it
 */
void os_android_open_file_chooser();

/// Open a file chooser GTK dialog
/**
 * The function does not return anything. When the user selects a file, a
 * SDL_DropEvent is raised with the filename on it
 */
void os_linux_open_file_chooser(char *buf);

/// Get path to the default program file
/**
 * Needs a big enough buffer, just use 256.
 * Returns 0 on success.
 */
int os_get_default_program_path(char* buf);

#endif
