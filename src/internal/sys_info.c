#include <stdlib.h>
#include <string.h>
#include "str_utils.h"
#include "cmd_runner.h"
#include <stdio.h>
#include <stdbool.h>


#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>


#include <mntent.h>


#include "sys_info.h"

// parse line from /ets/os-release  so we get value from key value pair
char* os_release_get_value(char* line, char* prefix){
    char* value = line + strlen(prefix);

    size_t len = strlen(value);

    if(len > 0 &&  value[len-1] == '\n'){
        value[len-1] = '\0';
        len--;
    }
    if ( len > 0 && value[len-1] == '"'){
        value[len-1] = '\0';
    }

    if( len > 0 && value[0] == '"'){
        value++;
    }

    if (len <= 0){
        return NULL;
    }

    return strdup(value);


}

// find linux distro info,
// also used when we decide what package manager are are we expecting
OsInfo* get_os_info(){


    FILE *file = fopen("/etc/os-release","r");
    if (!file){
        return NULL;
    }

    char line [500];
    OsInfo* info = malloc(sizeof(OsInfo));
    info->pretty_name = NULL;
    info->id = NULL;
    info->id_like = NULL;

    while(fgets(line,sizeof(line),file)){
        if(has_prefix(line,"PRETTY_NAME=")){
            info->pretty_name = os_release_get_value(line,"PRETTY_NAME=");
        }
        // distribution family
          if(has_prefix(line,"ID_LIKE=")){
             info->id_like = os_release_get_value(line,"ID_LIKE=");
         }
         // exact os type
         if (has_prefix(line,"ID=")){
            info->id = os_release_get_value(line,"ID=");
            //printf("%s",info->id);
        }

    }

    if(!info->pretty_name){
        info->pretty_name = strdup("unknown");
    }
    if(!info->id_like){
        info->id_like = strdup("unknown");
    }
    if(!info->id){
        info->id = strdup("unknown");
    }


    fclose(file);

    return info;

}

void free_os_info(OsInfo *info){
    if (!info) return;

    free(info->id_like);
    free(info->pretty_name);
    free(info->id);
    free(info);
}



CpuInfo* get_cpu_info(){

    struct stat st;

    if (stat("/usr/bin/lscpu",&st) == -1){
        return NULL;
    }


    CpuInfo *info = malloc(sizeof(CpuInfo));

    //  we could also  read from file /proc/cpuinfo but data are strange on ARM


    char* cmd_argv[] ={"lscpu",NULL};


    char* stdout =run_cmd(cmd_argv);
    char* line = strtok(stdout, "\n");

    if (strlen(stdout) == 0){
        info->model_name = strdup("unknown");
        info->architecture = strdup("unknown");
        info->cpu_count = strdup("unknown");
        return info;
    }

    while(line !=NULL){
        if (has_prefix(line, "Architecture:") ){
            char *colon = strchr(line,':');
            colon++;
            char* trimmed =trim(colon);
            info->architecture = trimmed;
        }else if (has_prefix(line, "CPU(s):")){
            char *colon = strchr(line,':');
            colon++;
            char* trimmed =trim(colon);
            info->cpu_count = trimmed;
        }else if (strstr(line,"Model name:")!=NULL ){

            char *colon = strchr(line,':');
            colon++;
            char* trimmed =trim(colon);
            info->model_name = trimmed;

        }
         line = strtok(NULL, "\n");
    }

    free(stdout);

    return info;

}


// extract long value from /proc/meminfo
// e.g line -> <Type>: <value>
long mem_line_get_long(char* line){

    const char *p = strchr(line,':');
    if(!p) return -1;
    p++;

    while(*p == ' ' || *p == '\t') p++;

    return strtol(p,NULL,10);


}


MemInfo get_mem_info(){

    FILE* file = fopen("/proc/meminfo","r");

    char line [500];

    MemInfo info;
    info.mem_total = -1;
    info.mem_available = -1;

    while(fgets(line,sizeof(line),file)){
        if(has_prefix(line,"MemTotal:")){
            info.mem_total = mem_line_get_long(line);
        }
        else if(has_prefix(line,"MemAvailable:")){
            info.mem_available =  mem_line_get_long(line);
        }

        else if(has_prefix(line,"SwapTotal:")){
            info.swap_total=  mem_line_get_long(line);
        }
        
        else if(has_prefix(line,"SwapFree:")){
            info.swap_free =  mem_line_get_long(line);
        }
    }


    fclose(file);

    return info;


    
}


