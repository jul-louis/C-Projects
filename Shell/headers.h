//
// Created by Luis Xie on 2021-09-25.
//

#ifndef UNTITLED8_HEADERS_H
#define UNTITLED8_HEADERS_H

#include <sys/types.h>
#include <ctype.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

extern const int MAX_LENGTH;
extern const int LINE_LENGTH;

extern pid_t pid;
extern long re_u_sec, re_sys_sec;

bool get_command(char cmd[], char* pid_str);
long time_calculate(bool is_user_time, bool is_sec, struct rusage end, struct rusage start);
void jobs(char* pid_str);
void refresh_usage(pid_t id);
void printer(char* filename, int count);
bool is_a_process_of_the_group(char* gid, char* qid_str);
void wait_any_process(char* qid_str, char* gid);
void bash(char * cmd, char* args);
bool isPurelyDigital(char* args);
void trim(char* src, char* out, bool trailing);
void update_process(bool when_jobs);
pid_t get_id (char* filename);
int count_running_spawned_process(char* filename);
void time_update(struct rusage end, struct rusage start);

#endif //UNTITLED8_HEADERS_H
