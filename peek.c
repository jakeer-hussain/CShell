#include "headers.h"

int compareNames(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void listFiles(const char *dirPath, int showHidden, int showDetails) {
    DIR *dir = opendir(dirPath);
    // printf("%s\n",dirPath);
    if (dir == NULL) {
        perror("opendir");
        return;
    }
    // printf("123%s\n",dirPath);
    struct dirent *entry;
    char **fileNames = NULL;
    int numFiles = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.' && !showHidden) {
            // printf("you are la\n");
            continue;
        }

        if (numFiles == 0) {
            // printf("you are la2\n");
            fileNames = (char **)malloc(sizeof(char *));
        } else {
            fileNames = (char **)realloc(fileNames, (numFiles + 1) * sizeof(char *));
            // printf("123%s\n",dirPath);
        }

        if (fileNames == NULL) {
            perror("malloc/realloc");
            return;
        }

        fileNames[numFiles] = strdup(entry->d_name);
        numFiles++;
    }
    // printf("123%s\n",dirPath);
    // printf("%s\n",fileNames[0]);
    closedir(dir);

    if (numFiles > 0) {
        qsort(fileNames, numFiles, sizeof(char *), compareNames);

        for (int i = 0; i < numFiles; i++) {
            char filePath[512];
            snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, fileNames[i]);

            struct stat fileStat;
            if (stat(filePath, &fileStat) == 0) {
                if (showDetails) {
                    char permissions[11];
                    strcpy(permissions, "----------");
                    if (S_ISDIR(fileStat.st_mode)) permissions[0] = 'd';
                    if (fileStat.st_mode & S_IRUSR) permissions[1] = 'r';
                    if (fileStat.st_mode & S_IWUSR) permissions[2] = 'w';
                    if (fileStat.st_mode & S_IXUSR) permissions[3] = 'x';
                    if (fileStat.st_mode & S_IRGRP) permissions[4] = 'r';
                    if (fileStat.st_mode & S_IWGRP) permissions[5] = 'w';
                    if (fileStat.st_mode & S_IXGRP) permissions[6] = 'x';
                    if (fileStat.st_mode & S_IROTH) permissions[7] = 'r';
                    if (fileStat.st_mode & S_IWOTH) permissions[8] = 'w';
                    if (fileStat.st_mode & S_IXOTH) permissions[9] = 'x';

                    struct passwd *pw = getpwuid(fileStat.st_uid);
                    struct group *gr = getgrgid(fileStat.st_gid);
                    struct tm *tm = localtime(&fileStat.st_mtime);
                    char timeStr[80];
                    strftime(timeStr, sizeof(timeStr), "%b %d %H:%M", tm);

                    printf("%s %ld %s %s %lld %s ", permissions, (long)fileStat.st_nlink,
                           pw->pw_name, gr->gr_name, (long long)fileStat.st_size, timeStr);
                }

                if (S_ISDIR(fileStat.st_mode)) {
                    printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET "\n", fileNames[i]);
                } else if (fileStat.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
                    printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET "\n", fileNames[i]);
                } else {
                    printf("%s\n", fileNames[i]);
                }
            }

            free(fileNames[i]);
        }
        // printf("123%s\n",dirPath);
        free(fileNames);
    }
}
