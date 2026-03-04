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
#include <sys/stat.h>

char SELECTED_COLOR[64] = COLOR_GREEN;

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
    printf("%s%s%s@%s%s%s\n",SELECTED_COLOR,pw->pw_name,COLOR_RESET,SELECTED_COLOR,hostname,COLOR_RESET);

    int width = strlen(pw->pw_name) + 1 + strlen(hostname);

    for(int i=0;i<width;i++){
        printf("-");
    }
    printf("\n");


}
void print_title(char* title){
    printf("%s%-15s%s: ",SELECTED_COLOR,title,COLOR_RESET);

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

    if (!cpu_info ){
        return;
    }
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
    char pkg_type [20] = "unknown";


    get_package_count(info,pkg_type,&count);

    print_title("Packages");
    if (count == -1){
        printf("unknown (%s)\n",pkg_type);
    }else{
        printf("%d (%s)\n",count,pkg_type);

    }


}


void print_locale(){

     setlocale(LC_ALL, "");   // init from env
     char *locale=setlocale(LC_CTYPE,NULL); // do not change just get current
    //char * locale = getenv("LANG");
    if (locale){
        print_title("Locale");
        printf("%s\n",locale);
    }
    // else{
    //     print_title("Locale");
    //     printf("%s\n","unknown");
    // }
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


    // printf("%ld MiB\n",sinfo.totalram * sinfo.mem_unit / 1024 / 1024);
    // printf("%ld MiB\n",sinfo.freeram * sinfo.mem_unit / 1024 / 1024);
    // printf("%ld MiB\n",(sinfo.bufferram + sinfo.freeram) * sinfo.mem_unit / 1024 / 1024);


}

void print_mem_used(){

    MemInfo mem_info= get_mem_info();

    double usedMB = (mem_info.mem_total - mem_info.mem_available) / 1024.0;
    double totalMB = mem_info.mem_total / 1024.0;

    double swapTotalMB = mem_info.swap_total  / 1024.0;
    double swapUsedMB =( mem_info.swap_total - mem_info.swap_free )/ 1024.0;

    print_title("Memory used");
    printf("%.2f/%.2f MB\n",usedMB,totalMB);
    print_title("Swap used");
    printf("%.2f/%.2f MB\n",swapUsedMB,swapTotalMB);

}


void print_net_info(){

    NetInfo net_info=get_net_info();
    //printf("%d interfaces",net_info.count);

    for(int i =0;i<net_info.count;i++){
        print_title("Local IP");
        printf("%s (interface: %s)\n",net_info.interfaces[i].ip,net_info.interfaces[i].name);

    }

    for(int i =0;i<net_info.count;i++){
        free(net_info.interfaces[i].ip);
        free(net_info.interfaces[i].name);
    }

    free(net_info.interfaces);


}

void print_laptop_info(){

    char info[256];
    info[0] = '\0';
    char buf[256];
    struct stat st;

    if (stat("/sys/class/dmi/id",&st)==0){

        FILE* file =fopen("/sys/class/dmi/id/sys_vendor","r");
        if(!file){
            return;
        }
        int len=fread(buf,1,256,file);
        buf[len-1] = '\0';
        //printf("%s\n",buf);

        fclose(file);
        strcat(info,buf);
        strcat(info," ");
        
        file =fopen("/sys/class/dmi/id/product_name","r");
        if(!file){
            return;
        }
        len=fread(buf,1,256,file);
        buf[len-1] = '\0';
        strcat(info,buf);

        fclose(file);

    }

    if (info[0]!='\0'){
        print_title("Laptop model");
        printf("%s\n",info);
    }

}

    void print_disk_usage(){



        AllPartitionsInfo* part_info = get_partitions();
        for(int i=0;i<part_info->count;i++){


            print_title("Disk usage");
            PartitionInfo part =part_info->parts[i];

            double usedGB=(part.total_bytes  - part.available_bytes)/1024.0/1204.0/1024.0;
            double totalGB=(part.total_bytes)/1024.0/1204.0/1024.0;


            printf("%.2f/%.2fGB (mounted: %s, fs: %s)\n",usedGB,totalGB,part.mnt_dir,part.mnt_type);

        }

        free_allPartitionsInfo(part_info);

        }
    