//
// Created by Luis Xie on 2021-09-26.
//
#include "headers.h"

extern long u_sec, sys_sec;
extern long u_micro_sec, sys_micro_sec;

// Note: when recoding the running child processes, excluding the shell and the child process that doing jobs().

void jobs(char* pid_str) {
    printf("Running processes: \n");
    update_process(true);
    int count = count_running_spawned_process("running_proc");
    printf("Processes =\t\t %d active\n", count);
    // TODO: Print the active proc table:
    if (count) {
        printer("running_proc", count);
    }

    printf("Completed processes:\n");
    printf("User Time: %ld\nSystem Time: %ld\n", re_u_sec, re_sys_sec);
    // TODO: KILL ALL process LEFT BEHIND.
}

void time_update(struct rusage end, struct rusage start) {
    u_sec += time_calculate(true, true, end, start);
    u_micro_sec += time_calculate(true, false, end, start);
    sys_sec += time_calculate(false, true, end, start);
    sys_micro_sec += time_calculate(false, false, end, start);
}

// This function will update job list.
void update_process(bool when_jobs) {

    char commands[LINE_LENGTH * 2];
    if (when_jobs) {
        char *basic = "ps -o pid,state=S,cputime=SEC,comm";
        //char *basic = "ps -o pid,state=S,cputimes=SEC,comm"; // cputimes works on LAB machines
        strcpy(commands, basic);
        strcat(commands, " | grep -vE 'grep|ps|ssh|awk|bash|(./)?shell379' | awk '$2~/S|S[s]|R|R[+]/' >running_proc");
    } else {
        char *basic = "ps -o pid,state,comm";
        //char *basic = "ps -o pid,state=S,cputimes=SEC,comm"; // cputimes works on LAB machines
        strcpy(commands, basic);
        strcat(commands, " | grep -vE 'grep|ps|ssh|awk|bash|(./)?shell379' | awk '$2~/S|S[s]|R|R[+]/' >pid_table");
    }
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
}


void bash(char * command, char * args) {

    bool background = false;

    printf("the last: %c\n", args[strlen(args)-2]);

    if (args[strlen(args)-2] == '&') {
        printf("This process will be running in background.\n");
        args[strlen(args)-2] = '\0';
        background = true;
    }

    char * argv[] = {"bash","-c",strcat(command,args), NULL};

    pid_t child;

    if (!background) {

        if ((child = fork()) < 0) {
            perror("fork failed");
        } else if (child == 0) {
            printf("Now executing something.\n");
            if (execve("/bin/bash", argv, NULL) < 0) {
                perror("Command Error");
                _exit(EXIT_FAILURE);
            }

        } else {
            int status;
            waitpid(child, &status, 0);
            printf("Child process ends, return.\n");

            return;
        }
    } else {

        if ((child = fork()) < 0) {
            perror("fork failed");
        } else if (child == 0) {
            printf("BG: Executing.\n");
            if (execve("/bin/bash", argv, NULL) < 0) {
                perror("Command Error");
                _exit(EXIT_FAILURE);
            }
        } else {
            printf("Running the child in the background\nParent can still work.\n");

            return;
        }
    }
}