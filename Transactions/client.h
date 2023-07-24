// Assignment 3: File - client.h

#ifndef INC_379A4_CLIENT_H
#define INC_379A4_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_IP_LENGTH 16
#define MAX_FILENAME_LEN 200
#define MAX_PID_LEN 100
#define MAXIMAL_LENGTH_PER_LINE 12


void client_parse_arguments(int argc, char* argv[], int arg_num);
bool is_valid_ip(char* ip_str);
void client_initialize();
void client_create_log_file();
void client_parse_work();

#endif //INC_379A4_CLIENT_H
