#include "headers.h"

char* usenow(char* dummytoken,char* initial_directory,char* univ_dir){
    if((dummytoken[0] == '.' && dummytoken[1] == '.' && dummytoken[2] == '\0')){
        char current[MAX_PATH_LENGTH];
        getcwd(current,sizeof(current));
        chdir("..");
        char current_directory[MAX_PATH_LENGTH];
        getcwd(current_directory, sizeof(current_directory));
        printf("%s\n",current_directory);
        univ_dir = current;
        return univ_dir;
    }
    else if(dummytoken[0] == '.'){
        char dummy[MAX_PATH_LENGTH];
        getcwd(dummy, sizeof(dummy));
        printf("%s\n",dummy);
        univ_dir = dummy;
        return univ_dir;
    }
    else if(dummytoken[0] == '~' && dummytoken[1] == '/' && dummytoken[2]!='\0'){
        char dummy[MAX_PATH_LENGTH];
        char dummydir[MAX_INT_LENGTH];
        dummydir[0] = '\0';
        getcwd(dummydir,sizeof(dummydir));
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
        univ_dir = dummydir;
        return univ_dir;
    }
    else{
        char current_directory[MAX_PATH_LENGTH];
        getcwd(current_directory, sizeof(current_directory));
        univ_dir = current_directory;
        char* dir2 = current_directory;
        char dummy[] = "/";
        strcat(current_directory,dummy);
        strcat(current_directory,dummytoken);
        if(chdir(dir2) != 0){
            perror("Error in changing directory");
        }
        else{
            printf("%s\n",current_directory);
        }
        return univ_dir;
    }
}
