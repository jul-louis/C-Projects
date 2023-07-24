//
// Created by Luis Xie on 2021-09-27.
//
#include "headers.h"

bool isPurelyDigital(char* arg) {
    bool passed = true;
    unsigned int len = strlen(arg);
  //  printf("length: %d\n", len);
    int index = 0;
    while (index < len && passed && arg[index] != '\0') {
       // printf("%c\n", arg[index]);
        if (!isdigit(arg[index])) {
           // printf("This, %c\n", arg[index]);
            passed = false;
        }
        index++;
    }
    return passed;
}

void trim(char* arg, char* backup, bool trim_trail_only) {
    int index_start = 0;
    unsigned int len = strlen(arg);
    unsigned int index_end = len-1;

    if (!trim_trail_only) {
        while (index_start < len && isspace(arg[index_start])) {
            index_start++;
        }
        printf("start: %d\n", index_start);
    }

    while (index_end>0 && isspace(arg[index_end])) {
        index_end--;
    }
    printf("end: %d\n", index_end);

    if (index_start > index_end && index_end==0) {
        backup = "";
    } else {
        strncpy(backup, arg + index_start, index_end - index_start + 1);
        backup[index_end - index_start + 1] = '\0';
        // mark the end of the str with a '\0'.
    }
}
/*
bool cannot_be_tracked(pid_t proc_id) {
    bool trackable = false;

    bash("ps", " -o ppid");
    return trackable;
}
 */

void printer(char* filename, int count) {
    printf("#  ");
    int character;
    int index = 0;

    FILE * out = fopen(filename, "r");

    if (out == NULL) {
        perror("Can't open file");
        _exit(EXIT_FAILURE);
    }

    character = fgetc(out);
    int last;
    while (1) {

        if (feof(out)) {
            break;
        } else {
            printf("%c", character);
            last = character;
            character = fgetc(out);
            if (last == '\n' && index <= count-1) {
                printf("%d: ", index);
                index++;
            }
        }

    }
    fclose(out);
}

pid_t get_id (char* filename) {
    FILE * file_p = fopen(filename, "r");

    if (file_p == NULL) {
        perror("Can't open file");
        return -1;
    }

    while (1) {

        if (feof(file_p)) {
            break;
        } else {
            char line[MAX_LENGTH];
            fscanf(file_p, "%20s", line);
            if (isPurelyDigital(line)) {
                pid_t id = atoi(line);
                fclose(file_p);
                printf("will kill pid: %d\n", id);
                return id;
            }
        }
    }
    fclose(file_p);
    return -1;
}

/*
void wait_any_process(char* query_str, char* g_id) {
    char args[LINE_LENGTH];
    strcpy(args,"while kill -0 ");
    strcat(args, query_str);
    strcat(args, " 2> /dev/null; do sleep 1; done;");
    char * argv[]= {"bash", "-c", args, NULL };

    pid_t child;
    if ((child=fork())<0) {
        perror("fork");
    } else if (child == 0) {
        if (!is_a_process_of_the_group(g_id, query_str)) {
            printf("Wait Failed: Neither a child process nor a spawned process.\n");
        } else {
            if (execve("/bin/bash", argv, NULL) == -1) {
                perror("Can't wait for the process, check your input first.");
            }
        }
    } else {
        int status;
        waitpid(child, &status, 0);
    }
}

bool is_a_process_of_the_group(char* gid_str, char* query) {
    char commands[LINE_LENGTH * 2];
    char *basic = "ps -o pid,state -g ";
    strcpy(commands, basic);
    strcat(commands, gid_str);
    strcat(commands, " -p ") ;strcat(commands, query);
    strcat(commands, " | grep -vE 'grep|ps|awk' | awk '$2~/S|S[s]|R|R[+]/' >check.txt");

    puts("NOTE: your command!\n");
    printf("%s\n", commands);
    puts("NOTE: your command!\n");

    char *argv1[] = {"bash", "-c", commands, NULL};

    pid_t child;
    if ((child = fork()) < 0) {
        perror("fork error");
    } else if (child == 0) {

        printf("child pid: %d\n", getpid());

        if (execve("/bin/bash", argv1, NULL) == -1) {
            perror("ERROR");
            _exit(EXIT_FAILURE);

        }
        _exit(EXIT_SUCCESS);
    } else {
        int status;
        waitpid(child,&status,0);
        printf("Child process ends, return.\n");
    }

    int c = count_running_spawned_process("check.txt");
    if (c<=0) {
        return false;
    } else {
        return true;
    }
}
*/
void refresh_usage(pid_t id) {
    int status;
    struct rusage re_usage;
    waitpid(id, &status, WNOHANG);
    getrusage(RUSAGE_CHILDREN, &re_usage);

    re_u_sec = re_usage.ru_utime.tv_sec;
    re_sys_sec = re_usage.ru_stime.tv_sec;
}

int count_running_spawned_process(char* filename) {
    FILE * out_running = fopen(filename, "r");
    int character;

    int count_total = -1;
    while (1) {
        if (feof(out_running)) {
            break;
        }
        else if (fgetc(out_running) == '\n') {
            count_total ++;
        }
    }
    fclose(out_running);

    printf("finished\n");

    return count_total;
}
