//
//  CMPUT 379 Assignment 1 - SHELL 379
//  Louie Xie 1571535, FALL 2021
//

#include "headers.h"

const int LINE_LENGTH = 100;
const int MAX_LENGTH = 20;

pid_t pid;
long re_u_sec, re_sys_sec = 0;
long u_sec = 0;
long u_micro_sec = 0;
long sys_sec = 0;
long sys_micro_sec = 0;

int main(int argc, char * argv[]) {

    struct rusage ru_start, ru_end, ru_c_start, ru_c_end;
    getrusage(RUSAGE_SELF, &ru_start);
    getrusage(RUSAGE_CHILDREN, &ru_c_start);

    char  pid_str[LINE_LENGTH];
    pid = getpid();

    sprintf(pid_str, "%d" , pid);

    printf("%ld.%06d\n", ru_start.ru_utime.tv_sec, ru_start.ru_utime.tv_usec);

    pid = getpid();
    printf("pid: %d\n", pid);

    bool continue_loop = true;
    while (continue_loop) {
        char command[MAX_LENGTH];
        printf("SHELL379: ");
        scanf("%s", command);

        sprintf(pid_str, "%d" , getpid());

        continue_loop = get_command(command, pid_str);
        update_process(false);
        refresh_usage(get_id("pid_table"));
    }

    wait(NULL);
    getrusage(RUSAGE_SELF, &ru_end);
    getrusage(RUSAGE_CHILDREN, &ru_c_end);


    time_update(ru_end, ru_start);
    time_update(ru_c_end, ru_c_start);


    printf("Resources used: \nUser time: %ld.%06ld sec\nSys time: %ld.%06ld sec\n",
           u_sec + u_micro_sec/1000000,u_micro_sec-u_micro_sec/1000000,
           sys_sec + sys_micro_sec/1000000,sys_micro_sec-sys_micro_sec/1000000);

    exit(EXIT_SUCCESS);

}

long time_calculate(bool is_user_time, bool is_sec, struct rusage ru_end, struct rusage ru_start) {
    if (is_user_time) {

        if (is_sec) {
            return (ru_end.ru_utime.tv_sec - ru_start.ru_utime.tv_sec);
        } else {
            return (long) (ru_end.ru_utime.tv_usec - ru_start.ru_utime.tv_usec);
        }

    } else {
        if (is_sec) {
            return (ru_end.ru_stime.tv_sec - ru_start.ru_stime.tv_sec);
        } else {
            return (long) (ru_end.ru_stime.tv_usec - ru_start.ru_stime.tv_usec);
        }
    }
}

bool get_command(char * command, char* pid_str) {
    if (strcmp(command, "exit")==0) {

        pid_t id = get_id("pid_table");
        while (id) {
            if (kill(id, SIGTERM)<0) {
               perror("SIGTERM");
               kill(id, SIGKILL);
            }
            update_process(false);
            id = get_id("pid_table");
        }
        return false;
    } else if (strcmp(command, "jobs")==0){
        jobs(pid_str);
        return true;

    } else if (strcmp(command, "kill")==0) {

        char arg[MAX_LENGTH];
        char trimmed_arg[MAX_LENGTH];
        fgets(arg, MAX_LENGTH, stdin);

        trim(arg, trimmed_arg, false);

        if (isPurelyDigital(trimmed_arg)) {
            printf("actually going this way\n");
            pid_t proc_id = atoi(trimmed_arg);

            if(kill(proc_id, SIGTERM)<0) {
                perror("SIGTERM failed");
                if(kill(proc_id, SIGKILL)<0) {
                    perror("SIGKILL failed");
                }
            }
        } else {
            bash(command, arg);
            return true;
        }

        return true;

    } else if (strcmp(command, "resume")==0) {

        char arg[MAX_LENGTH];
        char trimmed_arg[MAX_LENGTH];
        fgets(arg, MAX_LENGTH, stdin);

        trim(arg, trimmed_arg, false);

        if (isPurelyDigital(trimmed_arg)) {
            pid_t proc_id = atoi(trimmed_arg);

            if(kill(proc_id, SIGCONT)<0) {
                perror("SIGCONT failed");
            }
        } else {
            bash(command, arg);
            return true;
        }

        return true;

    } else if (strcmp(command, "sleep")==0) {
        char arg[MAX_LENGTH];
        char trimmed_arg[MAX_LENGTH];

        fgets(arg, MAX_LENGTH, stdin);
        trim(arg, trimmed_arg, false);

        if (isPurelyDigital(trimmed_arg)) {
            int sec = atoi(trimmed_arg);
            sleep(sec);
        } else {
            bash(command, arg);
            return true;
        }

        return true;

    } else if (strcmp(command, "suspend")==0) {
        char arg[MAX_LENGTH];
        char trimmed_arg[MAX_LENGTH];

        fgets(arg, MAX_LENGTH, stdin);
        trim(arg, trimmed_arg, false);

        if (isPurelyDigital(trimmed_arg)) {
            pid_t proc_id = atoi(trimmed_arg);

            if(kill(proc_id, SIGSTOP)<0) {
                perror("SIGCONT failed");
            }
        } else {
            bash(command, arg);
            return true;
        }
        return true;
    } else if (strcmp(command, "wait")==0) {
        char arg[MAX_LENGTH];
        char trimmed_arg[MAX_LENGTH];
        fgets(arg, MAX_LENGTH, stdin);

        trim(arg, trimmed_arg, false);

        if (isPurelyDigital(trimmed_arg)) {
            pid_t proc_id = atoi(trimmed_arg);
            int status;

            if(waitpid(proc_id, &status, 0)<0) {
                if (strcmp(pid_str,trimmed_arg)==0) {
                    return true;
                } else {
                    perror("waitpid error");
                    //wait_any_process(trimmed_arg, pid_str);
                }
            }
        } else {
            bash(command, arg);
            return true;
        }
        return true;
    } else {
        char arguments[LINE_LENGTH];
        char trimmed_arg[MAX_LENGTH];

        fgets(arguments, LINE_LENGTH, stdin);
        trim(arguments, trimmed_arg, true);

        bash(command, arguments);

        return true;
    }
}
