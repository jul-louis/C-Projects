//
// Created by Luis Xie on 2021-11-03.
//

#ifndef INC_379A4_SERVER_H
#define INC_379A4_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <fcntl.h>
#include "dynamic_queue.h"

#define MAX_CONNECTION 20

void server_parse_arguments(int argc, char* argv[], int arg_num);
void log_server_receive(char* host, char* n_string);
void log_server_done(char* host);
void server_routine();
void server_initialize();
void client_parse_work();
void log_client_send(char* value_n);
void server_create_log_file(pid_t pid);

#endif //INC_379A4_SERVER_H
