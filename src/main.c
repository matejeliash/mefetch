#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal/printer.h"
#include "internal/sys_info.h"

extern char SELECTED_COLOR[64];

void set_color(int argc,char* argv[]){

    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"--black") == 0){
            strcpy(SELECTED_COLOR,"\033[0;30m");
        }
        else if(strcmp(argv[i],"--red") == 0){
            strcpy(SELECTED_COLOR,"\033[0;31m");
        }
        else if(strcmp(argv[i],"--green") == 0){
            strcpy(SELECTED_COLOR,"\033[0;32m");
        }
        else if(strcmp(argv[i],"--yellow") == 0){
            strcpy(SELECTED_COLOR,"\033[0;33m");
        }
        else if(strcmp(argv[i],"--blue") == 0){
            strcpy(SELECTED_COLOR,"\033[0;34m");
        }
        else if(strcmp(argv[i],"--purple") == 0){
            strcpy(SELECTED_COLOR,"\033[0;35m");
        }
        else{
            printf("`%s` is unknown color, exiting program\n",argv[i]);
            exit(1);
        }

    }

}




int main(int argc,char* argv[]){

    //strcpy(SELECTED_COLOR,"\033[0;33m");

    set_color(argc,argv);

    OsInfo * os_info = get_os_info();

    printf("\n\n");

    print_user_info();
    print_os_pretty_name(os_info->pretty_name);
    print_kernel();
    print_desktop_info();
    print_cpu_info();
    print_package_count(os_info);
    print_locale();
    print_shell();
    print_uptime();
    print_mem_used();
    print_net_info();
    print_disk_usage();
    print_laptop_info();

    printf("\n");
    print_colors();

    printf("\n\n");

    free_os_info(os_info);





}
