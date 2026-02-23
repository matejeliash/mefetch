#include <stdlib.h>
#include <string.h>
#include "str_utils.h"
#include "cmd_runner.h"
#include <stdio.h>

#include "sys_info.h"

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



CpuInfo* get_cpu_info(){
    CpuInfo *info = malloc(sizeof(CpuInfo));

    //  we could also  read from file /proc/cpuinfo but data are strange on ARM


    char* cmd_argv[] ={"lscpu",NULL};

    char* stdout =run_cmd(cmd_argv);
    char* line = strtok(stdout, "\n");

    while(line !=NULL){
        if (has_prefix(line, "Architecture:") ){
            char *colon = strchr(line,':');
            colon++;
            char* trimmed =trim(colon);
            info-> architecture = trimmed;
        }else if (has_prefix(line, "CPU(s):")){
            char *colon = strchr(line,':');
            colon++;
            char* trimmed =trim(colon);
            info -> cpu_count = trimmed;
        }else if (strstr(line,"Model name:")!=NULL ){

            char *colon = strchr(line,':');
            colon++;
            char* trimmed =trim(colon);
            info -> model_name = trimmed;

        }
         line = strtok(NULL, "\n");
    }
    free(stdout);

    return info;

}




    
    // printf("%ld MiB\n",sinfo.totalram * sinfo.mem_unit / 1024 / 1024);
    // printf("%ld MiB\n",sinfo.freeram * sinfo.mem_unit / 1024 / 1024);
    // printf("%ld MiB\n",(sinfo.bufferram + sinfo.freeram) * sinfo.mem_unit / 1024 / 1024);


    // FILE* file = fopen("/proc/meminfo","r");

