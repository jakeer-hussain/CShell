#include "headers.h"
void print_virtual_memory(const char *pid) {
    char maps_path[MAX_PATH];
    char line[MAX_LINE];
    FILE *maps_file;

    // Construct path
    snprintf(maps_path, sizeof(maps_path), "/proc/%s/maps", pid);

    // Print Virtual Memory
    maps_file = fopen(maps_path, "r");
    if (maps_file) {
        unsigned long vm_size = 0;
        while (fgets(line, sizeof(line), maps_file)) {
            if (sscanf(line, "%lx", &vm_size) == 1) {
                // Reading the first address in each line
                break;
            }
        }
        fclose(maps_file);

        printf("Virtual Memory Size: %lu KB\n", vm_size / 1024);
    } else {
        printf("Unable to open maps file.\n");
    }
}
void print_process_info(const char *pid) {
    char status_path[MAX_PATH];
    char line[MAX_LINE];
    FILE *status_file;

    // Construct path
    snprintf(status_path, sizeof(status_path), "/proc/%s/status", pid);

    // Print PID
    printf("PID: %s\n", pid);

    // Print Process Status
    status_file = fopen(status_path, "r");
    if (status_file) {
        while (fgets(line, sizeof(line), status_file)) {
            if (strncmp(line, "State:", 6) == 0) {
                printf("Process Status: %s", line + 7);
                break;
            }
        }
        fclose(status_file);
    } else {
        printf("Unable to open status file.\n");
    }

    // Print Process Group
    printf("Process Group: %d\n", getpgid(atoi(pid)));

    // Print Executable Path
    char exe_path[MAX_PATH];
    snprintf(exe_path, sizeof(exe_path), "/proc/%s/exe", pid);
    char exe_link[MAX_PATH];
    ssize_t len = readlink(exe_path, exe_link, sizeof(exe_link) - 1);
    if (len != -1) {
        exe_link[len] = '\0';
        printf("Executable Path: %s\n", exe_link);
    } else {
        printf("Unable to determine executable path.\n");
    }

    // Print Virtual Memory
    print_virtual_memory(pid);
}