#ifndef PACKAGE_COUNT_H
#define PACKAGE_COUNT_H

#include "sys_info.h"

int get_package_count_deb();
int get_package_count_deb2();

int get_package_count_fedora();

int get_package_count_arch();

int  get_package_count_rpm();

int  get_package_count_alpine();

void print_package_info();


void get_package_count(OsInfo* info, char* pkg_type, int* count);



#endif
