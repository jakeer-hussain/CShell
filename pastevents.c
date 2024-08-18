#include "headers.h"
void addCommandToHistory(CommandEntry history[], int *historyCount, const char *command) {
    // printf("added\n");
    if (*historyCount == MAX_HISTORY_SIZE) {
        for (int i = 0; i < *(historyCount) - 1; ++i) {
            strcpy(history[i].command, history[i + 1].command);
        }
        *(historyCount) = *(historyCount) - 1;
    }
    strcpy(history[*(historyCount)].command, command);
    *(historyCount) = *(historyCount) + 1;
    // printf("%d",*historyCount);
}

void printPastEvents(CommandEntry history[], int historyCount) {

    if (historyCount > 0) {
        // printf("Past Events:\n");
        for (int i = 0; i < historyCount; ++i) {
            printf("%s\n", history[i].command);
        }
    } 
    // else {
    //     printf("No past events.\n");
    // }
}
char* warp(char* token,char* initial_directory){
    char *dummytoken = strtok(token, " ");
    int i = 0;
    while(dummytoken != NULL){
        if(strcmp(dummytoken,"warp" )!= 0){
            i=1;
        }
        if((dummytoken[0] == '.' && dummytoken[1] == '.' && dummytoken[2] == '\0')){
            chdir("..");
            char current_directory[MAX_PATH_LENGTH];
            getcwd(current_directory, sizeof(current_directory));
            printf("%s\n",current_directory);
        }
        else if(dummytoken[0] == '.'){
            char dummy[MAX_PATH_LENGTH];
            getcwd(dummy, sizeof(dummy));
            printf("%s\n",dummy);
        }
        else if(dummytoken[0] == '~' && dummytoken[1] == '/' && dummytoken[2]!='\0'){
            char dummy[MAX_PATH_LENGTH];
            dummy[0] = '\0';
            dummytoken[0] = '\0';
            char* dummytoken2 = dummytoken + 1;
            strcat(dummy,initial_directory);
            strcat(dummy,dummytoken2);
            if(chdir(dummy) != 0){
                perror("Error in opening files");
            }
            else{
                printf("%s\n",dummy);
            }
        }
        else if(dummytoken[0] == '~' && dummytoken[1] == '\0'){
            char dummydir[MAX_INT_LENGTH];
            getcwd(dummydir,sizeof(dummydir));
            chdir(initial_directory);
            printf("%s\n",initial_directory);
        }
        else if(i!=0){
            char current_directory[MAX_PATH_LENGTH];
            getcwd(current_directory, sizeof(current_directory));
            char* dir2 = current_directory;
            char dummy[] = "/";
            strcat(current_directory,dummy);
            strcat(current_directory,dummytoken);
            if(chdir(dir2) != 0){
                perror("Error in changing directory");
            }
        }
        dummytoken = strtok(NULL, " ");
    }
}
void proclore(char* dummytoken){
    char** store = (char**)malloc(sizeof(char*)*4);
    char *token = strtok(dummytoken, " ");
    int i=0;
    while (token != NULL) {
        store[i] = token;
        i++;
        token = strtok(NULL, " ");
        store = (char **)realloc(store, (i + 1) * sizeof(char *));
    }
    if (i==1) {
        // Print shell information
        char shell_pid[10];
        sprintf(shell_pid, "%d", getpid());
        print_process_info(shell_pid);
    } else {
        // Print information about the specified process
        print_process_info(store[1]);
    }
}
void executePastEvent(CommandEntry history[], int historyCount, int index,char* initial_directory) {
    if (index >= 0 && index < historyCount) {
        char output[MAX_INT_LENGTH];
        if((strncmp(history[index].command,"warp ",4) == 0) || (strncmp(history[index].command,"peek ",4) == 0)){
            warp(history[index].command,initial_directory);
            return;
        }
        else if(strncmp(history[index].command,"proclore ",8) == 0){
            proclore(history[index].command);
            return;
        }
        FILE *cmd_output = popen(history[index].command, "r");
        if (cmd_output == NULL) {
            printf("Failed to execute command.\n");
            return;
        }

        while (fgets(output, sizeof(output), cmd_output) != NULL) {
            printf("%s", output);
        }
        pclose(cmd_output);
    } else {
        printf("Invalid event index.\n");
    }
}

void executeMostRecentPastEvent(CommandEntry history[], int historyCount,char* initial_directory) {
    if (historyCount > 0) {
        if((strncmp(history[historyCount-1].command,"warp ",4) == 0) || (strncmp(history[historyCount-1].command,"peek ",4) == 0)){
            warp(history[historyCount-1].command,initial_directory);
            return;
        }
        executePastEvent(history,historyCount, historyCount - 1,initial_directory);
    } else {
        printf("No past events.\n");
    }
}
int executeCommand(const char *command) {
    char output[MAX_INT_LENGTH];
    FILE *cmd_output = popen(command, "r");
    if (cmd_output == NULL) {
        printf("Failed to execute command.\n");
        return 0;  // Return failure
    }
    while (fgets(output, sizeof(output), cmd_output) != NULL) {
        printf("%s", output);
    }

    pclose(cmd_output);
    return 1;  // Return success
}