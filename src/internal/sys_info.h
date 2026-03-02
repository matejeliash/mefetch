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
    long mem_total;
    long mem_available;
    long swap_total;
    long swap_free;
}MemInfo;

typedef struct NetInterface{
    char* name;
    char* ip;

}NetInterface;


typedef struct NetInfo{
    NetInterface* interfaces;
    int count;
}NetInfo;

typedef struct PartitionInfo{
    char* mnt_fsname;
    char* mnt_dir;
    char* mnt_type;
    unsigned long long total_bytes;
    unsigned long long  available_bytes;
} PartitionInfo;

typedef struct AllPartitionsInfo{
    PartitionInfo* parts;
    int count;
}AllPartitionsInfo;


OsInfo* get_os_info();
void free_os_info(OsInfo *info);

CpuInfo* get_cpu_info();

MemInfo get_mem_info();

NetInfo get_net_info();

void free_partitions(PartitionInfo* partitions,int count);
void free_allPartitionsInfo(AllPartitionsInfo* info);


AllPartitionsInfo*  get_partitions();

#endif
