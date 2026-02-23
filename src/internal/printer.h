#ifndef PRINTER_H
#define PRINTER_H

// color codes in ANSI for color prinrting
#define COLOR_GREEN "\033[0;32m"
//#define COLOR_BLUE  "\033[0;34m"  // blue was incorrectly green

#define COLOR_RESET "\033[0m"

#include "sys_info.h"

void print_user_info();

void print_title(char* title);

void print_desktop_info();

void print_cpu_info();

void print_package_count(OsInfo* info);

void print_locale();
void print_shell();

void print_colors();

void print_os_pretty_name(const char* pretty_name);
void print_kernel();

void print_uptime();

#endif
