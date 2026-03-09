#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "internal/printer.h"
#include "internal/sys_info.h"

extern char SELECTED_COLOR[64];
bool enable_art = true;

void handle_cli_args(int argc,char* argv[]){

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
        else if (strcmp(argv[i],"--no-art")==0){
            enable_art = false;
            
        }
        else if (strcmp(argv[i],"--help") == 0 || strcmp(argv[i],"-h")==0){
            printf("help will be added later");
            exit(0);
        }
        else{
            printf("`%s` is unknown color, exiting program\n",argv[i]);
            exit(1);
        }

    }

}


//     char *art[] = {

//     "          ,,         ",
//     "          || |       ",
//     "          || |    ,  ",
//     "          || |  | |  ",
//     "          || '--' |  ",
//     "    ,,    || .----'  ",
//     "   || |   || |       ",
//     "   |  '---'| |       ",
//     "   '------.| |       ",
//     "   ((_))  || |       ",
//     "   (o o)  || |       ",
//     "____\\_/___||_|_____  ",
//     NULL
// };




    char* art[] = {
   "          ___,           ",
   "     _.-'` __|__         ",
   "   .'  ,-:` \\;',`'-,     ",
   "  /  .'-;_,;  ':-_;,'\\   ",
   " /  /;   '/    ,  _`.-\\  ",
   "|  | '`. (`     /` ` \\`| ",
   "|  |:.  `\\`-.   \\_   / | ",
   "|  |     (   `,  .`\\ ;'| ",
   " \\  \\     | .'     `-'/  ",
   "  \\  `.   ;/        .'   ",
   "   '._ `'-._____.-'`     ",
   "      `-.____|           ",
   "        _____|_____      ",
   "       /___________\\     ",
   NULL
};




int main(int argc,char* argv[]){

    //strcpy(SELECTED_COLOR,"\033[0;33m");

    handle_cli_args(argc,argv);

    OsInfo * os_info = get_os_info();








    char data_lines[50][256];
    for(int i=0;i<50;i++){
        data_lines[i][0] = '\0';
    }

int used_idx = 0;

add_os_pretty_name(os_info->pretty_name,data_lines,&used_idx);
add_kernel(data_lines,&used_idx);
add_desktop_info(data_lines,&used_idx);
add_cpu_info(data_lines,&used_idx);
add_uptime(data_lines, &used_idx);
add_disk_usage(data_lines,&used_idx);
add_locale(data_lines,&used_idx);
add_shell(data_lines,&used_idx);
add_mem_used(data_lines,&used_idx);
add_package_count(os_info,data_lines,&used_idx);
add_laptop_info(data_lines,&used_idx);
add_net_info(data_lines,&used_idx);


//int globe_idx = 0;
    printf("\n");
print_user_info();


int data_idx = 0;
int art_idx =0;

while(1){

    if (enable_art){
        if(art[art_idx]!=NULL){
            printf("%s",art[art_idx]);
            art_idx++;
        }else{
            for (size_t i=0;i<strlen(art[0]);i++){
                printf(" ");
            }
        }
    }
    
    if(data_idx< used_idx){
         printf("%s",data_lines[data_idx]);
         data_idx++;
    }

    if (data_idx>= used_idx ){
        if ((enable_art && art[art_idx]==NULL )|| !enable_art){
            break;
        }

    }

    printf("\n");
}

    printf("\n\n");
    print_colors();

    free_os_info(os_info);
}
