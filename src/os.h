#ifndef OS_H
#define OS_H

void os_android_clipboard_test();
void os_android_open_file_chooser();

/// Get path to the default program file
/**
 * Needs a big enough buffer, just use 256.
 * Returns 0 on success.
 */
int os_get_default_program_path(char* buf);

#endif
