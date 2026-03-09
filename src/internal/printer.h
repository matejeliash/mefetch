#ifndef PRINTER_H
#define PRINTER_H

// color codes in ANSI for color prinrting
#define COLOR_GREEN "\033[0;36m"
//#define COLOR_BLUE  "\033[0;34m"  // blue was incorrectly green

#define COLOR_RESET "\033[0m"

#define BB "\033[40m"
#define BW "\033[47m"
#define BR "\033[41m"

#define MAX_LINE_WIDTH 256


extern char SELECTED_COLOR[64];


#include "sys_info.h"

void print_user_info();
void print_title(char* title);
void print_colors();



void add_laptop_info(char arr[][256], int* idx);
void add_desktop_info(char arr[][MAX_LINE_WIDTH], int* idx);
void add_cpu_info(char arr[][MAX_LINE_WIDTH],int* idx);
void add_disk_usage(char arr[][MAX_LINE_WIDTH], int* idx);
void add_locale(char arr[][MAX_LINE_WIDTH], int* idx);
void add_shell(char arr[][MAX_LINE_WIDTH], int* idx);
void add_kernel(char arr[][MAX_LINE_WIDTH], int* idx);
void add_uptime(char arr[][MAX_LINE_WIDTH], int* idx);
void add_os_pretty_name(const char* pretty_name,char arr [][256],int* idx);
void add_mem_used(char arr[][256],int* idx);
void add_package_count(OsInfo* info,char arr[][256],int *idx);
void add_net_info(char arr[][256],int* idx);


//void print_desktop_info();
//void print_cpu_info();
//void print_package_count(OsInfo* info);
//void print_locale();
//void print_shell();
//void print_os_pretty_name(const char* pretty_name);
//void print_kernel();
//void print_uptime();
//void print_mem_used();
//void print_net_info();
//void print_laptop_info();
//void print_disk_usage();

#endif
