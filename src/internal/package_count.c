#include "package_count.h"

#include "cmd_runner.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//#include <sqlite3.h>



int get_package_count_deb(){

    // dpkg is better option that apt, that is not stable for scripting usage
    char* argv[] = {"dpkg","-l",NULL};
    char* str = run_cmd(argv);
    if (!str){
        return -1;
    }
    int count  = 0;
    char * line_start=str;

    while(*line_start){
                                                           // select also 
        // if ((line_start[0] == 'i' && line_start[1] =='i') ||(line_start[0] == 's' && line_start[1] == 'i')) {
        //     count++;
        // }

        if (line_start[1] =='i') {
            count++;
        }

        char* next_newline= strchr(line_start,'\n');
        if (!next_newline) break;
        line_start = next_newline + 1;
    }

    free(str);

    return count;
}


int get_package_count_deb2(){


    FILE *file = fopen("/var/lib/dpkg/status","r");
    if (!file){
        return -1;
    }
    int count = 0;
    char line [8024];

    while(fgets(line,sizeof(line),file)){
        if (strstr(line,"Status:")!=NULL && strstr(line,"installed")!=NULL){
            count++;
        }
    }

    fclose(file);

    return count;
}

// // works for newer RedHat distros like fedora, this is not reliable
// int get_package_count_fedora() {

//     // sqlite db used by dnf5>
//     char * db_path = "/var/lib/rpm/rpmdb.sqlite";

//     int rc;
//     sqlite3 *db;
//         rc = sqlite3_open(db_path, &db);

//     if (rc != SQLITE_OK) {
//         //printf("Cannot open DB: %s\n", sqlite3_errmsg(db));
//         if(db) sqlite3_close(db);

//         return -1;
//     }
//     sqlite3_stmt *stmt;

//     // select number of rows from Packages table
//     const char *sql = "SELECT COUNT(1) FROM Packages;";

//     // Prepare statement
//     rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
//     if (rc != SQLITE_OK) {
//         //printf("Prepare error: %s\n", sqlite3_errmsg(db));
//         sqlite3_close(db);
//         return -1;
//     }

//     // just retrieve one row
//     rc = sqlite3_step(stmt);
//     int count ;

//     if (rc == SQLITE_ROW) {
//          count = sqlite3_column_int(stmt,0);
//     }else{
//        count = -1;
//     }


//     // cleanup
//     sqlite3_finalize(stmt);
//     sqlite3_close(db);
//     return count;
// }



int  get_package_count_rpm(){

    char* cmd[] = { "rpm", "-qa", "--nosignature", "--nodigest", "--qf", "'%{NAME}\n'", NULL};
    //int count = run_cmd_line_counter(cmd);
    int count = stdout_line_counter(cmd);
    return count;
}

int get_package_count_arch(){
    char* cmd[] = { "pacman","-Qq",NULL};
    int count = run_cmd_line_counter(cmd);
    return count;

}


int get_package_count_alpine(){


    FILE *file = fopen("/lib/apk/db/installed","r");
    if (!file){
        return -1;
    }
    int count = 0;
    char line [8024];

    while(fgets(line,sizeof(line),file)){
        if (line[0] == 'P'){
            count++;
        }
    }

    fclose(file);

    return count;
}



void get_package_count(OsInfo* info, char* pkg_type, int* count){
    // printf("packages: %d\n",count);
    if( (strcmp(info->id,"debian") == 0 )|| (strstr(info->id_like,"debian") !=NULL) ){
        *count = get_package_count_deb2();
        strcpy(pkg_type,"dpkg");
    }
    else if( (strcmp(info->id,"fedora") == 0 )|| (strstr(info->id_like,"fedora")!= NULL) ){
        *count = get_package_count_rpm();
        strcpy(pkg_type,"rpm");
    }

    else if( (strstr(info->id,"opensuse") != NULL )|| (strstr(info->id_like,"opensuse")!= NULL) ){
        *count = get_package_count_rpm();
        strcpy(pkg_type,"rpm");
    }
    else if( (strcmp(info->id,"arch") == 0 )|| (strstr(info->id_like,"arch")!= NULL) ){
        *count = get_package_count_arch();
        strcpy(pkg_type,"pacman");

    }
    else if( (strcmp(info->id,"alpine") == 0 )|| (strstr(info->id_like,"alpine")!= NULL) ){
        *count = get_package_count_alpine();
        strcpy(pkg_type,"apk");

    }


}
