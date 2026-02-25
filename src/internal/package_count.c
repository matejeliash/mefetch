#include "package_count.h"

#include "cmd_runner.h"
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>



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
        if ((line_start[0] == 'i' && line_start[1] =='i') ||(line_start[0] == 's' && line_start[1] == 'i')) {
            count++;
        }

        char* next_newline= strchr(line_start,'\n');
        if (!next_newline) break;
        line_start = next_newline + 1;
    }

    free(str);

    return count;
}

// works for newer RedHat distros like fedora
int get_package_count_fedora() {

    // sqlite db used by dnf5>
    char * db_path = "/var/lib/rpm/rpmdb.sqlite";

    int rc;
    sqlite3 *db;
        rc = sqlite3_open(db_path, &db);

    if (rc != SQLITE_OK) {
        //printf("Cannot open DB: %s\n", sqlite3_errmsg(db));
        if(db) sqlite3_close(db);

        return -1;
    }
    sqlite3_stmt *stmt;

    // select number of rows from Packages table
    const char *sql = "SELECT COUNT(1) FROM Packages;";

    // Prepare statement
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        //printf("Prepare error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    // just retrieve one row
    rc = sqlite3_step(stmt);
    int count ;

    if (rc == SQLITE_ROW) {
         count = sqlite3_column_int(stmt,0);
    }else{
       count = -1;
    }


    // cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return count;
}



int  get_package_count_rpm(){

    char* cmd[] = { "rpm", "-qa", "--nosignature", "--nodigest", "--qf", "'%{NAME}\n'", NULL};
    int count = run_cmd_line_counter(cmd);
    return count;
}