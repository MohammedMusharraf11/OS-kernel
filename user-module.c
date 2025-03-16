#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

#define MAX_PROCESSES 100
#define FILENAME "pids.txt"

struct Process {
    pid_t pid;
    pid_t ppid;
    int visited;
};

void write_pid_info() {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%d %d\n", getpid(), getppid());
    fclose(file);
}

void allocate_memory() {
    char *buffer = malloc(1024 * 1024); // Allocate 1MB
    if (buffer == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    memset(buffer, 0, 1024 * 1024); // Prevent compiler optimizations
}

void create_children(int level) {
    if (level <= 0) return;

    for (int i = 0; i < 2; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Child process
            write_pid_info();
            allocate_memory();
            create_children(level - 1);
            sleep(10); // Keep process alive for observation
            exit(EXIT_SUCCESS);
        }
    }
}

void print_memory_map(pid_t pid, int depth) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/maps", pid);
    FILE *file = fopen(path, "r");
    if (file) {
        char line[1024];
        while (fgets(line, sizeof(line), file)) {
            for (int j = 0; j < depth + 1; j++) printf("│   ");
            printf("│   %s", line);
        }
        fclose(file);
    }
}

void print_tree(struct Process *processes, int count, pid_t current_pid, int depth) {
    for (int i = 0; i < count; i++) {
        if (processes[i].ppid == current_pid && !processes[i].visited) {
            processes[i].visited = 1;
            for (int j = 0; j < depth; j++) printf("│   ");
            printf("├─ PID: %d (Parent: %d)\n", processes[i].pid, processes[i].ppid);

            print_memory_map(processes[i].pid, depth);
            print_tree(processes, count, processes[i].pid, depth + 1);
        }
    }
}

int main() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    fclose(file);
    
    write_pid_info();
    create_children(2);
    sleep(2);

    // Read PID info
    file = fopen(FILENAME, "r");
    if (!file) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    struct Process processes[MAX_PROCESSES];
    int count = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) && count < MAX_PROCESSES) {
        sscanf(line, "%d %d", &processes[count].pid, &processes[count].ppid);
        processes[count].visited = 0;
        count++;
    }
    fclose(file);

    // Root process memory map
    pid_t main_pid = getpid();
    printf("Main PID: %d\n", main_pid);
    print_memory_map(main_pid, 0);

    print_tree(processes, count, main_pid, 0);

    // Cleanup child processes
    for (int i = 0; i < count; i++) {
        if (processes[i].pid != main_pid) {
            kill(processes[i].pid, SIGTERM);
        }
    }

    return EXIT_SUCCESS;
}
