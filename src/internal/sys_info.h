#ifndef HARDWARE_INFO_H
#define HARDWARE_INFO_H



// store basic CPU info
typedef struct CpuInfo{
    char* model_name;
    char* cpu_count;
    char* architecture;
} CpuInfo;


typedef struct OsInfo{
    char* pretty_name; // human readable extensive OS version
    char* id; // id is like debian | ubuntu 
    char* id_like; // e.g. ubuntu's  id_like == debian
}OsInfo;


OsInfo* get_os_info();

CpuInfo* get_cpu_info();

// typedef struct MemInfo{
//     char* avail_mem;
//     char*  total_mem;
// } MemInfo;
//
//

#endif
