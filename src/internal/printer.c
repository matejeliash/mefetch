#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

#include "sys_info.h"
#include "package_count.h"
#include "printer.h"
#include <sys/utsname.h>
#include <sys/sysinfo.h>


// 
void print_user_info(){
    // get hostname
    char hostname[100];
    if ( gethostname(hostname, sizeof(hostname))!=0){
        return;
   }

    //get username
    unsigned int uid = getuid();
    struct passwd *pw =  getpwuid( uid);
    if(!pw){
        return;
    }
    printf("%s%s%s@%s%s%s\n",COLOR_GREEN,pw->pw_name,COLOR_RESET,COLOR_GREEN,hostname,COLOR_RESET);

}
void print_title(char* title){
    printf("%s%s%s:",COLOR_GREEN,title,COLOR_RESET);

}
void print_desktop_info(){
    char* session_type=getenv("XDG_SESSION_TYPE");
    char* desktop=getenv("XDG_CURRENT_DESKTOP");
    if (session_type && session_type[0] !='\0'){
        if ( desktop && desktop[0] !='\0'){
            print_title("Desktop");
            printf("%s(%s)\n",desktop,session_type);
        }
    }

}


void print_cpu_info(){

    CpuInfo *cpu_info = get_cpu_info();
    print_title("CPU");
    printf("%s %s (%s cores)\n",
        cpu_info->model_name,
        cpu_info->architecture,
        cpu_info->cpu_count
    );

    free(cpu_info->model_name);
    free(cpu_info->architecture);
    free(cpu_info->cpu_count);
    free(cpu_info);

}

// later change to switch and probably move to package_count.c

void print_package_count(OsInfo* info){

    int count = -1;
    char pkg_type [10];
    strcpy(pkg_type,"unknown");
    // printf("packages: %d\n",count);
    if( (strcmp(info->id,"debian") == 0 )|| (strstr(info->id_like,"debian") !=NULL) ){
        count = get_package_count_deb( );
        strcpy(pkg_type,"dpkg");
    }
    else if( (strcmp(info->id,"fedora") == 0 )|| (strstr(info->id_like,"fedora")!= NULL) ){
        count = get_package_count_fedora();
        strcpy(pkg_type,"rpm");
    }
    print_title("Packages");
    printf("%d (%s)\n",count,pkg_type);

}


void print_locale(){

    setlocale(LC_ALL, "");   // init from env
    char *locale=setlocale(LC_ALL,NULL); // do not change just get current
    if (locale){
        print_title("Locale");
        printf("%s\n",locale);
    }else{
        print_title("Locale");
        printf("%s\n","unknown");

    }
}


const char* get_filename(const char* path){
    const char* ptr = strrchr(path,'/'); // move to last / 
    
    // of ptr == NULL that it is just filename
    return ptr ? ptr+1 : path;

}

void print_shell(){

    char* shell = getenv("SHELL");
    if(shell){
        char path [256];
        char shell_name[256];
        
        // store proc filepath for process in path 
        sprintf(path, "/proc/%d/exe", getppid());
        
        // get path for executable from which process is created
        ssize_t len = readlink(path, shell_name, sizeof(shell_name) - 1);
        shell_name[len] = '\0';
        if (len > 0){
            print_title("Shell");
            printf("%s (default: %s)\n",get_filename(shell_name),get_filename(shell));
        }

    }

}

void print_colors(){
    
    for (int i=0;i<8;i++){
        printf("\033[4%dm   %s",i,COLOR_RESET);
    }
    printf("\n");
    for (int i=0;i<8;i++){
        printf("\033[10%dm   %s",i,COLOR_RESET);
    }
    printf("\n");

}

void print_os_pretty_name(const char* pretty_name){
    print_title("OS");
    printf("%s\n",pretty_name);
}

void print_kernel(){
    struct utsname kernel_info;
    if(uname(&kernel_info)==0){
        print_title("Kernel");
        printf("%s %s %s\n",kernel_info.sysname,kernel_info.release,kernel_info.version);
    }

}

void print_uptime(){
    struct sysinfo sinfo;
    if (sysinfo(&sinfo) !=0){
        return;
    }

    //printf("uptime %ld\n ",sinfo.uptime);
    int days = sinfo.uptime / 86400 ;
    int rest = sinfo.uptime % 86400;
    int hours = rest / 3600 ;
    rest = rest % 3600;
    int mins = rest / 60;
    rest = rest %60;
    print_title("Uptime");
    printf("%d days  %02d:%02d:%02d\n",days,hours,mins,rest);


}
