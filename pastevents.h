#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H
typedef struct {
    char command[4096];
} CommandEntry;

void addCommandToHistory(CommandEntry history[], int *historyCount, const char *command);
void printPastEvents(CommandEntry history[], int historyCount);
void executePastEvent(CommandEntry history[], int historyCount, int index,char* initial_directory);
void executeMostRecentPastEvent(CommandEntry history[], int historyCount,char* initial_directory);
void proclore(char* dummytoken);
char* warp(char* token,char* initial_directory);
// int executeCommand(const char *command);
#endif
