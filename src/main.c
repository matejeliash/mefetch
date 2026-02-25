#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "stdbool.h"

#include <sqlite3.h>
#include <pwd.h>
#include "internal/cmd_runner.h"
#include "internal/package_count.h"
#include "internal/str_utils.h"
#include "internal/printer.h"
#include "internal/sys_info.h"



int main(){

    printf("\n\n");
    print_user_info();
    OsInfo * info = get_os_info();
    print_os_pretty_name(info->pretty_name);
    print_kernel();
    print_desktop_info();
    print_cpu_info();
    print_package_count(info);
    free(info->pretty_name);
    free(info->id);
    free(info->id_like);
    free(info);
    print_locale();
    print_shell();
    print_uptime();
    print_mem_used();
    print_net_info();
    printf("\n");
    print_colors();
    printf("\n\n");




   // get_mem_info();





}