NetInfo get_net_info(){

    NetInfo info;
    info.count = 0;
    info.interfaces=  NULL;


    struct ifaddrs *if_arr, *if_cur; // list of interface and cur for iterating
    char ip[INET_ADDRSTRLEN]; // store ipv4 address

    if (getifaddrs(&if_arr) == -1) {
        return info;
    }

    int count = 0;
    
    for (if_cur = if_arr; if_cur != NULL; if_cur = if_cur->ifa_next) {
        if (!if_cur->ifa_addr)
            continue;

        // Check for IPv4 addresses only
        if (if_cur->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *sa = (struct sockaddr_in *)if_cur->ifa_addr;
            
            // convert to string e.g. 192.160.0.1
            inet_ntop(AF_INET, &(sa->sin_addr), ip, INET_ADDRSTRLEN);

            // Skip localhost
            if (strcmp(ip, "127.0.0.1") != 0) {
                //printf("%s: %s\n", if_cur->ifa_name, ip);
                count++;
            }
        }
    }

    //printf("%d\n",count);
    info.count = count;


    if (count ==0){
        freeifaddrs(if_arr);
        return info;

    }

    info.interfaces = malloc(count * sizeof(NetInterface));

    int pos=0;

    for (if_cur = if_arr; if_cur != NULL; if_cur = if_cur->ifa_next) {
        if (!if_cur->ifa_addr)
            continue;

        // Check for IPv4 addresses only
        if (if_cur->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *sa = (struct sockaddr_in *)if_cur->ifa_addr;
            
            // convert to string e.g. 192.160.0.1
            inet_ntop(AF_INET, &(sa->sin_addr), ip, INET_ADDRSTRLEN);

            // Skip localhost
            if (strcmp(ip, "127.0.0.1") != 0) {
                info.interfaces[pos].name = strdup(if_cur->ifa_name);
                info.interfaces[pos].ip = strdup(ip);
                pos++;
            }
        }

    }

    freeifaddrs(if_arr);
    return info;
}

bool is_fs_allowed(const char* fs){
    char* allowed[] = {"ext2","ext3","ext4","fat16",
        "fat32","vfat","exfat","ntfs",
        "fuse.sshfs",
        NULL
    };
    int idx = 0;
    while(allowed[idx] !=NULL){
        if(strcmp(allowed[idx],fs)==0){
            return true;

        }
        idx++;
    }

    return false;
}


void free_partitions(PartitionInfo* partitions,int count){
    if (partitions){
        for(int i =0;i<count;i++){
            free(partitions[i].mnt_fsname);
            free(partitions[i].mnt_dir);
            free(partitions[i].mnt_type);
        }
        free(partitions);
    }

}

void free_allPartitionsInfo(AllPartitionsInfo* info){
    free_partitions(info->parts,info->count);
    free(info);

}


AllPartitionsInfo*  get_partitions(){

    FILE* file = fopen("/proc/mounts","r");
    if(!file){
        return NULL;
    }

    int cap = 4;// just average number of mounted fs 
    int count =0;
    PartitionInfo* partitions  = malloc(cap * sizeof(PartitionInfo));
    if(!partitions) {
        fclose(file);
        return NULL;
    } 

    struct mntent *mounted;

    struct statvfs st;

    while((mounted=getmntent(file))!=NULL){
        char* fs = mounted->mnt_type;

        if(is_fs_allowed(fs)){

            // cap reached double size and realloc
            if(cap == count){
                cap *=2;
                
                PartitionInfo* tmp = realloc(partitions,cap * sizeof(PartitionInfo));
                // realloc fails
                if (!tmp){
                    free_partitions(tmp,count);
                    fclose(file);
                    return NULL;
                }
                partitions = tmp;
            }

            // get disk space usage 
            partitions[count].total_bytes = 0;
            partitions[count].available_bytes = 0;
            if(statvfs(mounted->mnt_dir,&st)==0){
                partitions[count].total_bytes = st.f_blocks * st.f_bsize;
                partitions[count].available_bytes = st.f_bavail * st.f_bsize;
            }

            partitions[count].mnt_fsname= strdup(mounted->mnt_fsname);
            partitions[count].mnt_dir= strdup(mounted->mnt_dir);
            partitions[count].mnt_type= strdup(mounted->mnt_type);
            count++;
        }
    }

    fclose(file);

    AllPartitionsInfo *info = malloc(sizeof(AllPartitionsInfo));
    if(!info){
        free_partitions(partitions,count);
        return NULL;

    }
    info->count = count;
    info->parts = partitions;

    return info;

    }





