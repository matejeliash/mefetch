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

    // char* cmd_args[] = {"asdasd","asdasd",NULL};
    // char* stdout = run_cmd(cmd_args);
    // printf("%s\n",stdout);
    // free(stdout);




   // get_mem_info();





}
