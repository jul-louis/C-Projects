//
// Created by Luis Xie on 2021-11-18.
//

#ifndef CMPUT379A3_SERVER_SUPPORT_H
#define CMPUT379A3_SERVER_SUPPORT_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define MAX_FILENAME_LEN 200
#define MAX_PID_LEN 100

bool is_purely_digital(char* arg) {
    // This function will test if a string contains only digital characters.
    // Ref: From CMPUT379 Assignment 1 by Louie Xie
    bool passed = true;
    unsigned int len = strlen(arg);

    int index = 0;
    while (index < len && passed && arg[index] != '\0') {

        if (!isdigit(arg[index])) {
            // if any char in the string is non-digital, NOT PURELY DIGITAL - return false.
            passed = false;
        }
        index++;
    }
    return passed;
}

// Use functions in tands.c
void Trans(int n);
void Sleep(int n);

#endif //CMPUT379A3_SERVER_SUPPORT_H
