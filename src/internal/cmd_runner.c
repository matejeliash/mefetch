#include "cmd_runner.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

// runs selected program in new process,
// creates pipe  and writes stdout to buffer
char *run_cmd(char** argv) {
    //create pipe
    int pipefd[2];
    if (pipe(pipefd) < 0) return NULL;

    // create child process
    pid_t pid = fork();
    if (pid < 0) {
        close(pipefd[0]);
        close(pipefd[1]);
        return NULL;
    }

    if (pid == 0) {
        // child process
        close(pipefd[0]);
        // redirect child process stdout to pipe
        dup2(pipefd[1], STDOUT_FILENO);  // 
        close(pipefd[1]);
        execvp(argv[0], argv);
        _exit(127); // exec failed
    }

    // parent process write close
    close(pipefd[1]);

    // create buffer for reading from pipe
    size_t cap = 1024*10, len = 0;
    char *buf = malloc(cap);
    if (!buf) {
        close(pipefd[0]);
        waitpid(pid, NULL, 0);
        return NULL;
    }
    ssize_t n;
    // read from pipe 
    while ((n = read(pipefd[0], buf + len, cap - len)) > 0) {
        len += n;
        // double size when cap reached
        if (len == cap) {
            cap *= 2;
            char *tmp = realloc(buf, cap);
            // realloc was not successful ,
            if (!tmp) {
                free(buf);
                close(pipefd[0]);
                waitpid(pid, NULL, 0);
                return NULL;
            }
            buf = tmp;
        }
    }

    close(pipefd[0]);
    waitpid(pid, NULL, 0);

    // add terminator 
    buf[len] = '\0';
    return buf;
}

// run program in process, user 
int run_cmd_line_counter(char** argv){
    char* str = run_cmd(argv);
    if (!str){
        return -1;
    }

    char* p = str;  // keep original pointer for free
    int count = 0;
    while(*p){
        if (*p == '\n'){
            count++;
        }
        p++;
    }

    free(str);  // Free the original pointer
    return count;
}
