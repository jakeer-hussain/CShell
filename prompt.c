#include "headers.h"

void prompt(char* initial_directory) {
    // Do not hardcode the prmopt
    char *username = getlogin();
    if (username == NULL) {
        perror("getlogin");
        return;
    }
    struct utsname sys_info;
    if (uname(&sys_info) == -1) {
        perror("uname");
        return;
    }
    // Get the initial directory from which the shell was invoked
    // char initial_directory[MAX_PATH_LENGTH];
    // if (getcwd(initial_directory, sizeof(initial_directory)) == NULL) {
    //     perror("getcwd");
    //     return;
    // }
    char current_directory[MAX_PATH_LENGTH];
    if (getcwd(current_directory, sizeof(current_directory)) == NULL) {
        perror("getcwd");
        return;
    }
    // Display prompt
    // printf("%s\n",initial_directory);
    if (strncmp(current_directory, initial_directory, strlen(initial_directory)) == 0) {
        // printf("<%s@%s:~%s> ", username, sys_info.nodename, current_directory + strlen(initial_directory));
        current_directory[0] = '~';
        memmove(current_directory + 1,current_directory + strlen(initial_directory), strlen(current_directory) - strlen(initial_directory) + 1);
    }
    printf("<%s@%s:%s> ", username, sys_info.nodename, current_directory);
    // printf("<Everything is a file> ");
}