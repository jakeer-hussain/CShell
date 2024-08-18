#include "headers.h"
int main()
{
    char initial_directory[4096];
    if(getcwd(initial_directory, sizeof(initial_directory)) == NULL){
        perror("getcwd");
    }
    char* univ_dir = initial_directory;
    CommandEntry history[MAX_HISTORY_SIZE];
    int historyCount = 0;
    while (1){
        prompt(initial_directory);
        char input[MAX_INT_LENGTH],input2[MAX_INT_LENGTH];
        input2[0] = '\0';
        fgets(input, MAX_INT_LENGTH, stdin);
        input[strcspn(input, "\n")] = '\0';
        strcpy(input2,input);
        char* token = strtok(input, " ");
        int i = 0;
        if(i==0 && (strncmp(token,"warp ",4) == 0)){
            while (token != NULL) {
                i++;
                token = strtok(NULL, " ");
                if(i > 0 && token != NULL){
                    if((strncmp(token,".. ",2) == 0) || (strncmp(token,". ",1) == 0) || (token[0] == '~' && token[1] == '/' && token[2]!='\0') ){
                        univ_dir = usenow(token,initial_directory,univ_dir);
                    }
                    else if(token[0] == '-'){
                        char dummydir[MAX_INT_LENGTH];
                        getcwd(dummydir,sizeof(dummydir));
                        printf("%s\n",dummydir);
                        chdir(univ_dir);
                        strcpy(univ_dir, dummydir);
                    }
                    else if(token[0] == '~'){
                        char dummydir[MAX_INT_LENGTH];
                        getcwd(dummydir,sizeof(dummydir));
                        univ_dir = dummydir;
                        chdir(initial_directory);
                        printf("%s\n",initial_directory);
                    }
                    else{
                        char* univdir = usenow(token,initial_directory,univ_dir);
                    }
                }}
            if( i == 1 ){
                chdir(initial_directory);
                printf("%s\n",initial_directory);
            }
        }
        if(i==0 && (strncmp(token,"peek ",4)==0)){
            char** store = (char**)malloc(sizeof(char*)*4);
            while (token != NULL) {
                store[i] = token;
                i++;
                token = strtok(NULL, " ");
                store = (char **)realloc(store, (i + 1) * sizeof(char *));
            }
            if( (i == 3 || i == 4) && ( strcmp(store[1],"-a") == 0 || strcmp(store[1],"-l") == 0 || strcmp(store[1],"-la") == 0 || strcmp(store[1],"-al") == 0 ) ){
                int showHidden = 0, showDetails = 0;
                char *dirPath = NULL;
                if( (i==3 || i==4) && (store[i-1][0] == '~')){
                    char dummydir[MAX_PATH_LENGTH];
                    dummydir[0] = '\0';
                    for(int z=0;z<strlen(store[i-1]);z++){
                        store[i-1][z] = store[i-1][z+1];
                    }
                    strcat(dummydir,initial_directory);
                    strcat(dummydir,store[i-1]);
                    store[i-1] = dummydir;
                }
                for (int y = 1; y < i; y++) {
                    if (strcmp(store[y], "-a") == 0) {
                        showHidden = 1;
                    } else if (strcmp(store[y], "-l") == 0) {
                        showDetails = 1;
                    } else if (strcmp(store[y], "-al") == 0 || strcmp(store[y], "-la") == 0) {
                        showHidden = 1;
                        showDetails = 1;
                    } else {
                        dirPath = store[y];
                    }
                }
                if (dirPath == NULL) {
                    printf("Directory path not provided.\n");
                    return 1;
                }
                listFiles(dirPath, showHidden, showDetails);
            }
            else if( i == 1 ){
                int showHidden = 0, showDetails = 0;
                char dummy[MAX_INT_LENGTH];
                getcwd(dummy,sizeof(dummy));
                char* dirPath = dummy;
                listFiles(dirPath, showHidden, showDetails);
            }
            else{
                int z = 1;
                while( z < i ){
                    if((strcmp(store[z],"..") == 0) || (strcmp(store[z],".") == 0) || (store[z][0] == '~' && store[z][1] == '/' && store[z][2]!='\0')){
                        usenow(store[z],initial_directory,univ_dir);
                    }
                    else if(strcmp(store[z], "~") == 0){
                        chdir(initial_directory);
                        printf("%s\n",initial_directory);
                    }
                    else{
                        char* dummy = usenow(store[z],initial_directory,univ_dir);
                    }
                    z+=1;
                }}
        }
        if(i==0 && (strncmp(token,"proclore ",8)==0)){
            char** store = (char**)malloc(sizeof(char*)*4);
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
        if ((strncmp(input2, "warp ",4) == 0) || (strncmp(input2,"peek ",4) == 0) || (strncmp(input2,"proclore ",8) == 0)){
            if (historyCount == 0 || strcmp(input2, history[historyCount - 1].command) != 0) {
                addCommandToHistory(history, &historyCount, input2);
            }
        }
        else{
            FILE *file = fopen("history.txt", "r");
            if (file != NULL) {
                while (fgets(history[historyCount].command, sizeof(history[historyCount].command), file) != NULL) {
                    history[historyCount].command[strcspn(history[historyCount].command, "\n")] = '\0'; // Remove newline
                    historyCount++;
                    if (historyCount >= MAX_HISTORY_SIZE) {
                        break;
                    }
                }
            fclose(file);
    }
            if (strcmp(input2, "pastevents execute") == 0) {
                executeMostRecentPastEvent(history, historyCount,initial_directory);
            } else if (strncmp(input2, "pastevents execute ", 18) == 0) {
                int index;
                if (sscanf(input2 + 18, "%d", &index) == 1) {
                    executePastEvent(history, historyCount, historyCount - index,initial_directory);
                    if(index!=1){
                    addCommandToHistory(history, &historyCount, history[historyCount-index].command);
                }
                } else {
                    printf("Invalid command format.\n");
                }
            } else if (strcmp(input2, "pastevents") == 0) {
                printPastEvents(history, historyCount);
            } else if (strcmp(input2, "pastevents purge") == 0) {
                historyCount = 0;
                fclose(file);
            } else if (strcmp(input2, "exit") == 0) {
                chdir(initial_directory);
                file = fopen("history.txt", "w");
                if (file != NULL) {
                    for (int i = 0; i < historyCount; ++i) {
                        fprintf(file, "%s\n", history[i].command);
                    }
                    fclose(file);
                }
                break;
            } else {
                if (historyCount == 0 || strcmp(input2, history[historyCount - 1].command) != 0) {
                    addCommandToHistory(history, &historyCount, input2);
                }
                char output[MAX_INT_LENGTH];
                FILE *cmd_output = popen(input2, "r");
                if (cmd_output == NULL) {
                    printf("Failed to execute command.\n");
                    continue;
                }
                while (fgets(output, sizeof(output), cmd_output) != NULL) {
                    printf("%s", output);
                }
                pclose(cmd_output);
            }
        }
    }
return 0;
}