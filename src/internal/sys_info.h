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


typedef struct MemInfo{
    long total;
    long available;
}MemInfo;

typedef struct NetInterface{
    char* name;
    char* ip;

}NetInterface;

typedef struct NetInfo{
    NetInterface* interfaces;
    int count;
}NetInfo;


OsInfo* get_os_info();

CpuInfo* get_cpu_info();


MemInfo get_mem_info();


NetInfo get_net_info();

#endif
